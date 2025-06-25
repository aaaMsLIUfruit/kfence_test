#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>

/* Note: This file may contain functions that are not used by the
 * main test function. They are retained from the original Juliet
 * test case to preserve context and for potential future use.
 */

void CWE415_Double_Free__kfence_alloc_kfence_free_int_44_bad()
    printf("+++ Starting test: test_CWE415_Double_Free__malloc_free_int_44 +++\n");
{
    int * data;
    /* define a function pointer */
    void (*funcPtr) (int *) = badSink;
    /* Initialize data */
    data = NULL;
    data = (int *)kfence_alloc(100*sizeof(int));
    if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return -1; }
    /* POTENTIAL FLAW: Free data in the source - the bad sink kfence_frees data as well */
    kfence_free(data);
    /* use the function pointer */
    funcPtr(data);
}\n    printf("--- Test finished: test_CWE415_Double_Free__malloc_free_int_44 ---\\n");\n    return 0;\n}
