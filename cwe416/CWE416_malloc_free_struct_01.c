#include <stdio.h>
#include <stdlib.h>

#include <nuttx/mm/kfence.h>

// Definition for the structure used in this test case.
// This is typically defined in std_testcase.h in the Juliet suite.
typedef struct _twoIntsStruct
{
    int intOne;
    int intTwo;
} twoIntsStruct;


/**
 * @brief Test case for CWE-416: Use After Free (struct).
 *
 * This function is adapted from the Juliet Test Suite's
 * CWE416_Use_After_Free__malloc_free_struct_01.c.
 *
 * It tests a use-after-free scenario involving a struct, which helps
 * verify KFENCE's effectiveness with more complex data types and
 * different memory sizes/layouts.
 */
int test_cwe416_malloc_free_struct_01(void)
{
    FAR twoIntsStruct *data;

    printf("\n+++ Starting test: CWE-416 Use After Free (struct) +++\n");

    /* Allocate memory for an array of structs using KFENCE API */
    data = (FAR twoIntsStruct *)kfence_alloc(100 * sizeof(twoIntsStruct));
    if (data == NULL)
    {
        printf("ERROR: kfence_alloc() failed\n");
        return -1;
    }

    /* Initialize the memory */
    for (size_t i = 0; i < 100; i++)
    {
        data[i].intOne = 1;
        data[i].intTwo = 2;
    }
    printf("Memory for structs allocated and initialized at %p\n", data);

    /*
     * POTENTIAL FLAW: The memory is freed here, but the 'data' pointer
     * is still available.
     */
    kfence_free(data);
    printf("Memory at %p has been freed.\n", data);

    /*
     * VULNERABLE OPERATION: Attempting to access the memory via the
     * stale pointer. KFENCE should detect this use-after-free error.
     */
    printf("Attempting to access freed struct memory...\n");
    printf("Value from freed struct: intOne = %d, intTwo = %d\n", data[0].intOne, data[0].intTwo);

    printf("--- Test finished: CWE-416 Use After Free (struct) ---\n");

    return 0;
}