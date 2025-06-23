#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129fgets05(void)
{
    {
        int data;
        /* Initialize data */
        data = -1;
        if(staticTrue)
        {
            {
                char inputBuffer[CHAR_ARRAY_SIZE] = "";
                /* POTENTIAL FLAW: Read data from the console using fgets() */
                if (fgets(inputBuffer, CHAR_ARRAY_SIZE, stdin) != NULL)
                {
                    /* Convert to int */
                    data = atoi(inputBuffer);
    return 0;
}
