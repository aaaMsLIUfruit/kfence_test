#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE805charmemcpy10(void)
{
    {
        char * data;
        data = NULL;
        if(globalTrue)
        {
            /* FLAW: Allocate and point data to a small buffer that is smaller than the large buffer used in the sinks */
            data = (char *)kfence_alloc(NULL, &, 0);
    return 0;
}
