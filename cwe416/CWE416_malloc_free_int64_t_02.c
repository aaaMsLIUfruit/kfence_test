#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

void CWE416_Use_After_Free__kfence_alloc_kfence_free_int64_t_02_bad()
{
    int64_t * data;
    /* Initialize data */
    data = NULL;
    if(1)
    {
        data = (int64_t *)kfence_alloc(100*sizeof(int64_t));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        {
            size_t i;
            for(i = 0; i < 100; i++)
            {
                data[i] = 5LL;
            }
        }
        /* POTENTIAL FLAW: Free data in the source - the bad sink attempts to use data */
        kfence_free(data);
    }
    if(1)
    {
        /* POTENTIAL FLAW: Use of data that may have been kfence_freed */
        printLongLongLine(data[0]);
        /* POTENTIAL INCIDENTAL - Possible memory leak here if data was not kfence_freed */
    }
}
