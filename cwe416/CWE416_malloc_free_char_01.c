#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>

#include <nuttx/mm/kfence.h>

/**
 * @brief Test case for CWE-416: Use After Free.
 *
 * This function is adapted from the Juliet Test Suite, specifically
 * from the test case CWE416_Use_After_Free__malloc_free_int_01.c.
 *
 * It demonstrates a classic use-after-free scenario:
 * 1. Memory is allocated using kfence_alloc().
 * 2. The allocated memory is deallocated using kfence_free().
 * 3. An attempt is made to access the freed memory.
 *
 * KFENCE is expected to detect and report the memory access violation
 * that occurs after the memory has been freed.
 */
int test_cwe416_malloc_free_char_01(void)
{
    FAR int *data;

    printf("+++ Starting test: CWE-416 Use After Free (kfence_alloc/kfence_free) +++\n");

    /* Allocate a block of memory using KFENCE API */
    data = (FAR int *)kfence_alloc(100 * sizeof(int));
    if (data == NULL)
    {
        printf("ERROR: kfence_alloc() failed\n");
        return -1;
    }

    /* Initialize the memory */
    for (size_t i = 0; i < 100; i++)
    {
        data[i] = 5;
    }
    printf("Memory allocated and initialized at %p\n", data);

    /*
     * POTENTIAL FLAW: The memory is freed here, but a pointer to it
     * is still available in the 'data' variable.
     */
    kfence_free(data);
    printf("Memory at %p has been freed.\n", data);

    /*
     * VULNERABLE OPERATION: Attempting to access the memory after it has
     * been freed. KFENCE should detect this as a use-after-free error.
     */
    printf("Attempting to access freed memory...\n");
    printf("Value at freed memory location: %d\n", data[0]);

    printf("--- Test finished: CWE-416 Use After Free (kfence_alloc/kfence_free) ---\n");

    return 0;
}