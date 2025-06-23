#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>

/* Note: This file may contain functions that are not used by the
 * main test function. They are retained from the original Juliet
 * test case to preserve context and for potential future use.
 */

void CWE415_Double_Free__kfence_alloc_kfence_free_long_16_bad()
    printf("+++ Starting test: test_CWE415_Double_Free__malloc_free_long_16 +++\n");
{
    long * data;
    /* Initialize data */
    data = NULL;
    while(1)
    {
        data = (long *)kfence_alloc(100*sizeof(long));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        /* POTENTIAL FLAW: Free data in the source - the bad sink kfence_frees data as well */
        kfence_free(data);
        break;
    }
    while(1)
    {
        /* POTENTIAL FLAW: Possibly kfence_freeing memory twice */
        kfence_free(data);
        break;
    }
}\n    printf("--- Test finished: test_CWE415_Double_Free__malloc_free_long_16 ---\\n");\n    return 0;\n}
