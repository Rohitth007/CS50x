from sys import argv, exit
from cs50 import SQL

if len(argv) != 2:
    print("Usage: python roster.py house")
    exit(1)

db = SQL("sqlite:///students.db")  # For connecting with Sqlite3

roster = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last ,first", argv[1])
# *****executing a SELECT SQL returns a "list of rows" with each "row being a dictionary with keys as column names".***** 

for student in roster:
    if student["middle"] == None:  # If no middle name (NULL), it returns a None Type
        print(student["first"]+" "+student["last"]+" , born "+str(student["birth"]))
    else:
        print(student["first"]+" "+student["middle"]+" "+student["last"]+" , born "+str(student["birth"]))