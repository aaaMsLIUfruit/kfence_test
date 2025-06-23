/* TEMPLATE GENERATED TESTCASE FILE
Filename: CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_66a.c
Label Definition File: CWE122_Heap_Based_Buffer_Overflow__c_CWE805.string.label.xml
Template File: sources-sink-66a.tmpl.c
*/
/*
 * @description
 * CWE: 122 Heap Based Buffer Overflow
 * BadSource:  Allocate using malloc() and set data pointer to a small buffer
 * GoodSource: Allocate using malloc() and set data pointer to a large buffer
 * Sinks: snprintf
 *    BadSink : Copy string to data using snprintf
 * Flow Variant: 66 Data flow: data passed in an array from one function to another in different source files
 *
 * */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

#include <wchar.h>

#ifdef _WIN32
#define SNPRINTF _snprintf
#else
#define SNPRINTF snprintf
#endif

/* The sinks are defined in the _66b file */
void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_66b_badSink(char * dataArray[]);

/* This is the new test entry point */
int test_CWE122_snprintf_66(void)
{
    char * data;
    char * dataArray[5];

    printf("+++ Starting test: CWE122 Heap Overflow (snprintf, multi-file) +++\\n");

    /* Malloc is used instead of kfence_alloc to avoid KFENCE reporting
     * a double-free on the array element when the test framework
     * cleans up the containing allocation.
     */
    data = (char *)malloc(50*sizeof(char));
    if (data == NULL)
      {
        printf("ERROR: malloc() failed\\n");
        return -1;
      }
    data[0] = '\0'; /* null terminate */

    /* Put data in array */
    dataArray[2] = data;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_snprintf_66b_badSink(dataArray);
    
    /* The memory is intentionally not freed, as the overflow in the sink
     * should have been detected by now. Freeing it here might cause
     * confusing secondary errors.
     */

    printf("--- Test finished: CWE122 Heap Overflow (snprintf, multi-file) ---\\n");

    return 0;
}
