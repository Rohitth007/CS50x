#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX] = { 0 };

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool check_loop(int loser, int new_pair);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)     // Compares strings and adds to ranks array.
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;            // Increamenting the no of votes each got
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])    // Whoever got more votes in a pair is added to pairs struct
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;                             // Increamenting pair_count.
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int swaps = 1;
    while (swaps != 0)
    {
        swaps = 0;
        for (int i = 0; i < pair_count - 1; i++)        // Bubble soting in decreasing order
        {
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                pair temp;
                temp = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = temp;
                swaps++;
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int i = 0;
    bool loop;

    locked[pairs[i].winner][pairs[i].loser] = true;        // Locking the first strongest pair

    for (; i < pair_count - 1; i++)
    {
        loop = check_loop(pairs[i + 1].loser, i + 1);       // Reccursion function to check for loop if next pair is added

        if (!loop)
        {
            locked[pairs[i + 1].winner][pairs[i + 1].loser] = true;
        }
    }
    return;
}

// Reccursion function for loop checking
bool check_loop(int loser, int new_pair)
{
    if (loser == pairs[new_pair].winner)
    {
        return true;                 // Returns true if loser of any pair is the winner of the pair which we want to add.
    }                                // i.e if the reccursion function loops and comes back to the same point.

    for (int i = 0; i < new_pair; i++)
    {
        if (loser == pairs[i].winner)    // Checking if the loser is a winner for any of the previously existing winners.
        {
            return check_loop(pairs[i].loser, new_pair);
        }
    }
    return false;                   // Returns false if loser is not a winner of any other pair.
}                                   // i.e., we hit a deadend.

// Print the winner of the election
void print_winner(void)                 // The winner is the guy who has not lost to anybody.
{
    bool print_winner;

    for (int i = 0; i < candidate_count; i++)
    {
        print_winner = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])                 // If i is the loser then he is not a winner.
            {
                print_winner = false;
            }
        }
        if (print_winner)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}
