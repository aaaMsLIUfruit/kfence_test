#include "std_testcase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <nuttx/mm/kfence.h>

/*
 * This helper function allocates memory, initializes it, but then frees it
 * before returning the pointer. This creates a dangling pointer.
 */
static FAR char *helper_returns_freed_ptr_01(const char *aString)
{
    FAR char *reversedString = NULL;
    if (aString == NULL)
    {
        return NULL;
    }

    size_t len = strlen(aString);
    reversedString = (FAR char *)kfence_alloc(len + 1);
    if (reversedString == NULL)
    {
        printf("ERROR: kfence_alloc() failed in helper function\n");
        return NULL;
    }

    // Reverse the string
    for (size_t j = 0; j < len; j++)
    {
        reversedString[j] = aString[len - j - 1];
    }
    reversedString[len] = '\0';

    printf("Helper function allocated and initialized memory at %p\n", reversedString);

    /* FLAW: Freeing a memory block and then returning a pointer to it. */
    kfence_free(reversedString);
    printf("Helper function freed memory at %p\n", reversedString);

    return reversedString;
}

/**
 * @brief Test case for CWE-416: Use After Free (Return Freed Pointer).
 *
 * This function is adapted from the Juliet Test Suite's
 * CWE416_Use_After_Free__return_freed_ptr_01.c.
 *
 * It tests KFENCE's ability to detect use-after-free scenarios that
 * cross function boundaries. A helper function returns a pointer that
 * has already been freed, and this function attempts to use it.
 */
int test_cwe416_return_freed_ptr_01(void)
{
    printf("\n+++ Starting test: CWE-416 Use After Free (return freed pointer) +++\n");

    /* Call the helper function that returns a dangling pointer */
    FAR char *dangling_ptr = helper_returns_freed_ptr_01("TestString");

    if (dangling_ptr != NULL)
    {
        /*
         * VULNERABLE OPERATION: Attempting to use the pointer returned by the
         * helper function. KFENCE should detect this as a use-after-free error.
         */
        printf("Attempting to access the returned (and freed) pointer...\n");
        printf("Content of dangling pointer: %s\n", dangling_ptr);
    }
    else
    {
        printf("Test could not be performed because helper returned NULL.\n");
    }

    printf("--- Test finished: CWE-416 Use After Free (return freed pointer) ---\n");
    return 0;
}