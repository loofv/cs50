from cs50 import get_int

while True:
    height = get_int("Height: ")
    if height > 0 and height < 9:
        break


height += 1
for i in range(1, height):
    spaces = height - i - 1
    for j in range(spaces):
        print(" ", end="")
    print("#" * i)
