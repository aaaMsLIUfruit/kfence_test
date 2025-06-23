#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_loop_66b_badSink(char * dataArray[]);

int test_CWE122_loop_66(void)
{
    char * data;
    char * dataArray[5];

    printf("+++ Starting test: CWE122 Heap Overflow (loop, multi-file) +++\\n");
    
    data = (char *)malloc(50*sizeof(char));
    if (data == NULL)
    {
        printf("ERROR: malloc() failed\\n");
        return -1;
    }
    data[0] = '\0';
    
    dataArray[2] = data;
    CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_loop_66b_badSink(dataArray);

    printf("--- Test finished: CWE122 Heap Overflow (loop, multi-file) ---\\n");
    
    return 0;
} 