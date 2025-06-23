#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <nuttx/mm/kfence.h>
#include "std_testcase.h"

int testcwe122cCWE129listensocket17(void)
{
    {
        int i,j;
        int data;
        /* Initialize data */
        data = -1;
        for(i = 0; i < 1; i++)
        {
            {
    #ifdef _WIN32
                WSADATA wsaData;
                int wsaDataInit = 0;
    #endif
                int recvResult;
                struct sockaddr_in service;
                SOCKET listenSocket = INVALID_SOCKET;
                SOCKET acceptSocket = INVALID_SOCKET;
                char inputBuffer[CHAR_ARRAY_SIZE];
                do
                {
    #ifdef _WIN32
                    if (WSAStartup(MAKEWORD(2,2), &wsaData) != NO_ERROR)
                    {
                        break;
    return 0;
}
