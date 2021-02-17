from sys import argv, exit
from cs50 import SQL
import csv

if len(argv) != 2:
    print("Usage: python import.py characters.csv")
    exit(1)

db = SQL("sqlite:///students.db")  # Connecting with SQLite3

with open(argv[1], 'r') as data:  # Opening CSV file
    reader = csv.DictReader(data)

    for row in reader:
        # divide name into first middle n last
        name_arr = ["", "", ""]
        space = 0
        for char in row["name"]:
            if char == ' ':
                space += 1
            elif space == 0:
                name_arr[0] += char
            elif space == 1:
                name_arr[1] += char
            elif space == 2:
                name_arr[2] += char

        # Insert into students.db database
        if space == 2:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", 
                       name_arr[0], name_arr[1], name_arr[2], row["house"], row["birth"])
        else:
            db.execute("INSERT INTO students (first, last, house, birth) VALUES (?, ?, ?, ?)", 
                       name_arr[0], name_arr[1], row["house"], row["birth"])
