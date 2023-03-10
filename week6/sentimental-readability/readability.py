from cs50 import get_string


def is_end_sentence(letter):
    return letter == "?" or letter == "!" or letter == "."


def calc_grade(letters, words, sentences):
    L = float(letters / words * 100)
    S = float(sentences / words * 100)
    index = 0.0588 * L - 0.296 * S - 15.8

    index = round(index)
    return int(index)


text = get_string("Text: ")

letters = 0
words = 1
sentences = 0


for letter in text:
    if letter.isalpha():
        letters += 1
    elif letter == " ":
        words += 1
    elif is_end_sentence(letter):
        sentences += 1

grade = calc_grade(letters, words, sentences)

if grade < 1:
    print("Before Grade 1")
elif grade > 15:
    print("Grade 16+")
else:
    print("Grade", grade)

