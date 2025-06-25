#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>

/* Note: This file may contain functions that are not used by the
 * main test function. They are retained from the original Juliet
 * test case to preserve context and for potential future use.
 */

void CWE415_Double_Free__kfence_alloc_kfence_free_wchar_t_15_bad()
    printf("+++ Starting test: test_CWE415_Double_Free__malloc_free_wchar_t_15 +++\n");
{
    wchar_t * data;
    /* Initialize data */
    data = NULL;
    switch(6)
    {
    case 6:
        data = (wchar_t *)kfence_alloc(100*sizeof(wchar_t));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        /* POTENTIAL FLAW: Free data in the source - the bad sink kfence_frees data as well */
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
        /* POTENTIAL FLAW: Possibly kfence_freeing memory twice */
        kfence_free(data);
        break;
    default:
        /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
        printLine("Benign, fixed string");
        break;
    }
}\n    printf("--- Test finished: test_CWE415_Double_Free__malloc_free_wchar_t_15 ---\\n");\n    return 0;\n}
