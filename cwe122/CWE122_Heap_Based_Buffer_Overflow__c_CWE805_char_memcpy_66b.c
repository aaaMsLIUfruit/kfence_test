#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

void CWE122_Heap_Based_Buffer_Overflow__c_CWE805_char_memcpy_66b_badSink(char * dataArray[])
{
    char * data = dataArray[2];
    {
        char source[100];
        memset(source, 'C', 100-1);
        source[100-1] = '\0';

        /* FLAW: Possible buffer overflow if source is larger than data */
        memcpy(data, source, 100*sizeof(char));

        printLine(data);
        /* kfence_free is not called here, as the out-of-bounds write
         * is expected to be detected by KFENCE immediately.
         * In a real scenario, this would be a memory leak if the
         * program didn't crash.
         */
    }
} 