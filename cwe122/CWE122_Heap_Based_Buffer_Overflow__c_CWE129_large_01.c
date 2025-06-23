#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129large01(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        /* POTENTIAL FLAW: Use an invalid index */
        data = 10;
        {
            int i;
            int * buffer = (int *)kfence_alloc(NULL, &, 0);
    return 0;
}
