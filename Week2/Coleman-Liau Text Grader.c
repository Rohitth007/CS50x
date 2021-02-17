// Coleman-Liau Text Grader

// Assuming no sentences has tabs or double scaces.
// Assuming things like Mr.X as a sentence as there is a fullstop.
// Assuming hypened words as a single word.
#include<stdio.h>
#include<cs50.h>
#include<math.h>

int main(void)
{
    int count_letters = 0;
    int count_words = 0;
    int count_sent = 0;
    float L, S;
    int index;

    string text = get_string("Text: ");

    for (int i = 0; text[i] != '\0'; i++)
    {
        if ((text[i] > 64 && text[i] < 91) || (text[i] > 96 && text[i] < 123))  // Using ASCII Table  for selecting only alphabets
        {
            count_letters++;
        }
        else if (text[i] == ' ')  // 'Assuming-this-as-one-word' (Words only end with spaces)
        {
            count_words++;
        }
        else if (text[i] == '.' || text[i] == '!' || text[i] == '?')   // Assuming all sentences end with either . , ! or ?
        {
            count_sent++;
        }
    }
    count_words++;   // There is one extra word because the last word doesn't have a space after it.

    printf("%d letter(s)\n%d word(s)\n%d sentence(s)\n", count_letters, count_words, count_sent);

    L = (count_letters * 100.0) / count_words;
    S = (count_sent * 100.0) / count_words;
    index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", index);
    }
}
