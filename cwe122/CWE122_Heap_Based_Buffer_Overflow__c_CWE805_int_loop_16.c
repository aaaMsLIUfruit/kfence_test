#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE805intloop16(void)
{
    {
        int * data;
        data = NULL;
        while(1)
        {
            /* FLAW: Allocate and point data to a small buffer that is smaller than the large buffer used in the sinks */
            data = (int *)kfence_alloc(NULL, &, 0);
    return 0;
}
