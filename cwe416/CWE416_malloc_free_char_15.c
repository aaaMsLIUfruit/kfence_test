#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

void CWE416_Use_After_Free__kfence_alloc_kfence_free_char_15_bad()
{
    char * data;
    /* Initialize data */
    data = NULL;
    switch(6)
    {
    case 6:
        data = (char *)kfence_alloc(100*sizeof(char));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        memset(data, 'A', 100-1);
        data[100-1] = '\0';
        /* POTENTIAL FLAW: Free data in the source - the bad sink attempts to use data */
        kfence_free(data);
        break;
    default:
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
        break;
    }
    switch(7)
    {
    case 7:
        /* POTENTIAL FLAW: Use of data that may have been kfence_freed */
        printf("Value at freed memory location: %s\n", data);
        /* POTENTIAL INCIDENTAL - Possible memory leak here if data was not kfence_freed */
        break;
    default:
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
        break;
    }
}
