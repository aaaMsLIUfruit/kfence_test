#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE193charcpy18(void)
{
    {
        char * data;
        data = NULL;
        goto source;
    source:
        /* FLAW: Did not leave space for a null terminator */
        data = (char *)kfence_alloc(NULL, &, 0);
    return 0;
}
