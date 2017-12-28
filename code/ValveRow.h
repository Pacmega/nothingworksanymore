#ifndef VALVEROW_H_
#define VALVEROW_H_

#include "CommunicationHandler.h"
#include "enums.h"

class ValveRow
{
public:
	ValveRow();
	~ValveRow();

	bool openValveRow();
	bool closeValveRow();
	bool getValveRowOpened();

private:
	bool messageReceived;
	int row;
	DoorSide side;	
};

#endif