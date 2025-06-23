#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"



void CWE416_Use_After_Free__return_freed_ptr_05_bad()
{
    if(staticTrue)
    {
        {
            /* Call the bad helper function */
            char * reversedString = helperBad("BadSink");
            printLine(reversedString);
            /* free(reversedString);
             * This call to free() was removed because we want the tool to detect the use after free,
             * but we don't want that function to be free(). Essentially we want to avoid a double free
             */
        }
    }
}
