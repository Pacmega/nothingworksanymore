#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>

#include "networkInterface.h"
#include "lib/enums.h"

networkInterface::networkInterface(int port)
{
	sock = CreateTCPClientSocket (port);
}

networkInterface::~networkInterface()
{
	close(sock);
}

char* networkInterface::receiveMessage()
{
	for (int j = 0; j < RCVBUFSIZE; ++j)
	{
		echoBuffer[j] = '\0';
	}

	if (recv(sock, echoBuffer, RCVBUFSIZE, 0) >= 0)
	{
		int size = sizeOfMessage(echoBuffer);
		echoBuffer[size-1] = '\0'; // Remove the semicolon at the end of the received message
		return echoBuffer;
	}
	return NULL;
}

char* networkInterface::sendMessage(const char message[])
{
	int size = sizeOfMessage(message);

	if(send(sock, message, size, 0) >= 0)
	{
		info_s("verbose mode", message);
	}
	else
	{
		std::cout << "Error sending message \n";
	}
	return receiveMessage();
}

int networkInterface::sizeOfMessage(const char message[])
{
    int sizeOfMsg = 0;
    for (int i = 0; i < RCVBUFSIZE; i++)
    {
        if (message[i] == '\0')
        {
            return sizeOfMsg;
        }
        else
        {
            sizeOfMsg++;
        }
    }

    // Message was not NULL terminated correctly, return error
    return -1;
}