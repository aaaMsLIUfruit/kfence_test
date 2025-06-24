#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>

/* Note: This file may contain functions that are not used by the
 * main test function. They are retained from the original Juliet
 * test case to preserve context and for potential future use.
 */

void CWE415_Double_Free__kfence_alloc_kfence_free_char_01_bad()
{
    printf("+++ Starting test: test_CWE415_Double_Free__malloc_free_char_01 +++\n");
    char * data;
    /* Initialize data */
    data = NULL;
    data = (char *)kfence_alloc(NULL, 100*sizeof(char));
    if (data == NULL) { printf("ERROR: kfence_alloc() failed\n"); return; }
    /* POTENTIAL FLAW: Free data in the source - the bad sink kfence_frees data as well */
    kfence_free(data);
    /* POTENTIAL FLAW: Possibly kfence_freeing memory twice */
    kfence_free(data);
    printf("--- Test finished: test_CWE415_Double_Free__malloc_free_char_01 ---\n");
    return;
}
