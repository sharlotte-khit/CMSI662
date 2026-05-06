import os
from flask import Flask, request, make_response, redirect, render_template, g, abort
from services.user_service import get_user_with_credentials, logged_in
from services.account import get_balance, do_transfer, get_accounts
from flask_wtf.csrf import CSRFProtect
from dotenv import load_dotenv

load_dotenv()

app = Flask(__name__)
app.config['SECRET_KEY'] = os.getenv("SECRET_KEY")
if not app.config['SECRET_KEY']:
    raise RuntimeError("SECRET_KEY is missing from the .env file")

csrf = CSRFProtect(app)

@app.route("/", methods=['GET'])
def home():
    if not logged_in():
        return render_template("login.html")
    return redirect('/dashboard')

@app.route("/login", methods=["POST"])
def login():
    email = request.form.get("email")
    password = request.form.get("password")

    user = get_user_with_credentials(email, password)

    # Generic login error helps prevent user enumeration.
    # It does not reveal whether the email or password was wrong.
    if not user:
        return render_template("login.html", error="Invalid credentials"), 401

    response = make_response(redirect("/dashboard"))

    # httponly helps prevent JavaScript from stealing the JWT if XSS occurs.
    # samesite helps reduce CSRF risk.
    # secure should be True in production with HTTPS.
    response.set_cookie(
        "auth_token",
        user["token"],
        httponly=True,
        samesite="Lax",
        secure=False
    )

    return response, 303

@app.route("/logout", methods=['GET'])
def logout():
    response = make_response(redirect("/dashboard"))
    response.delete_cookie('auth_token')
    return response, 303

@app.route("/dashboard", methods=['GET'])
def dashboard():
    if not logged_in():
        return render_template("login.html")

    # Only get accounts owned by the logged-in user.
    # This prevents Bob from seeing Alice's catteries on the dashboard.
    accounts = get_accounts(g.user)

    return render_template(
        "dashboard.html",
        email=g.user,
        accounts=accounts
    )

@app.route("/details", methods=['GET'])
def details():
    if not logged_in():
        return render_template("login.html")

    account_number = request.args['account']

    # get_balance checks both account id AND owner.
    # This prevents users from viewing catteries they do not own.
    balance = get_balance(account_number, g.user)

    if balance is None:
        abort(404, "Cattery not found")

    return render_template(
        "details.html",
        user=g.user,
        account_number=account_number,
        balance=balance
    )

@app.route("/transfer", methods=["GET"])
def transfer_page():
    if not logged_in():
        return render_template("login.html")
    return render_template("transfer.html")

@app.route("/transfer", methods=["POST"])
def transfer():
    if not logged_in():
        return render_template("login.html")

    source = request.form.get("from")
    target = request.form.get("to")

    try:
        amount = int(request.form.get("amount"))
    except (TypeError, ValueError):
        return render_template(
            "transfer.html",
            error="Invalid amount. Please enter a whole number.",
            source=source
        ), 400

    # Validate transfer amount.
    if amount <= 0:
        return render_template(
            "transfer.html",
            error="Amount must be positive.",
            source=source
        ), 400

    if amount > 20:
        return render_template(
            "transfer.html",
            error="You can only rehome up to 20 cats at a time.",
            source=source
        ), 400

    # Authorization check.
    # User can only transfer cats FROM a cattery they own.
    available_balance = get_balance(source, g.user)

    if available_balance is None:
        return render_template(
            "transfer.html",
            error="Cattery not found.",
            source=source
        ), 404

    if amount > available_balance:
        return render_template(
            "transfer.html",
            error="You don't have that many cats.",
            source=source
        ), 400

    # Complete transfer functionality.
    if not do_transfer(source, target, amount):
        return render_template(
            "transfer.html",
            error="Transfer failed. Please check the destination cattery.",
            source=source
        ), 400

    return render_template(
        "transfer.html",
        success=f"Successfully rehomed {amount} cats!",
        source=source
    )

if __name__ == "__main__":
    app.run(debug=True)