#ifndef CWE416_TESTS_H
#define CWE416_TESTS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#ifdef CONFIG_MM_KFENCE

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/**
 * @brief Test case for CWE-416: Use After Free (char array).
 *
 * This function demonstrates a classic use-after-free scenario:
 * 1. Memory is allocated using kfence_alloc().
 * 2. The allocated memory is deallocated using kfence_free().
 * 3. An attempt is made to access the freed memory.
 *
 * This test uses KFENCE API (kfence_alloc/kfence_free) to ensure reliable
 * detection. KFENCE uses sampling mechanisms that may not detect all
 * malloc/free calls, so using the KFENCE API directly ensures the test
 * will trigger detection.
 *
 * KFENCE is expected to detect and report the memory access violation
 * that occurs after the memory has been freed.
 */
int test_cwe416_malloc_free_char_01(void);

/**
 * @brief Additional test cases for CWE-416: Use After Free (char array).
 */
int test_cwe416_malloc_free_char_02(void);
int test_cwe416_malloc_free_char_03(void);
int test_cwe416_malloc_free_char_04(void);
int test_cwe416_malloc_free_char_05(void);
int test_cwe416_malloc_free_char_06(void);
int test_cwe416_malloc_free_char_07(void);
int test_cwe416_malloc_free_char_08(void);
int test_cwe416_malloc_free_char_09(void);
int test_cwe416_malloc_free_char_10(void);
int test_cwe416_malloc_free_char_11(void);
int test_cwe416_malloc_free_char_12(void);
int test_cwe416_malloc_free_char_13(void);
int test_cwe416_malloc_free_char_14(void);
int test_cwe416_malloc_free_char_15(void);
int test_cwe416_malloc_free_char_16(void);
int test_cwe416_malloc_free_char_17(void);
int test_cwe416_malloc_free_char_18(void);

/**
 * @brief Test case for CWE-416: Use After Free (struct).
 *
 * This function tests a use-after-free scenario involving a struct, which helps
 * verify KFENCE's effectiveness with more complex data types and
 * different memory sizes/layouts.
 *
 * This test uses KFENCE API (kfence_alloc/kfence_free) to ensure reliable
 * detection across different data types and memory layouts.
 */
int test_cwe416_malloc_free_struct_01(void);

/**
 * @brief Additional test cases for CWE-416: Use After Free (struct).
 */
int test_cwe416_malloc_free_struct_02(void);
int test_cwe416_malloc_free_struct_03(void);
int test_cwe416_malloc_free_struct_04(void);
int test_cwe416_malloc_free_struct_05(void);
int test_cwe416_malloc_free_struct_06(void);
int test_cwe416_malloc_free_struct_07(void);
int test_cwe416_malloc_free_struct_08(void);
int test_cwe416_malloc_free_struct_09(void);
int test_cwe416_malloc_free_struct_10(void);
int test_cwe416_malloc_free_struct_11(void);
int test_cwe416_malloc_free_struct_12(void);
int test_cwe416_malloc_free_struct_13(void);
int test_cwe416_malloc_free_struct_14(void);
int test_cwe416_malloc_free_struct_15(void);
int test_cwe416_malloc_free_struct_16(void);
int test_cwe416_malloc_free_struct_17(void);
int test_cwe416_malloc_free_struct_18(void);

/**
 * @brief Test case for CWE-416: Use After Free (long).
 *
 * This function tests a use-after-free scenario involving long integers,
 * which helps verify KFENCE's effectiveness with different integer types
 * and memory sizes.
 *
 * This test uses KFENCE API (kfence_alloc/kfence_free) to ensure reliable
 * detection with long integer data types.
 */
int test_cwe416_malloc_free_long_01(void);

/**
 * @brief Additional test cases for CWE-416: Use After Free (long).
 */
int test_cwe416_malloc_free_long_02(void);
int test_cwe416_malloc_free_long_03(void);
int test_cwe416_malloc_free_long_04(void);
int test_cwe416_malloc_free_long_05(void);
int test_cwe416_malloc_free_long_06(void);
int test_cwe416_malloc_free_long_07(void);
int test_cwe416_malloc_free_long_08(void);
int test_cwe416_malloc_free_long_09(void);
int test_cwe416_malloc_free_long_10(void);
int test_cwe416_malloc_free_long_11(void);
int test_cwe416_malloc_free_long_12(void);
int test_cwe416_malloc_free_long_13(void);
int test_cwe416_malloc_free_long_14(void);
int test_cwe416_malloc_free_long_15(void);
int test_cwe416_malloc_free_long_16(void);
int test_cwe416_malloc_free_long_17(void);
int test_cwe416_malloc_free_long_18(void);

/**
 * @brief Test case for CWE-416: Use After Free (int64_t).
 *
 * This function tests a use-after-free scenario involving 64-bit integers,
 * which helps verify KFENCE's effectiveness with 64-bit data types
 * and different memory alignments.
 *
 * This test uses KFENCE API (kfence_alloc/kfence_free) to ensure reliable
 * detection with 64-bit integer data types.
 */
int test_cwe416_malloc_free_int64_t_01(void);

/**
 * @brief Additional test cases for CWE-416: Use After Free (int64_t).
 */
int test_cwe416_malloc_free_int64_t_02(void);
int test_cwe416_malloc_free_int64_t_03(void);
int test_cwe416_malloc_free_int64_t_04(void);
int test_cwe416_malloc_free_int64_t_05(void);
int test_cwe416_malloc_free_int64_t_06(void);
int test_cwe416_malloc_free_int64_t_07(void);
int test_cwe416_malloc_free_int64_t_08(void);
int test_cwe416_malloc_free_int64_t_09(void);
int test_cwe416_malloc_free_int64_t_10(void);
int test_cwe416_malloc_free_int64_t_11(void);
int test_cwe416_malloc_free_int64_t_12(void);
int test_cwe416_malloc_free_int64_t_13(void);
int test_cwe416_malloc_free_int64_t_14(void);
int test_cwe416_malloc_free_int64_t_15(void);
int test_cwe416_malloc_free_int64_t_16(void);
int test_cwe416_malloc_free_int64_t_17(void);
int test_cwe416_malloc_free_int64_t_18(void);

/**
 * @brief Test case for CWE-416: Use After Free (Return Freed Pointer).
 *
 * This function tests KFENCE's ability to detect use-after-free scenarios that
 * cross function boundaries. A helper function returns a pointer that
 * has already been freed, and this function attempts to use it.
 *
 * This is a more complex scenario that tests KFENCE's ability to track
 * memory state across function calls using KFENCE API.
 */
int test_cwe416_return_freed_ptr_01(void);

/**
 * @brief Additional test cases for CWE-416: Use After Free (Return Freed Pointer).
 */
int test_cwe416_return_freed_ptr_02(void);
int test_cwe416_return_freed_ptr_03(void);
int test_cwe416_return_freed_ptr_04(void);
int test_cwe416_return_freed_ptr_05(void);
int test_cwe416_return_freed_ptr_06(void);
int test_cwe416_return_freed_ptr_07(void);
int test_cwe416_return_freed_ptr_08(void);
int test_cwe416_return_freed_ptr_09(void);
int test_cwe416_return_freed_ptr_10(void);
int test_cwe416_return_freed_ptr_11(void);
int test_cwe416_return_freed_ptr_12(void);
int test_cwe416_return_freed_ptr_13(void);
int test_cwe416_return_freed_ptr_14(void);
int test_cwe416_return_freed_ptr_15(void);
int test_cwe416_return_freed_ptr_16(void);
int test_cwe416_return_freed_ptr_17(void);
int test_cwe416_return_freed_ptr_18(void);

#endif /* CONFIG_MM_KFENCE */

#endif /* CWE416_TESTS_H */ 