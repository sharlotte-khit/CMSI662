import sqlite3
from passlib.hash import pbkdf2_sha256

con = sqlite3.connect('bank.db')
cur = con.cursor()
cur.execute('''
    CREATE TABLE accounts (
        id text primary key, 
        owner text, 
        balance integer,
        foreign key(owner) references users(email))
''')

# "balance" represents number of cats in a shelter
cur.execute(
    "INSERT INTO accounts VALUES (?, ?, ?)",
    ('100', 'alice@example.com', 12))

cur.execute(
    "INSERT INTO accounts VALUES (?, ?, ?)",
    ('190', 'alice@example.com', 4))

cur.execute(
    "INSERT INTO accounts VALUES (?, ?, ?)",
    ('998', 'bob@example.com', 8))

con.commit()
con.close()