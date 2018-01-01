#include <iostream>

#include "Door.h"
#include "lib/enums.h"
#include "CommunicationHandler.h"

Door::Door(CommunicationHandler* existingHandler, DoorType Type, DoorSide Side)
	: cHandler(existingHandler)
	, lightInside(existingHandler, (Side==left) ? 1 : 3)
	, lightOutside(existingHandler, (Side==left) ? 2 : 4)
	, topValves(cHandler, 3, Side)
	, middleValves(cHandler, 2, Side)
	, bottomValves(cHandler, 1, Side)
{
	interruptCaught = false;
	messageReceived = false;
}

Door::~Door()
{

}

void Door::interruptReaction()
{
	if (!interruptCaught)
	{
		// No interrupt was caught yet, OHSHITPANIC
		interruptCaught = true;
		stopDoor();
	}
	else
	{
		stopDoor();
		interruptCaught = false; // Set up for another emergency stop later on
	}
}

int Door::allowExit()
{
	DoorState currentState = cHandler->getDoorState(side);
	int rtnval;

	if (currentState == doorOpen)
	{
		return lightInside.greenLight();
	}
	else if (currentState == doorClosed || currentState == doorLocked)
	{
		rtnval = openDoor();
		switch (rtnval)
		// Because greenLight has its own return values, we need to be change
		// openDoor's return to distinguish it from greenLight's.
		{
			case 0:
				// Door has been opened
				return lightInside.greenLight();
			case -1:
				return -3; // openDoor not acknowledged
			case -2:
				return -4; // Interrupt received
			case -3:
				return -5; // Water is not at a level where the door
						   // can be opened.
			default:
				return -6; // This shouldn't be possible.
		}
	}
	else
	{
		return -7; // Door is not in a state where a boat can be allowed to leave.
	}
}

int Door::allowEntry()
{
	// std::cout << "[DBG] Door::allowEntry - getDoorState\n";
	DoorState currentState = cHandler->getDoorState(side);
	int rtnval;

	if (currentState == doorOpen)
	{
		// std::cout << "[DBG] Door::allowEntry - lightOutside::greenLight (door was open)\n";
		return lightOutside.greenLight();
	}
	else if (currentState == doorClosed || currentState == doorLocked)
	{
		rtnval = openDoor();
		switch (rtnval)
		// Because greenLight has its own return values, we need to be change
		// openDoor's return to distinguish it from greenLight's.
		{
			case 0:
				// Door has been opened
				// std::cout << "[DBG] Door::allowEntry - lightOutside::greenLight (door has been opened)\n";
				return lightOutside.greenLight();
			case -1:
				return -3; // openDoor not acknowledged
			case -2:
				return -4; // Interrupt received
			case -3:
				return -5; // Water is not at a level where the door
						   // can be opened.
			default:
				return -6; // This shouldn't be possible.
		}
	}
	else
	{
		return -7; // Door is not in a state where a boat can be allowed to enter.
	}
}

int Door::openDoor()
{
	// We can assume the left door can be opened when waterLevel = low,
	// while the right door can only be opened when waterLevel = high.

	WaterLevel currentWLevel = cHandler->getWaterLevel();
	if (!((side == left && currentWLevel == low) || (side == right && currentWLevel == high)))
	{
		// The water is not at the right level to open the left door,
		// but the water also isn't at the right level to open the right door
		return -3; // Water level invalid for opening door
	}

	if (type == fastLock)
	{
		// Door is locked
		messageReceived = cHandler->unlockDoor(side);

		if (!messageReceived)
		{
			return -1; // Message was not acknowledged by the simulator
		}
		// Door is not locked
	}

	messageReceived = cHandler->openDoor(side);
	if (!messageReceived)
	{
		return -1; // Message was not acknowledged by the simulator
	}

	DoorState currentState = cHandler->getDoorState(side);
	do
	{
		if (currentState == doorStopped)
		{
			messageReceived = cHandler->openDoor(side);
			
			if (!messageReceived)
			{
				return -1; // Message was not acknowledged by the simulator
			}
		}
		currentState = cHandler->getDoorState(side);
	} while (!interruptCaught && currentState != doorOpen);

	if (currentState != doorOpen)
	{
		return -2; // An interrupt was received
	}
	else
	{
		return 0; // Door opened
	}
}

int Door::closeDoor()
{
	messageReceived = cHandler->closeDoor(side);
	if (!messageReceived)
	{
		return -1; // Message was not acknowledged by the simulator
	}

	DoorState currentState = cHandler->getDoorState(side);
	do
	{
		if (currentState == doorStopped)
		{
			messageReceived = cHandler->closeDoor(side);
			
			if (!messageReceived)
			{
				return false; // Message was not acknowledged by the simulator
			}
		}
		currentState = cHandler->getDoorState(side);
	} while (!interruptCaught && currentState != doorClosed);

	if (currentState != doorClosed)
	{
		return -2; // An interrupt was received
	}
	else
	{
		return 0; // Door opened
	}

	if (type == fastLock)
	{
		// Door should be locked
		messageReceived = cHandler->lockDoor(side);

		if (!messageReceived)
		{
			return -1; // Message was not acknowledged by the simulator
		}
		// Door is locked
	}
}

int Door::stopDoor()
{
	// The CommunicationHandler part does way too much for stopping.
	cHandler->stopDoor(side);
	return -1;
}