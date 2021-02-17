from cs50 import SQL
from flask import Flask, render_template, request, redirect

app = Flask(__name__)

db = SQL("sqlite:///lecture.db")

@app.route("/")
def index():
    rows = db.execute("SELECT * FROM registrants")
    return render_template("index.html", rows=rows)

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        name = request.form.get("name")
        if not name:
            return render_template("sorry.html", message="a name")
        email = request.form.get("email")
        if not email:
            return render_template("sorry.html", message="an email adress")
        db.execute("INSERT INTO registrants (name, email) VALUES (?, ?)", name, email)
        return redirect("/")