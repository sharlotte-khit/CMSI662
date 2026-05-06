import sqlite3

MAX_TRANSFER = 1000

def get_accounts(owner):
    try:
        con = sqlite3.connect('bank.db')
        cur = con.cursor()

        # Parameterized query prevents SQL injection.
        # Only returns accounts owned by the logged-in user.
        cur.execute(
            '''
            SELECT id, balance
            FROM accounts
            WHERE owner=?
            ''',
            (owner,)
        )

        return cur.fetchall()

    finally:
        con.close()


def get_balance(account_number, owner):
    try:
        con = sqlite3.connect('bank.db')
        cur = con.cursor()

        # Parameterized query prevents SQL injection.
        # The owner check enforces authorization.
        cur.execute(
            '''
            SELECT balance FROM accounts
            WHERE id=? AND owner=?
            ''',
            (account_number, owner)
        )

        row = cur.fetchone()

        if row is None:
            return None

        return row[0]

    finally:
        con.close()


def do_transfer(source, target, amount):
    try:
        con = sqlite3.connect('bank.db')
        cur = con.cursor()

        # Check whether the target cattery exists.
        cur.execute(
            '''
            SELECT id FROM accounts
            WHERE id=?
            ''',
            (target,)
        )

        row = cur.fetchone()

        if row is None:
            return False

        # Remove cats from source cattery.
        cur.execute(
            '''
            UPDATE accounts
            SET balance=balance-?
            WHERE id=?
            ''',
            (amount, source)
        )

        # Add cats to target cattery.
        cur.execute(
            '''
            UPDATE accounts
            SET balance=balance+?
            WHERE id=?
            ''',
            (amount, target)
        )

        con.commit()
        return True

    except sqlite3.Error:
        return False

    finally:
        con.close()