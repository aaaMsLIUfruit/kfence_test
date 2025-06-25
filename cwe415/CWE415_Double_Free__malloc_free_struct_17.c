#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>

/* Note: This file may contain functions that are not used by the
 * main test function. They are retained from the original Juliet
 * test case to preserve context and for potential future use.
 */

void CWE415_Double_Free__kfence_alloc_kfence_free_struct_17_bad()
    printf("+++ Starting test: test_CWE415_Double_Free__malloc_free_struct_17 +++\n");
{
    int i,j;
    twoIntsStruct * data;
    /* Initialize data */
    data = NULL;
    for(i = 0; i < 1; i++)
    {
        data = (twoIntsStruct *)kfence_alloc(100*sizeof(twoIntsStruct));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        /* POTENTIAL FLAW: Free data in the source - the bad sink kfence_frees data as well */
        kfence_free(data);
    }
    for(j = 0; j < 1; j++)
    {
        /* POTENTIAL FLAW: Possibly kfence_freeing memory twice */
        kfence_free(data);
    }
}\n    printf("--- Test finished: test_CWE415_Double_Free__malloc_free_struct_17 ---\\n");\n    return 0;\n}
