#include "Sluice.h"
#include "CommunicationHandler.h"
#include <iostream>
#include "Door.h"
#include "lib/enums.h"
#include "lib/returnValues.h"

Sluice::Sluice(int port)
	: cHandler(port)
	, leftDoor(&cHandler, (port==5557) ? fastLock : noLock, left)
	, rightDoor(&cHandler, (port==5557) ? fastLock : noLock, right)
{

}

Sluice::~Sluice()
{

}

void Sluice::passInterrupt()
{
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
				allowEntry(left);
				break;
			case allowingEntryRight:
				allowEntry(right);
				break;
			case allowingExitLeft:
				allowExit(left);
				break;
			case allowingExitRight:
				allowExit(right);
				break;
		}
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

int Sluice::start()
{
	if (!emergency)
	{
		WaterLevel currentWLevel = cHandler.getWaterLevel();
		int rtnval;

		switch(currentWLevel)
		{
			case low:
				stateBeforeEmergency = sluicingUp;

				if (cHandler.getDoorState(left) == doorOpen)
				{
					rtnval = leftDoor.closeDoor();
					if (!(rtnval == success))
					{
						// Don't continue if we can't close the door.
						return rtnval;
					}
				}
				if (cHandler.getDoorState(left) == doorClosed || cHandler.getDoorState(left) == doorLocked)
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
										return noAckReceived;
									}
								}
								break;
							case aboveValve2:
								if(!rightDoor.middleValves.getValveRowOpened())
								{
									if (!rightDoor.middleValves.openValveRow())
									{
										return noAckReceived;
									}
								}
								break;
							case aboveValve3:
								if(!rightDoor.topValves.getValveRowOpened())
								{
									if (!rightDoor.topValves.openValveRow())
									{
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

					if (currentWLevel != high)
					{
						return interruptReceived;
					}
					else
					{
						// After finishing the process, close all valves.
						if (!closeValves(right))
						{
							return noAckReceived;
						}

						return success;
					}

				}
				break;
			case high:
				stateBeforeEmergency = sluicingDown;
				if (cHandler.getDoorState(right) == doorOpen)
				{
					rtnval = rightDoor.closeDoor();
					if (!(rtnval == success))
					{
						// Don't continue if we can't close the door.
						return rtnval;
					}
				}

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

				if (currentWLevel != low)
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
				break;
			default:
				// Can't start moving a boat that can't possibly have entered.
				return invalidWaterLevel;
		}
	}
	else
	{
		// There was an emergency, which means there should be an old state to restore to.

		if (stateBeforeEmergency == sluicingUp)
		{

		}
		else if (stateBeforeEmergency == sluicingDown)
		{

		}
		else
		{
			// There was no state to restore to. Start may have been called while an emergency was going on.
			return invalidCall;
		}
	}

	return workInProgress;
}

int Sluice::allowEntry(DoorSide side)
{
	if (side == left)
	{
		stateBeforeEmergency = allowingEntryLeft;
		return leftDoor.allowEntry();
	}
	else // side == right
	{
		stateBeforeEmergency = allowingEntryRight;
		return rightDoor.allowEntry();
	}
}

int Sluice::allowExit(DoorSide side)
{
	if (side == left)
	{
		stateBeforeEmergency = allowingExitLeft;
		return leftDoor.allowExit();
	}
	else // side == right
	{
		stateBeforeEmergency = allowingExitRight;
		return rightDoor.allowExit();
	}
}