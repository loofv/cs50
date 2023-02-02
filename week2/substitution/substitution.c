#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
char LETTERS_UPPER[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
char LETTERS_LOWER[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
const int LETTERSLENGTH = 26;
// Helper method to improve readability and reduce complexity
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

int get_index(char c)
{
    for (int i = 0; i < LETTERSLENGTH; i++)
    {
        if (LETTERS_UPPER[i] == toupper(c))
        {
            return i;
        }
    }
    return -1;
}


// Should contain all characters in alphabet and no duplicates
bool is_valid(string cipher)
{
    int curr_count;
    for (int i = 0; i < LETTERSLENGTH; i++)
    {
        char alph = LETTERS_UPPER[i];
        curr_count = 0;
        for (int j = 0; j < LETTERSLENGTH; j++)
        {
            char c = toupper(cipher[j]);
            if (c == alph)
            {
                curr_count++;
            }
        }
        if (curr_count != 1)
        {
            return false;
        }
    }
    return true;
}

int main(int argc, string argv[])
{
    string cipher;
    if (argv[1] != NULL)
    {
        cipher = argv[1];
    }
    else
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (strlen(cipher) == 0 || argv[2] != NULL)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    if (strlen(cipher) != 26 || !is_valid(cipher))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    for (int i = 0; i < LETTERSLENGTH; i++)
    {
        if (!is_letter(cipher[i]))
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
    }

    string plaintext;

    plaintext = get_string("plaintext: ");

    int len = strlen(plaintext);

    for (int i = 0; i < len; i++)
    {
        char c = plaintext[i];
        if (is_letter(c))
        {
            int index = get_index(c);
            if (isupper(c))
            {
                plaintext[i] = toupper(cipher[index]);
            }
            else
            {
                plaintext[i] = tolower(cipher[index]);
            }
        }
    }
    printf("ciphertext: %s\n", plaintext);
}
