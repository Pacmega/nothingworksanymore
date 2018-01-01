#ifndef SLUICE_H_
#define SLUICE_H_

#include "lib/enums.h"
#include "CommunicationHandler.h"
#include "Door.h"

class Sluice
{
public:
	Sluice(int port);
	~Sluice();
	
	int start();
	int allowEntry(DoorSide side);
    int allowExit(DoorSide side);
    void passInterrupt();
    CommunicationHandler cHandler;

private:
	
	Door leftDoor;
	Door rightDoor;
	int returnValue;
};

#endif