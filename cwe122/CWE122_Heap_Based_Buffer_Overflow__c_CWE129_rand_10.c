#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129rand10(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        if(globalTrue)
        {
            /* POTENTIAL FLAW: Set data to a random value */
            data = RAND32();
    return 0;
}
