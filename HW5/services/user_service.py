import sqlite3, os, jwt
from datetime import datetime, timedelta, timezone
from passlib.hash import pbkdf2_sha256
from flask import request, g
from dotenv import load_dotenv

load_dotenv()
SECRET = os.getenv("JWT_SECRET")

if not SECRET:
    raise RuntimeError("SECRET_KEY is missing from the .env file")

def get_user_with_credentials(email, password):
    try:
        con = sqlite3.connect('bank.db')
        cur = con.cursor()

        # Parameterized query prevents SQL injection.
        cur.execute(
            '''
            SELECT email, name, password
            FROM users
            WHERE email=?
            ''',
            (email,)
        )

        row = cur.fetchone()

        # User enumeration defense:
        # If the user does not exist, still perform a dummy PBKDF2 verification.
        # This makes timing differences harder to use for account discovery.
        if row:
            email, name, password_hash = row
            valid = pbkdf2_sha256.verify(password, password_hash)
        else:
            pbkdf2_sha256.verify(
                password,
                "$pbkdf2-sha256$29000$dummy_salt$dummy"
            )
            valid = False

        if not valid:
            return None

        return {
            "email": email,
            "name": name,
            "token": create_token(email)
        }

    finally:
        con.close()


def logged_in():
    token = request.cookies.get('auth_token')

    try:
        # JWT signature verification prevents forged authentication tokens.
        data = jwt.decode(token, SECRET, algorithms=['HS256'])

        g.user = data['sub']

        return True

    except jwt.InvalidTokenError:
        return False


def create_token(email):
    now = datetime.now(timezone.utc)

    payload = {
        'sub': email,
        'iat': now,
        'exp': now + timedelta(minutes=60)
    }

    token = jwt.encode(payload, SECRET, algorithm='HS256')

    return token