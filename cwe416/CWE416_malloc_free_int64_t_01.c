#include "std_testcase.h"
#include <nuttx/mm/kfence.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Test case for CWE-416: Use After Free (int64_t).
 * Adapted from CWE416_Use_After_Free__malloc_free_int64_t_01.c.
 */
int test_cwe416_malloc_free_int64_t_01(void)
{
    FAR int64_t *data;
    printf("\n+++ Starting test: CWE-416 Use After Free (int64_t) +++\n");

    data = (FAR int64_t *)kfence_alloc(100 * sizeof(int64_t));
    if (data == NULL)
    {
        printf("ERROR: kfence_alloc() failed\n");
        return -1;
    }

    for (size_t i = 0; i < 100; i++)
    {
        data[i] = 5LL;
    }
    printf("Memory (int64_t) allocated and initialized at %p\n", data);

    kfence_free(data);
    printf("Memory at %p has been freed.\n", data);

    printf("Attempting to access freed memory...\n");
    /* VULNERABLE OPERATION: KFENCE should detect this. */
    printf("Value at freed memory location: %" PRId64 "\n", data[0]);

    printf("--- Test finished: CWE-416 Use After Free (int64_t) ---\n");
    return 0;
}