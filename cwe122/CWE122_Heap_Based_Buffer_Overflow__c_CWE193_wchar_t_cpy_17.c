#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE193wchartcpy17(void)
{
    {
        int i;
        wchar_t * data;
        data = NULL;
        for(i = 0; i < 1; i++)
        {
            /* FLAW: Did not leave space for a null terminator */
            data = (wchar_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
