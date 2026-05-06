# KittyVault

KittyVault is a secure Flask web application where users can safely transfer cats between shelters.

This project was originally based on a classroom banking application and expanded with additional web security protections, transfer functionality, validation, and custom styling.

The application demonstrates several important secure web development concepts including:

* PBKDF2 password hashing
* JWT authentication
* CSRF protection
* SQL Injection prevention
* XSS prevention
* User enumeration defense
* Secure validation and error handling

---

# Features

* Secure login system
* JWT authentication stored in cookies
* Cat transfer functionality between shelters
* Transfer validation
* Animated CSS styling
* Extensive inline security comments

---

# Security Protections

## Password Hashing

Passwords are securely stored using salted PBKDF2 hashes through Passlib.

Plaintext passwords are never stored in the database.

Example:

```python
pbkdf2_sha256.hash("123456")
```

During login:

```python
pbkdf2_sha256.verify(password, password_hash)
```

This helps protect user credentials even if the database is compromised.

---

## JWT Authentication

Authentication uses signed JSON Web Tokens (JWTs).

Example:

```python
jwt.encode(payload, SECRET, algorithm='HS256')
```

The JWT is stored inside an HTTP-only cookie.

Example:

```python
response.set_cookie(
    "auth_token",
    user["token"],
    httponly=True,
    samesite="Lax"
)
```

### Security Benefits

* `httponly=True`

  * Prevents JavaScript from reading the cookie.
  * Helps defend against XSS stealing authentication tokens.

* `samesite="Lax"`

  * Helps reduce CSRF attacks by limiting cross-site cookie sending.

---

## SQL Injection Prevention

All database queries use parameterized SQL statements.

Example:

```python
cur.execute(
    '''
    SELECT balance FROM accounts
    WHERE id=? AND owner=?
    ''',
    (account_number, owner)
)
```

This prevents attackers from injecting malicious SQL code into queries.

Unsafe code would look like:

```python
"SELECT * FROM users WHERE email='" + email + "'"
```

Parameterized queries separate SQL instructions from user data.

---

## Cross-Site Scripting (XSS) Prevention

Templates use Jinja templating.

Example:

```html
{{ user }}
```

Jinja automatically escapes HTML characters before rendering output.

If a user attempted to enter:

```html
<script>alert('XSS')</script>
```

it would render as harmless text instead of executing JavaScript.

This helps defend against reflected XSS attacks.

---

## CSRF Protection

The application uses Flask-WTF CSRF protection.

Example:

```python
csrf = CSRFProtect(app)
```

All POST forms include CSRF tokens:

```html
<input type="hidden" name="csrf_token" value="{{ csrf_token() }}" />
```

### Why This Matters

Without CSRF protection, attackers could trick authenticated users into submitting unintended requests from malicious websites.

CSRF tokens ensure requests originated from the legitimate application.

---

## User Enumeration Defense

The login system uses generic error messages:

```python
return render_template(
    "login.html",
    error="Invalid credentials"
)
```

The application never reveals whether:

* the email exists
* the password was wrong

This helps prevent attackers from discovering valid accounts.

Additionally, the application performs a dummy PBKDF2 verification even if the user does not exist:

```python
pbkdf2_sha256.verify(
    password,
    "$pbkdf2-sha256$29000$dummy_salt$dummy"
)
```

This helps reduce timing attacks.

---

## Validation and Error Handling

The transfer system validates:

* transfer amount
* account ownership
* available balance
* valid target account

Example:

```python
if amount <= 0:
    abort(400, "Amount must be positive")
```

The application also avoids exposing raw database or server errors to users.

Generic errors are safer because they leak less information to attackers.

---

# Running the Application

## Create Virtual Environment

```bash
python3 -m venv env
source env/bin/activate
```

---

## Install Dependencies

```bash
pip install -r requirements.txt
```

---

## Create Database

```bash
python createdb.py
python makeaccounts.py
```

---

## Run the Application

Using Flask:

```bash
export FLASK_APP=app.py
flask run
```

Or:

```bash
FLASK_APP=app.py flask run
```

---

## Open Browser

```text
http://127.0.0.1:5000
```

---

# Demo Accounts

## Alice

```text
alice@example.com
123456
```

## Bob

```text
bob@example.com
123456
```

---

# Project Structure

```text
HW5/
│
├── app.py
├── createdb.py
├── makeaccounts.py
├── bank.db
├── requirements.txt
├── README.md
│
├── services/
│   ├── account.py
│   └── user_service.py
│
├── templates/
│   ├── login.html
│   ├── dashboard.html
│   ├── details.html
│   └── transfer.html
│
└── static/
    └── style.css
```

---

# Technologies Used

* Python
* Flask
* SQLite
* Flask-WTF
* Passlib
* PyJWT
* HTML/CSS

---

# Assets / Credits

Background image pattern from Vecteezy:

Cat Seamless Pattern Vectors by Vecteezy  
https://www.vecteezy.com/free-vector/cat-seamless-pattern

---

# Disclaimer

This application was created for educational purposes as part of a secure web development assignment.

It demonstrates security concepts but is not intended for production deployment without additional hardening.

