import os
from time import time  # https://www.geeksforgeeks.org/get-current-date-and-time-using-python/?ref=rp and https://www.tutorialspoint.com/sqlite/sqlite_date_time.htm

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp  # https://www.geeksforgeeks.org/create-temporary-files-and-directories-using-python-tempfile/
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash  # https://techmonger.github.io/4/secure-passwords-werkzeug/

from helpers import apology, login_required, lookup, usd, ftime

# Configure application. On app.config --> https://hackersandslackers.com/configure-flask-applications/
app = Flask(__name__)

# Ensure templates are auto-reloaded --> https://stackoverflow.com/questions/9508667/reload-flask-app-when-template-file-changes
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request  # https://pythonise.com/series/learning-flask/python-before-after-request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter. For more info on jinja2 --> https://flask.palletsprojects.com/en/1.1.x/templating/
app.jinja_env.filters["usd"] = usd
app.jinja_env.filters["ftime"] = ftime

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get User id
    user_id = session["user_id"]

    # Get Stock Info
    table = []
    rows = db.execute("SELECT symbol, SUM(shares) AS shares FROM transactions WHERE user_id= :user_id GROUP BY symbol", user_id=user_id)
    for row in rows:
        quoted = lookup(row["symbol"])
        current_price = quoted["price"]
        table_row = {"symbol":row["symbol"], "shares":row["shares"], "current_price":current_price}
        table.append(table_row)

    # Get cash info
    rows = db.execute("SELECT cash FROM users WHERE id= :id", id=user_id)

    return render_template("index.html", cash=rows[0]["cash"], table=table)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # Get user id
        user_id = session["user_id"]

        # Check if symbol was submitted and Check if Company exists.
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        if not quoted:
            return apology("Symbol not Recognized")

        # Get time data as early as possible
        buying_time = time()

        # Check if number of shares was submitted and it is a positive integer
        shares = request.form.get("shares")
        if not shares.isdigit() or int(shares) <= 0:
            return apology("Enter a positive Integer")

        # Check if user has enough money
        cost = int(shares)*quoted["price"]
        rows = db.execute("SELECT * FROM users WHERE id= :id", id=user_id)
        if cost > rows[0]["cash"]:
            return apology("Not Enough Money in Wallet")

        # Buy shares
        cash_left = rows[0]["cash"] - cost

        # Update users table
        db.execute("UPDATE users SET cash = :cash WHERE id = :id", id=user_id, cash=cash_left)

        # Log Transaction
        db.execute("INSERT INTO transactions (symbol, shares, action, price, time, user_id) VALUES (:symbol, :shares, :action, :price, :time, :user_id)", symbol=quoted["symbol"], shares=shares, action="buy", price=quoted["price"], time=int(buying_time), user_id=user_id)

        flash("Shares bought successfully!")

        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # Get user id
    user_id = session["user_id"]

    # Get Transaction Log
    table = db.execute("SELECT * FROM transactions WHERE user_id = :user_id", user_id=user_id)

    return render_template("history.html", table=table)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        username = request.form.get("username")
        if not username:
            return apology("must provide username", 403)

        # Ensure password was submitted
        password = request.form.get("password")
        if not password:
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], password):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Flash user
        flash(f"Welcome {rows[0]['username']}!")

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Flash user
    flash('You have successfully been logged out.')

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    quoted = None
    # When user searches for a quote
    if request.method == "POST":
        # Get Stock info from API through lookup helper function
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        # If Company does not exist or wrong format was used.
        if quoted == None:
            flash("Symbol does not exist. Try Again.")

    return render_template("quote.html", quoted=quoted)



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        # Check if username was submitted
        username = request.form.get("username")
        if not username:
            return apology("Username not Provided.")

        # Check if password was submitted
        password = request.form.get("password")
        if not password:
            return apology("Password not Provided.")

        # Check if password was confirmed
        confirmation = request.form.get("confirmation")
        if not confirmation:
            return apology("Please Confirm Password.")

        # Check if passwords match
        if password != confirmation:
            return apology("Passwords Do Not Match.")

        # Check if username doesn't already exist
        rows = db.execute("SELECT username FROM users WHERE username= :username", username=username)
        if len(rows) != 0:
            return apology("Username Already Exists.")

        # Add user into SQL database after hashing password
        hashed = generate_password_hash(password, "sha256")
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hashed)", username=username, hashed=hashed)

        # Flash user
        flash("You have successfully been registered.\nLogin below.")

        # Redirect user to login page
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        return render_template("sell.html")

    else:
        # Get user id
        user_id = session["user_id"]

        # Check if symbol if submitted and it exists
        symbol = request.form.get("symbol")
        quoted = lookup(symbol)
        if not quoted:
            return apology("Symbol not Recognized")

        # Get timestamp as soon as possible
        selling_time = time()

        # Check if no of shares is submitted and it is a positive integer
        shares = request.form.get("shares")
        if not shares.isdigit() or int(shares) <= 0:
            return apology("Enter a Positive Integer")

        # Check if the user has that stock or has as many shares as the user has inputted
        rows = db.execute("SELECT * FROM transactions WHERE (user_id= :user_id AND symbol= :symbol)", user_id=user_id, symbol=quoted["symbol"])

        net_shares = 0   # Alternative: Make column witch keeps tabs on total shares of particular symbol
        for row in rows:
            net_shares += row["shares"]
        if net_shares < int(shares):
            return apology("You do not (have/have enough of) the stock")

        # Sell
        # Get current cash reserve
        rows = db.execute("SELECT cash FROM users WHERE id= :id", id=user_id)
        cash = rows[0]["cash"]

        # Update cash reserve
        cash += int(shares)*quoted["price"]
        db.execute("UPDATE users SET cash= :cash WHERE id= :id", cash=cash, id=user_id)

        # Log Transaction
        db.execute("INSERT INTO transactions (symbol, shares, action, price, time, user_id) VALUES (:symbol, :shares, :action, :price, :time, :user_id)", symbol=quoted["symbol"], shares=-int(shares), action='sell', price=quoted["price"], time=selling_time, user_id=user_id)

        flash("Shares successfully sold!")

        return redirect("/")

@app.route("/wallet", methods=["GET", "POST"])
@login_required
def wallet():
    if request.method == "GET":
        return render_template("wallet.html")
    else:
        # Check if cash inputted is a positive real number.
        cash = request.form.get("cash")
        if not cash.isdigit() or float(cash) <= 0:
            flash("Please Enter a positive number")
            return redirect("/wallet")

        # Add cash to SQL
        db.execute("UPDATE users SET cash= (cash + :cash) WHERE id= :id", cash=float(cash), id=session["user_id"])

        return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors  ??????
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
