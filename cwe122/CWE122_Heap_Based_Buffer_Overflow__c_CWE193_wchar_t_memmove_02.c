#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE193wchartmemmove02(void)
{
    {
        wchar_t * data;
        data = NULL;
        if(1)
        {
            /* FLAW: Did not leave space for a null terminator */
            data = (wchar_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
