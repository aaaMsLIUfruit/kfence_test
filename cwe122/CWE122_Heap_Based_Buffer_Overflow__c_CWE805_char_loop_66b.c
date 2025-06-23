#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_loop_66b_badSink(char * dataArray[])
{
    char * data = dataArray[2];
    {
        size_t i;
        char source[100];
        memset(source, 'C', 100-1);
        source[100-1] = '\0';

        /* FLAW: Possible buffer overflow if source is larger than data */
        for (i = 0; i < 100; i++)
        {
            data[i] = source[i];
        }
        data[100-1] = '\0';

        printLine(data);
        /* kfence_free is not called here for the same reason as in memcpy */
    }
} 