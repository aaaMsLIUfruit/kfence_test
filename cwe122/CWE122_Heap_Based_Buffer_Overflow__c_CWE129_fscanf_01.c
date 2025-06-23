#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129fscanf01(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        /* POTENTIAL FLAW: Read data from the console using fscanf() */
        fscanf(stdin, "%d", &data);
        {
            int i;
            int * buffer = (int *)kfence_alloc(NULL, &, 0);
    return 0;
}
