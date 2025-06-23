#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>

/* Note: This file may contain functions that are not used by the
 * main test function. They are retained from the original Juliet
 * test case to preserve context and for potential future use.
 */

void CWE415_Double_Free__kfence_alloc_kfence_free_long_12_bad()
    printf("+++ Starting test: test_CWE415_Double_Free__malloc_free_long_12 +++\n");
{
    long * data;
    /* Initialize data */
    data = NULL;
    if(globalReturnsTrueOrFalse())
    {
        data = (long *)kfence_alloc(100*sizeof(long));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        /* POTENTIAL FLAW: Free data in the source - the bad sink kfence_frees data as well */
        kfence_free(data);
    }
    else
    {
        data = (long *)kfence_alloc(100*sizeof(long));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        /* FIX: Do NOT kfence_free data in the source - the bad sink kfence_frees data */
    }
    if(globalReturnsTrueOrFalse())
    {
        /* POTENTIAL FLAW: Possibly kfence_freeing memory twice */
        kfence_free(data);
    }
    else
    {
        /* do nothing */
        /* FIX: Don't attempt to kfence_free the memory */
        ; /* empty statement needed for some flow variants */
    }
}\n    printf("--- Test finished: test_CWE415_Double_Free__malloc_free_long_12 ---\\n");\n    return 0;\n}
