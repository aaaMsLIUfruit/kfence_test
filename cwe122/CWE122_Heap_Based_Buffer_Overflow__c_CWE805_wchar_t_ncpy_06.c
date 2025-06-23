#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE805wchartncpy06(void)
{
    {
        wchar_t * data;
        data = NULL;
        if(STATIC_CONST_FIVE==5)
        {
            /* FLAW: Allocate and point data to a small buffer that is smaller than the large buffer used in the sinks */
            data = (wchar_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
