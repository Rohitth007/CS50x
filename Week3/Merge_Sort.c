#include <stdio.h>

int array[4] = {3, 7, 5, 6};

void mergesort(int l, int r);
void merge(int l, int m, int r);

int main(void)
{
    mergesort(0, 3);
    for (int a = 0; a < 4; a++)
    {
        printf("%d", array[a]);
    }
    printf("\n");
}

void mergesort(int l, int r)
{
    if (l < r)
    {
        // for (int a = l; a <= r; a++)
        // {
        //     printf("%d",array[a]);
        // }
        // printf("\n");
        int m = (l + r) / 2;
        mergesort(l, m);     // Search Left Half
        mergesort(m + 1, r);  // Search Right Half
        merge(l, m, r);      // Merge
    }
    return;
}

void merge(int l, int m, int r)
{
    int i, j;
    int L[m - l + 1];
    for (i = 0; i < m - l + 1; i++)
    {
        L[i] = array[l + i];
    }
    int R[r - m];
    for (j = 0; j < r - m; j++)
    {
        R[j] = array[m + 1 + j];
    }

    i = 0;
    j = 0;
    int k;
    for (k = l; i < m - l + 1 && j < r - m; k++) // Sorting two sorted arrays
    {
        if (L[i] > R[j])
        {
            array[k] = L[i];
            i++;
        }
        else if (L[i] <= R[j])
        {
            array[k] = R[j];
            j++;
        }
    }
    for (; i < m - l + 1; i++, k++)   // Adding whatever is left in Left Half
    {
        array[k] = L[i];
    }
    for (; j < r - m; j++, k++)       // Adding whatever is left in right half
    {
        array[k] = R[j];
    }
    // for (int a = l; a <= r; a++)
    // {
    //      printf("%d",array[a]);
    // }
    // printf("\n");
    return;
}


