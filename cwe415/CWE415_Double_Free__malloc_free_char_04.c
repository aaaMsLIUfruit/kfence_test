#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>

/* Note: This file may contain functions that are not used by the
 * main test function. They are retained from the original Juliet
 * test case to preserve context and for potential future use.
 */

void CWE415_Double_Free__kfence_alloc_kfence_free_char_04_bad()
    printf("+++ Starting test: test_CWE415_Double_Free__malloc_free_char_04 +++\n");
{
    char * data;
    /* Initialize data */
    data = NULL;
    if(STATIC_CONST_TRUE)
    {
        data = (char *)kfence_alloc(100*sizeof(char));
        if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
        /* POTENTIAL FLAW: Free data in the source - the bad sink kfence_frees data as well */
        kfence_free(data);
    }
    if(STATIC_CONST_TRUE)
    {
        /* POTENTIAL FLAW: Possibly kfence_freeing memory twice */
        kfence_free(data);
    }
}\n    printf("--- Test finished: test_CWE415_Double_Free__malloc_free_char_04 ---\\n");\n    return 0;\n}
