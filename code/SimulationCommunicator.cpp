#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>

#include "SimulationCommunicator.h"

SimulationCommunicator::SimulationCommunicator(int port)
{
	sock = CreateTCPClientSocket (port);
}

SimulationCommunicator::~SimulationCommunicator()
{
	close(sock);
}

char* SimulationCommunicator::sendMessage(const char message[])
{
	// // std::cout << "[DBG] Message to send (SimulationCommunicator): " << message << std::endl;
	int size = sizeOfMessage(message);

	if(send(sock, message, size, 0) < 0)
	{
		std::cout << "Error sending message\n";
		return NULL;
	}

	return receiveMessage();
}

char* SimulationCommunicator::receiveMessage()
{
	for (int j = 0; j < RCVBUFSIZE; ++j)
	{
		echoBuffer[j] = '\0';
	}

	if (recv(sock, echoBuffer, RCVBUFSIZE, 0) >= 0)
	{
		int size = sizeOfMessage(echoBuffer);
		echoBuffer[size-1] = '\0'; // Remove the semicolon at the end of the received message
		// // std::cout << "[DBG] Message received: " << echoBuffer << std::endl;
		return echoBuffer;
	}
	return NULL;
}

int SimulationCommunicator::sizeOfMessage(const char message[])
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