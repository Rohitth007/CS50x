#include<stdio.h>
#include<cs50.h>

int arg_check(int argc, string argv[]);

int main(int argc, string argv[])
{
    int error = arg_check(argc, argv);
    if (error != 0)
    {
        return error;
    }

    string key = argv[1];

    string plain_text = get_string("plaintext: ");


    for (int i = 0; plain_text[i] != '\0'; i++)
    {
        if (plain_text[i] > 64 && plain_text[i] < 91)         // If plain_text's letter considered is Capital
        {
            if (key[plain_text[i] - 65 ] < 91)                // If key's letter considered is also Capital
            {
                plain_text[i] = key[plain_text[i] - 65];      // Then Substitute
            }
            else                                              // But if key's letter considered is lowercase
            {
                plain_text[i] = key[plain_text[i] - 65] - 32; // Then Subtract 32 from ASCII value and Substitute
            }
        }
        else if (plain_text[i] > 96 && plain_text[i] < 123)    // Or if plain_text's letter considered is lowercase
        {
            if (key[plain_text[i] - 97 ] > 96)                // If key's letter considered is also lowercase
            {
                plain_text[i] = key[plain_text[i] - 97];      // Then Substitute
            }
            else                                              // But if key's letter considered is Capital
            {
                plain_text[i] = key[plain_text[i] - 97] + 32; // Then Add 32 to ASCII value and Substitute
            }
        }
    }
    printf("ciphertext: %s\n", plain_text);

}

int arg_check(int argc, string argv[])
{
    if (argc == 1)   // If no Key is entered
    {
        printf("ERROR_34: Key Argument not entered.\n\t./substitution <key>\n");
        return 1;
    }
    else if (argc > 2)  // If there are more than 2 arguments
    {
        printf("ERROR_71: Entered more than one argument.\n");
        return 1;
    }
    else
    {
        int i;
        string key = argv[1];
        for (i = 0; key[i] != '\0'; i++)   
        {
            if (key[i] < 65 || (key[i] > 90 && key[i] < 97) || key[i] > 122)      // Checking if key contains only alphabets.
            {
                printf("ERROR_57: Key contains non-alphabet characters.\n");
                return 1;
            }
            for (int j = i + 1; key[j] != '\0'; j++)      // Checking for Repeatition of Letters.
            {    
                if (key[j] == key[i])
                {
                    printf("ERROR_86: Key contains Repeatition of Letters.\n");
                    return 1;
                }
            }
        }
        if (i != 26)    // Checking if Key has 26 Characters.
        {
            printf("ERROR_22: Key should contain 26 Letter.\n");
            return 1;
        }
    }
    return 0;
}
