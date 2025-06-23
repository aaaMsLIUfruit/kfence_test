#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

void CWE416_Use_After_Free__kfence_alloc_kfence_free_char_17_bad()
{
    int i,j;
    char * data;
    /* Initialize data */
    data = NULL;
    for(i = 0; i < 1; i++)
    {
        data = (char *)kfence_alloc(100*sizeof(char));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        memset(data, 'A', 100-1);
        data[100-1] = '\0';
        /* POTENTIAL FLAW: Free data in the source - the bad sink attempts to use data */
        kfence_free(data);
    }
    for(j = 0; j < 1; j++)
    {
        /* POTENTIAL FLAW: Use of data that may have been kfence_freed */
        printf("Value at freed memory location: %s\n", data);
        /* POTENTIAL INCIDENTAL - Possible memory leak here if data was not kfence_freed */
    }
}
