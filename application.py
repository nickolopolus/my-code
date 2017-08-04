from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/", methods=["GET"])
@login_required
def index():
    
    #used to hold total account value and overall gain/loss
    total_value = 0
    total_fluct = 0
    
    #get rows from ownership table for user and user cash balance
    ownership_rows = get_ownership_rows_formatted(session["user_id"])        
    user_cash = get_user_cash(session["user_id"])
    
    #total value starts with user cash balance
    total_value += user_cash
    user_cash = usd(user_cash)
        
    #iterate through ownership rows, adding current value and gain or loss to total value and total fluct
    for row in ownership_rows:
        total_value += float(row["current_value"].replace("$", "").replace(",",""))
        total_fluct += float(row["gain_loss"].replace("$", "").replace(",",""))
        
    total_value = usd(total_value)
    total_fluct = usd(total_fluct)
        
    return render_template("index.html", rows=ownership_rows, balance=user_cash, value=total_value, gainloss=total_fluct)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    
    #if user reached route via GET (as by clicking a link or via redirect)
    if request.method == "GET":
        return render_template("buy.html")
        
    #if user reached route via post (as by submitting a purchase request from the buy page)
    else:

    	#extract symbol and quantity from request form
        symbol = request.form.get("stock symbol")
        quantity = request.form.get("quantity")
        
        #if any fields were left blank, render apology
        if symbol == "" or quantity == "" or float(quantity) <= 0:
            return apology("Must enter stock symbol and a positive quantity")

        quantity = float(request.form.get("quantity"))
            
        #retrieve stock data
        stock_data = lookup(symbol)
        
        #verify stock data retrieved as expected (as dict)
        if stock_data is None:
            return apology("Error retrieving stock data. Verify symbol is correct.")

        stock_price = stock_data.get("price", 0.0)
        symbol = stock_data.get("symbol")
            
        #get user's account balance
        user_cash = get_user_cash(session["user_id"])
        cost = stock_price * quantity
        
        #if funds are sufficient deduct purchase amount, update transaction history, ownership table
        if cost <= user_cash:
            
            #update account balance
            new_balance = user_cash - cost
            update_user_cash(new_balance, session["user_id"])
            
            #add row to transaction history
            add_transaction(session["user_id"], 1, symbol, quantity, stock_price, cost)
                
            #get current amount of stock owned by user
            owned_shares = get_quantity_owned(session["user_id"], symbol, stock_price)
            
            #if none owned at current price, insert row 
            if owned_shares == 0:
            	add_new_ownership(session["user_id"], symbol, quantity, stock_price, (quantity * stock_price))
            
            #else update quantity owned and starting value 
            else:
            	update_ownership_buy(session["user_id"], symbol, stock_price, quantity)

            return redirect(url_for('index'))
            
        else:
            return apology("Insufficient funds to complete transaction. Available funds: " + str(usd(rows[0]["cash"])))
            
            

@app.route("/history", methods=["GET"])
@login_required
def history():
    return render_template("history.html", transactions=get_transaction_history(session["user_id"]))

@app.route("/login", methods=["GET", "POST"])
def login():

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))
    
@app.route("/password_reset", methods=["GET", "POST"])
@login_required
def password_reset():
    
    # if user reached route via GET (user clicked a link to get to page), register template
    if request.method == "GET":
        return render_template("password_reset.html")
        
    else:
        current_password = request.form.get("current password")
        new_password = request.form.get("new password")
        retyped_password = request.form.get("retyped password")
        
        if current_password == "" or new_password == "" or retyped_password == "":
            return apology("Must enter all 3 password fields")
            
        if new_password != retyped_password:
            return apology("Passwords do not match")
            
        if current_password == new_password:
            return apology("Must not reuse password")
            
        if verify_password(session["user_id"], current_password):
            update_user_password(session["user_id"], pwd_context.hash(new_password))
            return redirect(url_for("index"))
        else:
            return apology("Invalid password. Try again.")
            
        


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    
    # if user reached route via GET (user clicked a link to get to page), register template
    if request.method == "GET":
        return render_template("quote.html")
        
    # if user reached route via POST (as by submitting a form via POST)
    elif request.method == "POST":
        
        #if any fields were left blank, render apology
        if request.form.get("stock symbol") == "" or request.form.get("quantity") == "":
            return apology("Must enter stock symbol and quantity")
        
        #retrieve stock data
        stock_data = lookup(request.form.get("stock symbol"))
        
        #verify stock data retrieved as expected (as dict)
        if stock_data is None:
            return apology("Error retrieving stock data. Verify symbol is correct.")
        
        #redirect to quoted template, passing name, price, symbol quantity, and total along
        return render_template("quoted.html", 
                name=stock_data.get("name", "n/a"), 
                price=usd(stock_data.get("price", "0.0")), 
                symbol=stock_data.get("symbol", "n/a"),
                quantity=request.form.get("quantity"), 
                total=(usd(float(request.form.get("quantity")) * stock_data.get("price", 0.0))))

@app.route("/register", methods=["GET", "POST"])
def register():
    
    # if user reached route via GET (user clicked a link to get to page), register template
    if request.method == "GET":
        return render_template("register.html")
        
    # if user reached route via POST (as by submitting a form via POST)
    elif request.method == "POST":
        
        #if any fields were left blank, render apology
        if request.form.get("username") == "" or request.form.get("password") == "" or request.form.get("retype-password") == "":
            return apology("Must enter a username and password.")
            
        #if passwords don't match, render apology
        elif request.form.get("password") != request.form.get("retype-password"):
            return apology("Passwords do not match.")
            
        #else, form was filled out and passwords matched
        else:
            
            #check if username exists
            if username_exists(request.form.get("username")):
                return apology("Sorry, username is already taken.")
                
            #if username doesnt already exist, register user
            else:
                add_user(request.form.get("username"), request.form.get("password"))
                return redirect(url_for("index"))

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    
    #if route reached via post (submitted request to sell shares)
    if request.method == "POST":
        
        if float(request.form.get("quantity")) < 1:
            return apology("Quantity must be greater than 0")

        symbol = request.form.get("symbol")
        purchase_price = request.form.get("purchase price")
        quantity_to_sell = int(request.form.get("quantity"))

        shares_owned = get_quantity_owned(session["user_id"], symbol, purchase_price)
         
        #if user does not own that combination of symbol and starting price
        if shares_owned == 0:
            return apology("You do not own " + request.form.get("symbol") + " at the purchase price of $" + str(request.form.get("purchase price")))
            
        #if user owns less than requested number of shares to be sold
        if shares_owned < quantity_to_sell:
            return apology("You own " + str(shares_owned) + " of " + request.form.get("symbol") + "." )
        
        #look up current stock data
        stock_data = lookup(symbol)
        
        #get current cash balance for user
        current_cash = get_user_cash(session["user_id"])
        
        #determine cash value of sale
        sale_amount = stock_data.get("price") * quantity_to_sell
        
        #if user is selling all shares they own at given starting price, delete the row from ownership
        if shares_owned == quantity_to_sell:
            remove_ownership(session["user_id"], symbol, purchase_price)
        
        #else update the ownership row to reflect change
        else:
            update_ownership_sell(session["user_id"], symbol, purchase_price, quantity_to_sell)
            
        #add transaction to history table
        add_transaction(session["user_id"], 0, symbol, quantity_to_sell, purchase_price, (quantity_to_sell * purchase_price))

        #add cash to user account
        update_user_cash(current_cash + sale_amount, session["user_id"])
            
        return redirect(url_for("index"))
        
    #else route reached via get, render sell.html
    else:
        ownership_rows = get_ownership_rows_formatted(session["user_id"])
        
        #lists for options dropdown
        symbols = []
        purchase_prices = []
        
        for item in ownership_rows:
            symbols.append(item["symbol"])
            purchase_prices.append(item["purchase_price"].strip("$"))
            
        return render_template("sell.html", rows=ownership_rows, symbols=symbols, prices=purchase_prices)

#############################################################
#
#functions to handle db operations
#
#############################################################

#returns rows from ownership table along with additional columns, primarily for index.html
def get_ownership_rows_formatted(user_id):
    
    ownership_rows = db.execute("SELECT * FROM ownership WHERE user_id = :uid", uid=user_id)
        
    if len(ownership_rows) > 0:
        
        for row in ownership_rows:
            stock_data = lookup(row["symbol"])
            if stock_data != None:
                row["name"] = stock_data.get("name")
                row["current_price"] = stock_data.get("price")
                row["current_value"] = stock_data.get("price") * float(row["quantity"])
                row["gain_loss"] = row["current_value"] - float(row["starting_value"])
                row["current_price"] = usd(row["current_price"])
                row["current_value"] = usd(row["current_value"])
                row["gain_loss"] = usd(row["gain_loss"])
                row["starting_value"] = usd(row["starting_value"])
                row["purchase_price"] = usd(row["purchase_price"])
                    
        return sorted(ownership_rows, key=lambda k: k['symbol'])
    
    return []

#returns user account balance
def get_user_cash(user_id):
	return db.execute("SELECT cash FROM users WHERE id = :uid", uid=user_id)[0]["cash"]

#chances user account balance
def update_user_cash(amount, user_id):
	return db.execute(("UPDATE users SET cash = :cash WHERE id = :user_id"), cash=amount, user_id=user_id)

#adds new row to history table
def add_transaction(user_id, buy, symbol, quantity, price, total):
	return db.execute("INSERT INTO history (user_id, buy, symbol, quantity, price, total) VALUES (:uid, :buy_bool, :symb, :qty, :prce, :totl)", 
		uid=user_id, buy_bool=buy, symb=symbol, qty=quantity, prce=price, totl=total)

#returns quantity of certain stock owned by user
def get_quantity_owned(user_id, symbol, price):

	row = db.execute("SELECT quantity FROM ownership WHERE user_id = :uid AND symbol = :symb AND purchase_price = :p",
                uid=user_id, symb=symbol, p=price)

	if len(row) == 0:
		return 0

	return row[0]["quantity"]

#adds new stock ownership to ownership table (when stock at given price is not currently owned)
def add_new_ownership(user_id, symbol, quantity, price, value):
	return db.execute("INSERT INTO ownership (user_id, symbol, quantity, purchase_price, starting_value) VALUES(:uid, :symb, :qty, :purchase_price, :starting_value)", 
		uid=user_id, symb=symbol, qty=quantity, purchase_price=price, starting_value=value)

#updates ownership table with new stock purchased (when some quantity at given price already owned)
def update_ownership_buy(user_id, symbol, price, quantity):

	current_row = db.execute("SELECT quantity, starting_value FROM ownership WHERE user_id=:uid, symbol=:symb, purchase_price=:p", uid=user_id, symb=symbol, p=price)

	if len(current_row) == 1:
		new_quantity = current_row[0]["quantity"] + quantity
		new_starting_value = new_quantity * price

		return db.execute("UPDATE ownership SET quantity = :qty, starting_value = :starting_value WHERE user_id = :uid AND symbol = :symb AND purchase_price = :p", 
			qty=new_quantity, uid=user_id, symb=symbol, starting_value=new_starting_value, p=price)
	
	return None

#updates ownership table with new stock details after sale 
def update_ownership_sell(user_id, symbol, price, quantity):

	current_holding = db.execute("SELECT * FROM ownership WHERE user_id = :uid AND symbol = :symb AND purchase_price = :p", 
		uid = user_id, symb = symbol, p = price)

	if current_holding[0]["quantity"] == quantity:
	    remove_ownership(user_id, symbol, price)
	
	elif current_holding[0]["quantity"] < quantity:
		new_quantity = current_holding[0]["quantity"] - quantity
		new_starting_value = new_quantity * price

		return db.execute("UPDATE ownership SET quantity = :new_qty, starting_value = :new_starting_val WHERE user_id = :uid AND symbol = :symb AND purchase_price = :p", 
			uid = user_id, new_qty = new_quantity, new_starting_val = new_starting_value)

	return None

#returns formatted rows (in list of dicts) from history table from user
def get_transaction_history(user_id):
    
    transactions = db.execute("SELECT * FROM history WHERE user_id = :uid", uid=user_id)
    
    if len(transactions) > 0:
        for row in transactions:
            row["price"] = usd(row["price"])
            row["buy"] = "BUY" if row["buy"] else "SELL"
    
        return sorted(transactions, key=lambda k: k['date_time'])
        
    return transactions

#returns true if username exists
def username_exists(username):
	return len(db.execute("SELECT * FROM users WHERE username = :un", un=username)) == 1

#adds new record to users table
def add_user(username, password):
	return db.execute("INSERT INTO users (username, hash) values(:un, :hash)", un=username, 
                hash=pwd_context.hash(password))
                
def verify_password(user_id, password):
    row = db.execute("SELECT hash FROM users WHERE id = :uid", uid = user_id)
    return len(row) == 1 and pwd_context.verify(password, row[0]["hash"])

def update_user_password(user_id, hashed):
    return db.execute("UPDATE users SET hash = :h WHERE id = :uid", h=hashed, uid=user_id)

#removes a row from ownership table (for use when a user sells all of a stock)
def remove_ownership(user_id, symbol, price):
	return db.execute("DELETE FROM ownership WHERE user_id = :uid AND symbol = :symb AND purchase_price = :p", uid = user_id, symb = symbol, p = price)