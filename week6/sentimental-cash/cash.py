from cs50 import get_float


# Convert from 0.41 to 41 cents
# * 100

def calculate_quarters(cents):
    return int(cents / 25)


def calculate_dimes(cents):
    return int(cents / 10)


def calculate_nickels(cents):
    return int(cents / 5)


def calculate_pennies(cents):
    return int(cents / 1)


while True:
    cents = get_float("Change owed: ")
    if cents > 0.0:
        break
cents = cents * 100.0


quarters = calculate_quarters(cents)
cents = cents - (quarters * 25)


dimes = calculate_dimes(cents)
cents = cents - (dimes * 10)

nickels = calculate_nickels(cents)
cents = cents - (nickels * 5)

pennies = calculate_pennies(cents)
cents = cents - pennies

coins = quarters + dimes + nickels + pennies
print("coins: ", coins)

