#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE193wchartloop09(void)
{
    {
        wchar_t * data;
        data = NULL;
        if(GLOBAL_CONST_TRUE)
        {
            /* FLAW: Did not leave space for a null terminator */
            data = (wchar_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
