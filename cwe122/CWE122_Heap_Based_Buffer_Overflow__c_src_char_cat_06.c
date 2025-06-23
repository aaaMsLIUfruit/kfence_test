#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122csrccharcat06(void)
{
    {
        char * data;
        data = (char *)kfence_alloc(NULL, &, 0);
    return 0;
}
