// Credit Card Algorithm, Luhn's Algorithm 
// Basically we did sum += num%10 (To get last digit) and num = num/10 (To remove the last digit)
#include<stdio.h>
#include<cs50.h>

int add_digits(int num);                        // For add the Doubled digits
int first_two_digits(int length, long int num); // To get the first two digits
int first_digit(int length, long int num);      // To get first digit

int main(void)
{
    long int card_num = get_long("Enter Credit Card Number: >");
    long int card_num_copy = card_num;
    int double_sum = 0;
    int remaining_sum = 0;
    int i;

    for (i = 0; card_num_copy != 0; i++)
    {
        if (i % 2 == 1)
        {
            double_sum += add_digits(2 * (card_num_copy % 10));
            card_num_copy = card_num_copy / 10;
        }

        if (i % 2 == 0)
        {
            remaining_sum += card_num_copy % 10;
            card_num_copy = card_num_copy / 10;
        } 
    }

    if ((double_sum + remaining_sum) % 10 == 0)
    {
        int two_digit_num = first_two_digits(i, card_num);

        if (two_digit_num == 34 || two_digit_num == 37)
        {
            printf("AMEX\n");
        }
        else if (two_digit_num == 51 || two_digit_num == 52 || two_digit_num == 53 ||two_digit_num == 54 || two_digit_num == 55)
        {
            printf("MASTERCARD\n");
        }
        else if (first_digit(i, card_num) == 4)
        {
            printf("VISA\n");
        }
    }
    else 
    {
        printf("INVALID\n");
    }
}

int first_digit(int length, long int num)
{
    int first_digit;
    for (int i = 0; num != 0; i++)
    {
        num = num / 10;
        if (i == length - 2)
        {
            first_digit = num;
        }
    }
    return first_digit;
}

int first_two_digits(int length, long int num)
{
    int first_two_digits;
    for (int i = 0; num != 0; i++)
    {
        num = num / 10;
        if (i == length - 3)
        {
            first_two_digits = num;
        }
    }
    return first_two_digits;
}

int add_digits(int num)
{
    int sum = 0;
    while (num != 0)
    {
        sum += num % 10;
        num = num / 10;
    }
    return sum;
}

