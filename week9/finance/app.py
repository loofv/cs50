import os
import sys
import datetime

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

STOCKTRANSACTION_BOUGHT = 'buy'
STOCKTRANSACTION_SOLD = 'sold'
# Hack to use print with flask for debugging
# print("------------------------ENUFF CASH ###: ", file=sys.stderr)


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    user_id = session["user_id"]
    stocks = db.execute("SELECT * FROM shareholders WHERE userid = ?", user_id)
    cashlist = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
    cash = cashlist[0]['cash']
    grandtotal = cash
    # print("------------------------cash ###: ", cash, file=sys.stderr)
    stocklist = []
    for stock in stocks:
        symbol = stock['symbol']
        data = lookup(symbol)
        price = str(data['price']).strip("$")
        price = float(price)
        amount = int(stock['amount'])

        total = 0
        total += amount * price
        grandtotal += total
        # print("------------------------stonk TOTAL ###: ", total, file=sys.stderr)
        stockitem = {
            'symbol': symbol,
            'price': usd(price),
            'amount': amount,
            'totalValue': usd(total)
        }
        stocklist.append(stockitem)

    cash = usd(cash)
    grandtotal = usd(grandtotal)
    return render_template("list.html", stocks=stocklist, cash=cash, grandtotal=grandtotal)


@app.route("/loan", methods=["GET", "POST"])
@login_required
def loan():
    if request.method == "POST":
        user_id = session["user_id"]
        loan = float(request.form.get("loan"))
        if loan <= 0:
            return apology("That would make for a very strange loan. Try again.")
        cashlist = db.execute("SELECT cash FROM users WHERE id = ?", user_id)
        cash = cashlist[0]['cash']
        if cash == 0:
            return apology("No bank will give you a loan if you're broke ;(")
        new_balance = loan + cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", new_balance, user_id)
        return redirect("/")
    else:
        return render_template("loan.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    if request.method == "POST":
        # print("------------------------Post req to buy ###: ", file=sys.stderr)
        symbol = request.form.get("symbol")
        shares = 0
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("Shares needs to be filled with whole values")
        if not symbol or shares <= 0:
            return apology("Symbol and shares both need to be filled")

        data = lookup(symbol)
        if not data:
            return apology("Could not find stock", 400)

        price = str(data['price']).strip("$")
        price = float(price)
        print("------------------------Post DATA PRICE ###: ", price, file=sys.stderr)

        total = price * shares
        user_id = session["user_id"]
        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)
        available_funds = rows[0]['cash']

        if available_funds < total:
            return apology("Insufficient funds")
        else:
            now = datetime.datetime.now()
            s1 = now.strftime("%Y-%m-%d %H:%M")
            new_balance = available_funds - total
            db.execute("UPDATE users SET cash = ? WHERE users.id = ?", new_balance, user_id)
            db.execute("INSERT INTO history (userid, stocktransaction, symbol, amount, price, datetime) VALUES (?, ?, ?, ?, ?, ?);",
                       user_id, STOCKTRANSACTION_BOUGHT, symbol, shares, price, s1)

            rows = db.execute("SELECT * FROM shareholders WHERE userid = ? AND symbol = ?", user_id, symbol)
            if rows:
                old_amount = rows[0]['amount']
                new_amount = old_amount + shares
                db.execute("UPDATE shareholders SET amount = ? WHERE userid = ?", new_amount, user_id)
            else:
                db.execute("INSERT INTO shareholders (userid, symbol, amount, price) VALUES (?, ?, ?, ?);",
                           user_id, symbol, shares, price)

        return redirect("/")

    else:
        # userid, stocktransaction, symbol, amount, price, datetime
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    stocklist = []
    user_id = session["user_id"]
    rows = db.execute("SELECT * FROM history WHERE userid = ?", user_id)
    for row in rows:
        stockitem = {
            'transactiontype': row['stocktransaction'],
            'symbol': row['symbol'],
            'price': usd(row['price']),
            'amount': row['amount'],
            'datetime': row['datetime'],
        }

        print("------------------------date time ###: ", row['datetime'], file=sys.stderr)
        stocklist.append(stockitem)

    return render_template("history.html", stocklist=stocklist)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":
        symbol = request.form.get("symbol")
        quote = lookup(symbol.upper())
        if not quote:
            return apology("Could not find stock", 400)

        return render_template("quoted.html", name=quote['name'], price=quote['price'], symbol=symbol)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

            # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT username FROM users WHERE username = ?", username)
        for row in rows:
            if row["username"] == username or username == '':
                return apology("Username already exists", 400)

        if password == '' or password != confirmation:
            return apology("Password either blank or not matching", 400)

        # DB stuff
        hash = generate_password_hash(password)

        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, hash)
        # Redirect user to login page
        return redirect("/login")

    else:
        # Redirect user to register page
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        user_id = session["user_id"]
        rows = db.execute("SELECT * FROM shareholders WHERE userid = ?", user_id)
        if not rows:
            return apology("You don't appear to own that stock")

        old_amount = rows[0]['amount']
        if shares > old_amount:
            return apology("You don't have that many stocks to sell")

        if not symbol or shares <= 0:
            return apology("Symbol and shares both need to be filled")

        data = lookup(symbol)
        if not data:
            return apology("Could not find that stock")

        price = str(data['price']).strip("$")
        price = float(price)
        total = price * shares
        rows = db.execute("SELECT * FROM users WHERE id = ?", user_id)
        available_funds = rows[0]['cash']

        now = datetime.datetime.now()
        s1 = now.strftime("%Y-%m-%d %H:%M")
        new_balance = available_funds + total
        db.execute("UPDATE users SET cash = ? WHERE users.id = ?", new_balance, user_id)
        db.execute("INSERT INTO history (userid, stocktransaction, symbol, amount, price, datetime) VALUES (?, ?, ?, ?, ?, ?);",
                   user_id, STOCKTRANSACTION_SOLD, symbol, shares, price, s1)

        new_amount = old_amount - shares

        # update or delete?
        if new_amount == 0:
            db.execute("DELETE FROM shareholders WHERE userid = ? AND symbol = ?", user_id, symbol)
        else:
            db.execute("UPDATE shareholders SET amount = ? WHERE userid = ?", new_amount, user_id)

        return redirect("/")

    else:
        user_id = session["user_id"]
        stocklist = db.execute("SELECT symbol FROM shareholders WHERE userid = ?", user_id)

        return render_template("sell.html", stocklist=stocklist)
