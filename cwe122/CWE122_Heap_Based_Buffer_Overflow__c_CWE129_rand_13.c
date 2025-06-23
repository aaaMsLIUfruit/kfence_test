#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129rand13(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        if(GLOBAL_CONST_FIVE==5)
        {
            /* POTENTIAL FLAW: Set data to a random value */
            data = RAND32();
    return 0;
}
