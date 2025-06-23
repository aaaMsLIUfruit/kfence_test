#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE806wchartmemcpy17(void)
{
    {
        int i;
        wchar_t * data;
        data = (wchar_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
