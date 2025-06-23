#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129large06(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        if(STATIC_CONST_FIVE==5)
        {
            /* POTENTIAL FLAW: Use an invalid index */
            data = 10;
    return 0;
}
