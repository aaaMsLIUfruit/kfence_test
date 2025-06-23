#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE193wchartmemcpy15(void)
{
    {
        wchar_t * data;
        data = NULL;
        switch(6)
        {
        case 6:
            /* FLAW: Did not leave space for a null terminator */
            data = (wchar_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
