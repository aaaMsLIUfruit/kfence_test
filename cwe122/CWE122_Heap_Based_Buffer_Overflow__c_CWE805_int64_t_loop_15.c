#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE805int64tloop15(void)
{
    {
        int64_t * data;
        data = NULL;
        switch(6)
        {
        case 6:
            /* FLAW: Allocate and point data to a small buffer that is smaller than the large buffer used in the sinks */
            data = (int64_t *)kfence_alloc(NULL, &, 0);
    return 0;
}
