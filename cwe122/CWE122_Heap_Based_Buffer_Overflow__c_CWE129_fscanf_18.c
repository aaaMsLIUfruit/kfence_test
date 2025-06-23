#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129fscanf18(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        goto source;
    source:
        /* POTENTIAL FLAW: Read data from the console using fscanf() */
        fscanf(stdin, "%d", &data);
        goto sink;
    sink:
        {
            int i;
            int * buffer = (int *)kfence_alloc(NULL, &, 0);
    return 0;
}
