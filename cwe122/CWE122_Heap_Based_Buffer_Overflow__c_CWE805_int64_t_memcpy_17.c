#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE805int64tmemcpy17(void)
{
    {
        int i;
        int64_t * data;
        data = NULL;
        for(i = 0; i < 1; i++)
        {
            /* FLAW: Allocate and point data to a small buffer that is smaller than the large buffer used in the sinks */
            data = (int64_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
