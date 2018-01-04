#include "Sluice.h"
#include "CommunicationHandler.h"
#include <iostream>
#include "Door.h"
#include "lib/enums.h"
#include "lib/returnValues.h"

// TODO: is it possible to pass references instead of pointers?
Sluice::Sluice(int port)
	: cHandler(port)
	, leftDoor(&cHandler, (port==5557) ? fastLock : noLock, left)
	, rightDoor(&cHandler, (port==5557) ? fastLock : noLock, right)
{
	emergency = false;
	stateBeforeEmergency = waitingForCommand;
}

Sluice::~Sluice()
{

}

void Sluice::passInterrupt()
{
	// TODO: This interrupt takes ages to complete. Fix that?

	leftDoor.interruptReaction();
	rightDoor.interruptReaction();

	if (!emergency)
	{
		// Emergency situation triggered
		emergency = true;
	}
	else
	{
		// Restore triggered
		switch(stateBeforeEmergency)
		{
			case sluicingUp:
				// Start itself handles the difference between up and down.
				start();
				break;
			case sluicingDown:
				// Start itself handles the difference between up and down.
				start();
				break;
			case allowingEntryLeft:
				allowEntry();
				break;
			case allowingEntryRight:
				allowEntry();
				break;
			case allowingExitLeft:
				allowExit();
				break;
			case allowingExitRight:
				allowExit();
				break;
			case waitingForCommand:
				// Do nothing.
				break;
		}

		emergency = false;
	}
}

bool Sluice::closeValves(DoorSide side)
{
	if (side == left)
	{
		if(leftDoor.topValves.getValveRowOpened())
		{
			if (!leftDoor.topValves.closeValveRow())
			{
				return false; // Failed to close valves: simulator did not acknowledge message
			}
		}
		if(leftDoor.middleValves.getValveRowOpened())
		{
			if (!leftDoor.middleValves.closeValveRow())
			{
				return false; // Failed to close valves: simulator did not acknowledge message
			}
		}
		if(leftDoor.bottomValves.getValveRowOpened())
		{
			if (!leftDoor.bottomValves.closeValveRow())
			{
				return false; // Failed to close valves: simulator did not acknowledge message
			}
		}
	}
	else // side == right
	{
		if(rightDoor.topValves.getValveRowOpened())
		{
			if (!rightDoor.topValves.closeValveRow())
			{
				return false; // Failed to close valves: simulator did not acknowledge message
			}
		}
		if(rightDoor.middleValves.getValveRowOpened())
		{
			if (!rightDoor.middleValves.closeValveRow())
			{
				return false; // Failed to close valves: simulator did not acknowledge message
			}
		}
		if(rightDoor.bottomValves.getValveRowOpened())
		{
			if (!rightDoor.bottomValves.closeValveRow())
			{
				return false; // Failed to close valves: simulator did not acknowledge message
			}
		}
	}

	// If this point is reached, sluice succeeded in closing all valves where necessary.
	return true;
}

int Sluice::sluiceUp(WaterLevel currentWLevel)
{
	do
	{
		currentWLevel = cHandler.getWaterLevel();
		switch(currentWLevel)
		{
			case low:
				if(!rightDoor.bottomValves.getValveRowOpened())
				{
					if (!rightDoor.bottomValves.openValveRow())
					{
						stateBeforeEmergency = waitingForCommand;
						return noAckReceived;
					}
				}
				break;
			case belowValve2:
				// Can't use one case for two possiblities, but these actually have the same consequences.
				if(!rightDoor.bottomValves.getValveRowOpened())
				{
					if (!rightDoor.bottomValves.openValveRow())
					{
						stateBeforeEmergency = waitingForCommand;
						return noAckReceived;
					}
				}
				break;
			case aboveValve2:
				if(!rightDoor.middleValves.getValveRowOpened())
				{
					if (!rightDoor.middleValves.openValveRow())
					{
						stateBeforeEmergency = waitingForCommand;
						return noAckReceived;
					}
				}
				break;
			case aboveValve3:
				if(!rightDoor.topValves.getValveRowOpened())
				{
					if (!rightDoor.topValves.openValveRow())
					{
						stateBeforeEmergency = waitingForCommand;
						return noAckReceived;
					}
				}
				break;
			case high:
				// Nothing to do here, about to break out of the while loop.
				break;
			case waterError:
				// Can't go on with incorrect data.
				return incorrectWaterLevel;
		}
	} while (currentWLevel != high && !emergency);

	if (emergency)
	{
		return interruptReceived;
	}
	else
	{
		// After finishing the process, close all valves.
		if (!closeValves(right))
		{
			stateBeforeEmergency = waitingForCommand;
			return noAckReceived;
		}

		stateBeforeEmergency = waitingForCommand;
		return success;
	}
}

int Sluice::sluiceDown(WaterLevel currentWLevel)
{
	leftDoor.bottomValves.openValveRow();

	do
	{
		currentWLevel = cHandler.getWaterLevel();
		if (currentWLevel == waterError)
		{
			// Can't go on with incorrect data.
			return incorrectWaterLevel;
		}
	} while (currentWLevel != low && !emergency);

	if (emergency)
	{
		return interruptReceived;
	}
	else
	{
		// After finishing the process, close all valves.
		if (!closeValves(left))
		{
			return noAckReceived;
		}
		
		return success;
	}
}

int Sluice::start()
{
	// std::cout << "[DBG] Start called" << std::endl;
	WaterLevel currentWLevel = cHandler.getWaterLevel();
	if (!emergency)
	{
		// std::cout << "[DBG] No emergency active." << std::endl;
		int rtnval;

		switch(currentWLevel)
		{
			case low:
				stateBeforeEmergency = sluicingUp;

				if (cHandler.getDoorState(left) == doorOpen)
				{
					rtnval = leftDoor.closeDoor();
					if (rtnval != success)
					{
						// Don't continue if we can't close the door.
						return rtnval;
					}
				}
				if (cHandler.getDoorState(left) == doorClosed || cHandler.getDoorState(left) == doorLocked)
				{
					return sluiceUp(currentWLevel);
				}
				else
				{
					stateBeforeEmergency = waitingForCommand;
					return incorrectDoorState; // Door is not in a state where sluicing can occur.
				}
				break;

			case high:
				stateBeforeEmergency = sluicingDown;
				if (cHandler.getDoorState(right) == doorOpen)
				{
					rtnval = rightDoor.closeDoor();
					if (rtnval != success)
					{
						// Don't continue if we can't close the door.
						return rtnval;
					}
				}

				if (cHandler.getDoorState(right) == doorClosed || cHandler.getDoorState(right) == doorLocked)
				{
					return sluiceDown(currentWLevel);
				}
				else
				{
					stateBeforeEmergency = waitingForCommand;
					return incorrectDoorState; // Door is not in a state where sluicing can occur.
				}

				break;
			default:
				// Can't start moving a boat that can't possibly have entered.
				return invalidWaterLevel;
		}
	}
	else
	{
		// std::cout << "[DBG] REEEEEEE." << std::endl;
		// There was an emergency, which means there should be an old state to restore to.

		// TODO: test if any of this works
		if (stateBeforeEmergency == sluicingUp)
		{
			// std::cout << "[DBG] Restoring sluice UP." << std::endl;
			return sluiceUp(currentWLevel);
		}
		else if (stateBeforeEmergency == sluicingDown)
		{
			// std::cout << "[DBG] Restoring sluice DOWN." << std::endl;
			return sluiceDown(currentWLevel);
		}
		else
		{
			// There was no state to restore to. Start may have been called while an emergency was going on.
			return invalidCall;
		}
	}

	return workInProgress;
}

int Sluice::allowEntry()
{
	// TODO: is it necessary to remember which side was being opened? *doesn't seem like it, there are no arguments being passed*
	WaterLevel currentWLevel = cHandler.getWaterLevel();
	if (currentWLevel == low)
	{
		stateBeforeEmergency = allowingEntryLeft;
		int rtnval = leftDoor.allowEntry();
		stateBeforeEmergency = waitingForCommand;
		return rtnval;
	}
	else if (currentWLevel == high)
	{
		stateBeforeEmergency = allowingEntryRight;
		int rtnval = rightDoor.allowEntry();
		stateBeforeEmergency = waitingForCommand;
		return rtnval;
	}
	else
	{
		return incorrectWaterLevel;
	}
}

int Sluice::allowExit()
{
	// TODO: is it necessary to remember which side was being opened? *doesn't seem like it, there are no arguments being passed*
	WaterLevel currentWLevel = cHandler.getWaterLevel();
	if (currentWLevel == low)
	{
		stateBeforeEmergency = allowingEntryLeft;
		return leftDoor.allowExit();
	}
	else if (currentWLevel == high)
	{
		stateBeforeEmergency = allowingEntryRight;
		return rightDoor.allowExit();
	}
	else
	{
		return incorrectWaterLevel;
	}
}