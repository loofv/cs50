#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Helper method to improve readability and reduce complexity of count_letters
bool is_letter(char c)
{
    if (c >= 97 && c <= 122)
    {
        return true;
    }
    if (c >= 65 && c <= 90)
    {
        return true;
    }
    return false;
}

int count_letters(string text)
{
    int len = strlen(text);
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (is_letter(text[i]))
        {
            count++;
        }
    }
    return count;
}

int count_words(string text)
{
    int len = strlen(text);
    int count = 1;
    for (int i = 0; i < len; i++)
    {
        if (text[i] == 32)
        {
            count++;
        }
    }
    return count;
}

int count_sentences(string text)
{
    int len = strlen(text);
    int count = 0;
    for (int i = 0; i < len; i++)
    {
        if (text[i] == 46 || text[i] == 63 || text[i] == 33)
        {
            count++;
        }
    }
    return count;
}

int calc_grade(float letters, float words, float sentences)
{
    float L = letters / words * 100;
    float S = sentences / words * 100;
    float index = 0.0588 * L - 0.296 * S - 15.8;

    index = (int) round(index);

    return index;
}

int main(void)
{
    string text = get_string("Text: ");

    float letters = count_letters(text);
    float words = count_words(text);
    float sentences = count_sentences(text);

    int grade = calc_grade(letters, words, sentences);
    if (grade < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    if (grade >= 16)
    {
        printf("Grade 16+\n");
        return 0;
    }
    else
    {
        printf("Grade %i\n", grade);
    }
}
