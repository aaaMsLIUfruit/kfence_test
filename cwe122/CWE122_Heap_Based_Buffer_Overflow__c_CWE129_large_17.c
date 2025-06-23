#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129large17(void)
{
    {
        int i,j;
        int data;
        /* Initialize data */
        data = -1;
        for(i = 0; i < 1; i++)
        {
            /* POTENTIAL FLAW: Use an invalid index */
            data = 10;
    return 0;
}
