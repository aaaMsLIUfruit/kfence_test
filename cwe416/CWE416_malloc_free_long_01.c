#include "std_testcase.h"
#include <nuttx/mm/kfence.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Test case for CWE-416: Use After Free (long).
 * Adapted from CWE416_Use_After_Free__malloc_free_long_01.c.
 */
int test_cwe416_malloc_free_long_01(void)
{
    FAR long *data;
    printf("\n+++ Starting test: CWE-416 Use After Free (long) +++\n");

    data = (FAR long *)kfence_alloc(100 * sizeof(long));
    if (data == NULL)
    {
        printf("ERROR: kfence_alloc() failed\n");
        return -1;
    }

    for (size_t i = 0; i < 100; i++)
    {
        data[i] = 5L;
    }
    printf("Memory (long) allocated and initialized at %p\n", data);

    kfence_free(data);
    printf("Memory at %p has been freed.\n", data);

    printf("Attempting to access freed memory...\n");
    /* VULNERABLE OPERATION: KFENCE should detect this. */
    printf("Value at freed memory location: %ld\n", data[0]);

    printf("--- Test finished: CWE-416 Use After Free (long) ---\n");
    return 0;
}