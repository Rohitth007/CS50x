// Greedy Algorithm for a Cashier
#include<stdio.h>
#include<math.h>
#include<cs50.h>

int main(void)
{
    int coins = 0;
    float dollars;
    do
    {
        dollars = get_float("How much cash do I owe you? > ");
    }
    while (dollars <= 0);
    int cents = round(dollars * 100);   // To round off all the insignificant decimal places
    
    while (cents != 0)
    {
        if (cents / 25 != 0)            // cents/25 is the number of 25 cents coins needed
        {
            coins += cents / 25;
            cents -= 25 * (cents / 25);  // Remainder of cash owed
        }
        else if (cents / 10 != 0)
        {
            coins += cents / 10;
            cents -= 10 * (cents / 10);
        }
        else if (cents / 5 != 0)
        {
            coins += cents / 5;
            cents -= 5 * (cents / 5);
        }
        else if (cents != 0)
        {
            coins += cents;
            cents -= cents;
        }
    }
    printf("%d\n", coins);
}
