#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129rand16(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        while(1)
        {
            /* POTENTIAL FLAW: Set data to a random value */
            data = RAND32();
            break;
    return 0;
}
