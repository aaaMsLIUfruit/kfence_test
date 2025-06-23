#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129rand15(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        switch(6)
        {
        case 6:
            /* POTENTIAL FLAW: Set data to a random value */
            data = RAND32();
            break;
        default:
            /* INCIDENTAL: CWE 561 Dead Code, the code below will never run */
            printLine("Benign, fixed string");
            break;
    return 0;
}
