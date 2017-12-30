#include "Sluice.h"
#include "CommunicationHandler.h"
#include <iostream>
#include "Door.h"
#include "lib/enums.h"

Sluice::Sluice(int port)
	: cHandler(port)
	, leftDoor(cHandler, (port==5557) ? fastLock : noLock, left)
	, rightDoor(cHandler, (port==5557) ? fastLock : noLock, right)
{
	std::cout << "[DBG] Sluice creating cHandler with port nr " << port << std::endl;
}

Sluice::~Sluice()
{

}

void Sluice::passInterrupt()
{
	leftDoor.interruptReaction();
	rightDoor.interruptReaction();
}

int Sluice::start()
{
	return -1;
}

int Sluice::allowEntry(DoorSide side)
{
	if (side == left)
	{
		std::cout << "[DBG] Sluice::allowEntry(" << side << ")\n";
		return leftDoor.allowEntry();
	}
	else // side == right
	{
		return rightDoor.allowEntry();
	}
}

int Sluice::allowExit(DoorSide side)
{
	if (side == left)
	{
		return leftDoor.allowExit();
	}
	else // side == right
	{
		return rightDoor.allowExit();
	}
}

// int main(int argc, char const *argv[])
// {
// 	Sluice henk = new Sluice(5555);
// 	std::cout << henk.cHandler.getDoorState(left) << std::endl;
// 	return 0;
// }