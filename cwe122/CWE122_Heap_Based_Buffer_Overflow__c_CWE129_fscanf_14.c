#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129fscanf14(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        if(globalFive==5)
        {
            /* POTENTIAL FLAW: Read data from the console using fscanf() */
            fscanf(stdin, "%d", &data);
    return 0;
}
