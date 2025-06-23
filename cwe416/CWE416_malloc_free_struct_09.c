#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

void CWE416_Use_After_Free__kfence_alloc_kfence_free_struct_09_bad()
{
    twoIntsStruct * data;
    /* Initialize data */
    data = NULL;
    if(GLOBAL_CONST_TRUE)
    {
        data = (twoIntsStruct *)kfence_alloc(100*sizeof(twoIntsStruct));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        {
            size_t i;
            for(i = 0; i < 100; i++)
            {
                data[i].intOne = 1;
                data[i].intTwo = 2;
            }
        }
        /* POTENTIAL FLAW: Free data in the source - the bad sink attempts to use data */
        kfence_free(data);
    }
    if(GLOBAL_CONST_TRUE)
    {
        /* POTENTIAL FLAW: Use of data that may have been kfence_freed */
        printStructLine(&data[0]);
        /* POTENTIAL INCIDENTAL - Possible memory leak here if data was not kfence_freed */
    }
}
