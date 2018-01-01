#include <iostream>
#include <string.h>

#include "CommunicationHandler.h"
#include "lib/commands.h"
#include "lib/enums.h"

CommunicationHandler::CommunicationHandler(int socket)
	: simulation(SimulationCommunicator(socket))
{
	// std::cout << "[DBG] cHandler creating SimulationCommunicator with socket " << socket << std::endl;
	savedLeftDoor.savedDoorState = doorStateError;
	savedLeftDoor.topValveOpen = false;
	savedLeftDoor.middleValveOpen = false;
	savedLeftDoor.bottomValveOpen = false;

	savedRightDoor.savedDoorState = doorStateError;
	savedRightDoor.topValveOpen = false;
	savedRightDoor.middleValveOpen = false;
	savedRightDoor.bottomValveOpen = false;
}

CommunicationHandler::~CommunicationHandler()
{

}

DoorState CommunicationHandler::getDoorState(DoorSide side)
{
	DoorState dState = doorStateError;
	const char* messageToSend;

	if (side == left)
	{
		messageToSend = GetDoorLeft;
	}
	else // side == right
	{
		messageToSend = GetDoorRight;
	}

	// std::cout << "[DBG] Message to send: " << messageToSend << std::endl;
	receivedMessage = simulation.sendMessage(messageToSend);
	
	// Switch cases aren't possible for strings sadly.
	if (strcmp(receivedMessage, "doorLocked") == 0)
	{
		dState = doorLocked;
	}
	else if (strcmp(receivedMessage, "doorClosed") == 0)
	{
		dState = doorClosed;
	}
	else if (strcmp(receivedMessage, "doorOpen") == 0)
	{
		dState = doorOpen;
	}
	else if (strcmp(receivedMessage, "doorClosing") == 0)
	{
		dState = doorClosing;
	}
	else if (strcmp(receivedMessage, "doorOpening") == 0)
	{
		dState = doorOpening;
	}
	else if (strcmp(receivedMessage, "doorStopped") == 0)
	{
		dState = doorStopped;
	}
	else if (strcmp(receivedMessage, "motorDamage") == 0)
	{
		dState = motorDamage;
	}

	return dState;
}

bool CommunicationHandler::lockDoor(DoorSide side)
{
	if (side == left)
	{
		receivedMessage = simulation.sendMessage(DoorLeftLock);
	}
	else // side == right
	{
		receivedMessage = simulation.sendMessage(DoorRightLock);
	}

	if (strcmp(receivedMessage, "ack") != 0)
	{
		return false; // Message was not acknowledged by the simulator or door could not be locked.
	}
	else
	{
		return true;
	}
}

bool CommunicationHandler::unlockDoor(DoorSide side)
{
	if (side == left)
	{
		receivedMessage = simulation.sendMessage(DoorLeftUnlock);
	}
	else // side == right
	{
		receivedMessage = simulation.sendMessage(DoorRightUnlock);
	}

	if (strcmp(receivedMessage, "ack") != 0)
	{
		return false; // Message was not acknowledged by the simulator
	}
	else
	{
		return true;
	}
}

bool CommunicationHandler::openDoor(DoorSide side)
{
	if (side == left)
	{
		receivedMessage = simulation.sendMessage(DoorLeftOpen);
	}
	else // side == right
	{
		receivedMessage = simulation.sendMessage(DoorRightOpen);
	}

	if (strcmp(receivedMessage, "ack") == 0)
	{
		return true; // Door was told to open.
	}
	else
	{
		return false; // Message was not acknowledged by the simulator
	}
}

bool CommunicationHandler::closeDoor(DoorSide side)
{
	// Door should deal with locking itself.

	if (side == left)
	{
		receivedMessage = simulation.sendMessage(DoorLeftClose);
	}
	else // side == right
	{
		receivedMessage = simulation.sendMessage(DoorRightClose);
	}

	if (strcmp(receivedMessage, "ack") == 0)
	{
		return true; // Door was told to close.
	}
	else
	{
		return false; // Message was not acknowledged by the simulator
	}
}

bool CommunicationHandler::stopDoor(DoorSide side)
{
	// TODO: if there is time, move the state saving and recovering to door itself

	// Only stop doors that are currently actually doing something,
	// and close any opened valves.
	// If this function is called when a door is already stopped,
	// the door is instead restored to its original status.

	// And yes, this function probably does way too much.
	// Consequence of having 0 free time, no vacation, and no weekend to speak of.
	// Any free time I was basically forced to have only increased stress levels.

	const char* getStatusMsg;

	if (side == left)
	{
		getStatusMsg = GetDoorLeft;
	}
	else // side == right
	{
		getStatusMsg = GetDoorRight;
	}

	receivedMessage = simulation.sendMessage(getStatusMsg);

	// Switch cases still aren't possible for strings sadly.
	// There are only two cases where the door actually needs to be stopped:
	// when it's closing and when it's opening.
	if (strcmp(receivedMessage, "doorClosing") == 0)
	{
		if (side == left)
		{
			savedLeftDoor.savedDoorState = doorClosing;

			receivedMessage = simulation.sendMessage(DoorLeftStop);
		}
		else // side == right
		{
			savedRightDoor.savedDoorState = doorClosing;

			receivedMessage = simulation.sendMessage(DoorRightStop);
		}

		if (strcmp(receivedMessage, "ack") == 0)
		{
			return 0; // Successfully stopped and saved state
		}
		else
		{
			return -1; // Message was not acknowledged by simulator
		}
	}
	else if (strcmp(receivedMessage, "doorOpening") == 0)
	{
		if (side == left)
		{
			savedLeftDoor.savedDoorState = doorOpening;

			receivedMessage = simulation.sendMessage(DoorLeftStop);
		}
		else // side == right
		{
			savedRightDoor.savedDoorState = doorOpening;

			receivedMessage = simulation.sendMessage(DoorRightStop);
		}

		if (strcmp(receivedMessage, "ack") == 0)
		{
			return 0; // Successfully stopped and saved state
		}
		else
		{
			return -1; // Message was not acknowledged by simulator
		}
	}
	else if (strcmp(receivedMessage, "doorClosed") == 0 || strcmp(receivedMessage, "doorLocked") == 0)
	{
		if (side == left)
		{
			if (strcmp(receivedMessage, "doorClosed") == 0)
			{
				savedLeftDoor.savedDoorState = doorClosed;
			}
			else // state = doorLocked
			{
				savedLeftDoor.savedDoorState = doorLocked;
			}

			savedLeftDoor.topValveOpen = getValveOpened(left, 3);
			savedLeftDoor.middleValveOpen = getValveOpened(left, 2);
			savedLeftDoor.bottomValveOpen = getValveOpened(left, 1);

			bool messageReceived;
			if (savedLeftDoor.topValveOpen)
			{
				messageReceived = valveClose(left, 3);
				if (messageReceived)
				{
					if (savedLeftDoor.middleValveOpen)
					{
						messageReceived = valveClose(left, 2);
						if (messageReceived)
						{
							if (savedLeftDoor.bottomValveOpen)
							{
								messageReceived = valveClose(left, 1);
								if (messageReceived)
								{
									return 0; // Success
								}
							}
							else
							{
								return 0; // Success
							}
						}
					}
					else if (savedLeftDoor.bottomValveOpen)
					{
						messageReceived = valveClose(left, 1);
						if (messageReceived)
						{
							return 0; // Success
						}
					}
					else
					{
						return 0; // Success
					}
				}
			}
			else if (savedLeftDoor.middleValveOpen)
			{
				messageReceived = valveClose(left, 2);
				if (messageReceived)
				{
					if (savedLeftDoor.bottomValveOpen)
					{
						messageReceived = valveClose(left, 1);
						if (messageReceived)
						{
							return 0; // Success
						}
					}
					else
					{
						return 0; // Success
					}
				}
			}
			else if (savedLeftDoor.bottomValveOpen)
			{
				messageReceived = valveClose(left, 1);
				if (messageReceived)
				{
					return 0; // Success
				}
			}
			else
			{
				return 0; // No valves to close.
			}
		}
		else // side == right
		{
			if (strcmp(receivedMessage, "doorClosed") == 0)
			{
				savedRightDoor.savedDoorState = doorClosed;
			}
			else // state = doorLocked
			{
				savedRightDoor.savedDoorState = doorLocked;
			}

			savedRightDoor.topValveOpen = getValveOpened(right, 3);
			savedRightDoor.middleValveOpen = getValveOpened(right, 2);
			savedRightDoor.bottomValveOpen = getValveOpened(right, 1);

			bool messageReceived;
			if (savedRightDoor.topValveOpen)
			{
				messageReceived = valveClose(right, 3);
				if (messageReceived)
				{
					if (savedRightDoor.middleValveOpen)
					{
						messageReceived = valveClose(right, 2);
						if (messageReceived)
						{
							if (savedRightDoor.bottomValveOpen)
							{
								messageReceived = valveClose(right, 1);
								if (messageReceived)
								{
									return 0; // Success
								}
							}
							else
							{
								return 0; // Success
							}
						}
					}
					else if (savedRightDoor.bottomValveOpen)
					{
						messageReceived = valveClose(right, 1);
						if (messageReceived)
						{
							return 0; // Success
						}
					}
					else
					{
						return 0; // Success
					}
				}
			}
			else if (savedRightDoor.middleValveOpen)
			{
				messageReceived = valveClose(right, 2);
				if (messageReceived)
				{
					if (savedRightDoor.bottomValveOpen)
					{
						messageReceived = valveClose(right, 1);
						if (messageReceived)
						{
							return 0; // Success
						}
					}
					else
					{
						return 0; // Success
					}
				}
			}
			else if (savedRightDoor.bottomValveOpen)
			{
				messageReceived = valveClose(right, 1);
				if (messageReceived)
				{
					return 0; // Success
				}
			}
			else
			{
				return 0; // No valves to close.
			}
		}

		// If this point is reached, somewhere along the way the simulator told the user to fuck off. (nack)
		return -1;
	}
	else if (strcmp(receivedMessage, "doorStopped") == 0)
	{
		// Door was stopped, restore it to the way it was.
		bool messageReceived;

		if (side == left)
		{
			switch (savedLeftDoor.savedDoorState)
			{
				case doorOpening:
					messageReceived = openDoor(left);
					if (messageReceived)
					{
						return 0; // Success
					}
					else
					{
						return -1; // Message was not acknowledged by simulator
					}
				case doorClosing:
					messageReceived = closeDoor(left);
					if (messageReceived)
					{
						return 0; // Success
					}
					else
					{
						return -1; // Message was not acknowledged by simulator
					}
				case doorLocked:
					if (savedLeftDoor.topValveOpen)
					{
						messageReceived = valveOpen(left, 3);
						if (messageReceived)
						{
							if (savedLeftDoor.middleValveOpen)
							{
								messageReceived = valveOpen(left, 2);
								if (messageReceived)
								{
									if (savedLeftDoor.bottomValveOpen)
									{
										messageReceived = valveOpen(left, 1);
										if (messageReceived)
										{
											return 0; // Success
										}
										else
										{
											return -1; // Message was not acknowledged by simulator
										}
									}
									else
									{
										return 0; // Success
									}
								}
								else
								{
									return -1; // Message was not acknowledged by simulator
								}
							}
							else if (savedLeftDoor.bottomValveOpen)
							{
								messageReceived = valveOpen(left, 1);
								if (messageReceived)
								{
									return 0; // Success
								}
								else
								{
									return -1; // Message was not acknowledged by simulator
								}
							}
							else
							{
								return 0; // Success
							}
						}
						else
						{
							return -1; // Message was not acknowledged by simulator
						}
					}
					else if (savedLeftDoor.middleValveOpen)
					{
						messageReceived = valveOpen(left, 2);
						if (messageReceived)
						{
							if (savedLeftDoor.bottomValveOpen)
							{
								messageReceived = valveOpen(left, 1);
								if (messageReceived)
								{
									return 0; // Success
								}
								else
								{
									return -1; // Message was not acknowledged by simulator
								}
							}
							else
							{
								return 0; // Success
							}
						}
						else
						{
							return -1; // Message was not acknowledged by simulator
						}
					}
					else if (savedLeftDoor.bottomValveOpen)
					{
						messageReceived = valveOpen(left, 1);
						if (messageReceived)
						{
							return 0; // Success
						}
						else
						{
							return -1; // Message was not acknowledged by simulator
						}
					}
					else
					{
						return 0; // There were no valves opened, so none were reopened.
					}
				default:
					// Nothing was changed by stopping the door, so there is nothing to change when restarting it.
					return 0;
			}
		}
		else // side == right
		{
			switch (savedRightDoor.savedDoorState)
			{
				case doorOpening:
					messageReceived = openDoor(right);
					if (messageReceived)
					{
						return 0; // Success
					}
					else
					{
						return -1; // Message was not acknowledged by simulator
					}
				case doorClosing:
					messageReceived = closeDoor(right);
					if (messageReceived)
					{
						return 0; // Success
					}
					else
					{
						return -1; // Message was not acknowledged by simulator
					}
				case doorLocked:
					if (savedRightDoor.topValveOpen)
					{
						messageReceived = valveOpen(right, 3);
						if (messageReceived)
						{
							if (savedRightDoor.middleValveOpen)
							{
								messageReceived = valveOpen(right, 2);
								if (messageReceived)
								{
									if (savedRightDoor.bottomValveOpen)
									{
										messageReceived = valveOpen(right, 1);
										if (messageReceived)
										{
											return 0; // Success
										}
										else
										{
											return -1; // Message was not acknowledged by simulator
										}
									}
									else
									{
										return 0; // Success
									}
								}
								else
								{
									return -1; // Message was not acknowledged by simulator
								}
							}
							else if (savedRightDoor.bottomValveOpen)
							{
								messageReceived = valveOpen(right, 1);
								if (messageReceived)
								{
									return 0; // Success
								}
								else
								{
									return -1; // Message was not acknowledged by simulator
								}
							}
							else
							{
								return 0; // Success
							}
						}
						else
						{
							return -1; // Message was not acknowledged by simulator
						}
					}
					else if (savedRightDoor.middleValveOpen)
					{
						messageReceived = valveOpen(right, 2);
						if (messageReceived)
						{
							if (savedRightDoor.bottomValveOpen)
							{
								messageReceived = valveOpen(right, 1);
								if (messageReceived)
								{
									return 0; // Success
								}
								else
								{
									return -1; // Message was not acknowledged by simulator
								}
							}
							else
							{
								return 0; // Success
							}
						}
						else
						{
							return -1; // Message was not acknowledged by simulator
						}
					}
					else if (savedRightDoor.bottomValveOpen)
					{
						messageReceived = valveOpen(right, 1);
						if (messageReceived)
						{
							return 0; // Success
						}
						else
						{
							return -1; // Message was not acknowledged by simulator
						}
					}
					else
					{
						return 0; // There were no valves opened, so none were reopened.
					}
				default:
					// Nothing was changed by stopping the door, so there is nothing to change when restarting it.
					return 0;
			}
		}

		return -1; // Not finished
	}
	else
	{
		// There is nothing to stop.
		return 0;
	}
}

bool CommunicationHandler::getValveOpened(DoorSide side, int row)
{
	bool opened = false;
	const char* messageToSend;

	if (side == left)
	{
		switch (row)
		{
			case 1:
				messageToSend = DoorLeftGetBottomValve;
				break;
			case 2:
				messageToSend = DoorLeftGetMiddleValve;
				break;
			case 3:
				messageToSend = DoorLeftGetTopValve;
				break;
		}
	}
	else // side == right
	{
		switch (row)
		{
			case 1:
				messageToSend = DoorRightGetBottomValve;
				break;
			case 2:
				messageToSend = DoorRightGetMiddleValve;
				break;
			case 3:
				messageToSend = DoorRightGetTopValve;
				break;
		}
	}

	receivedMessage = simulation.sendMessage(messageToSend);

	if (strcmp(receivedMessage, "open") == 0)
	{
		opened = true;
	}

	return opened;
}

bool CommunicationHandler::valveOpen(DoorSide side, int row)
{
	// Valves don't break when opened while already open, so no need to check.
	
	if (row >= 1 && row <= 3)
	{
		switch(side)
		{
			case left:
				switch(row)
				{
					case 1:
						receivedMessage = simulation.sendMessage(DoorLeftOpenBottomValve);
						break;
					case 2:
						receivedMessage = simulation.sendMessage(DoorLeftOpenMiddleValve);
						break;
					case 3:
						receivedMessage = simulation.sendMessage(DoorLeftOpenTopValve);
						break;
				}
				break;
			case right:
				switch(row)
				{
					case 1:
						receivedMessage = simulation.sendMessage(DoorRightOpenBottomValve);
						break;
					case 2:
						receivedMessage = simulation.sendMessage(DoorRightOpenMiddleValve);
						break;
					case 3:
						receivedMessage = simulation.sendMessage(DoorRightOpenTopValve);
						break;
				}
				break;
		}

		if (strcmp(receivedMessage, "ack") == 0)
		{
			return true; // Valve opened.
		}
	}
	
	return false;
}

bool CommunicationHandler::valveClose(DoorSide side, int row)
{
	
	// Valves don't break when closed while already closed, so no need to check.
	
	if (row >= 1 && row <= 3)
	{
		switch(side)
		{
			case left:
				switch(row)
				{
					case 1:
						receivedMessage = simulation.sendMessage(DoorLeftCloseBottomValve);
						break;
					case 2:
						receivedMessage = simulation.sendMessage(DoorLeftCloseMiddleValve);
						break;
					case 3:
						receivedMessage = simulation.sendMessage(DoorLeftCloseTopValve);
						break;
				}
				break;
			case right:
				switch(row)
				{
					case 1:
						receivedMessage = simulation.sendMessage(DoorRightCloseBottomValve);
						break;
					case 2:
						receivedMessage = simulation.sendMessage(DoorRightCloseMiddleValve);
						break;
					case 3:
						receivedMessage = simulation.sendMessage(DoorRightCloseTopValve);
						break;
				}
				break;
		}

		if (strcmp(receivedMessage, "ack") == 0)
		{
			return true; // Valve closed.
		}
	}
	
	return false;
}

int CommunicationHandler::redLight(int lightLocation)
{
	if (lightLocation >= 1 && lightLocation <= 4)
	{
		const char* message1ToSend;
		const char* message2ToSend;

		switch(lightLocation)
		{
			case 1:
				message1ToSend = TrafficLight1GreenOff;
				message2ToSend = TrafficLight1RedOn;
				break;
			case 2:
				message1ToSend = TrafficLight2GreenOff;
				message2ToSend = TrafficLight2RedOn;
				break;
			case 3:
				message1ToSend = TrafficLight3GreenOff;
				message2ToSend = TrafficLight3RedOn;
				break;
			case 4:
				message1ToSend = TrafficLight4GreenOff;
				message2ToSend = TrafficLight4RedOn;
				break;
		}

		receivedMessage = simulation.sendMessage(message1ToSend);
	
		if (strcmp(receivedMessage, "ack") == 0)
		{
			receivedMessage = simulation.sendMessage(message2ToSend);
			if (strcmp(receivedMessage, "ack") == 0)
			{
				return 0; // Success
			}
			else
			{
				return -1; // Only one of the lights was changed
			}
		}
		else
		{
			return -2; // None of the lights were changed.
		}
	}
	else
	{
		return -3; // Invalid lightLocation was passed
	}
}

int CommunicationHandler::greenLight(int lightLocation)
{
	if (lightLocation >= 1 && lightLocation <= 4)
	{
		const char* message1ToSend;
		const char* message2ToSend;

		switch(lightLocation)
		{
			case 1:
				message1ToSend = TrafficLight1RedOff;
				message2ToSend = TrafficLight1GreenOn;
				break;
			case 2:
				message1ToSend = TrafficLight2RedOff;
				message2ToSend = TrafficLight2GreenOn;
				break;
			case 3:
				message1ToSend = TrafficLight3RedOff;
				message2ToSend = TrafficLight3GreenOn;
				break;
			case 4:
				message1ToSend = TrafficLight4RedOff;
				message2ToSend = TrafficLight4GreenOn;
				break;
		}

		receivedMessage = simulation.sendMessage(message1ToSend);
	
		if (strcmp(receivedMessage, "ack") == 0)
		{
			receivedMessage = simulation.sendMessage(message2ToSend);
			if (strcmp(receivedMessage, "ack") == 0)
			{
				return 0; // Success
			}
		}
	}
	else
	{
		return -2; // Invalid lightLocation was passed
	}

	return -1; // One of the messages was not received by the simulator
}

LightState CommunicationHandler::getLightState(int lightLocation)
{
	LightState lState = lightError;

	if (lightLocation >= 1 && lightLocation <= 4)
	{
		const char* redLightMessage;
		const char* greenLightMessage;

		switch(lightLocation)
		{
			case 1:
				redLightMessage = TrafficLight1RedStatus;
				greenLightMessage = TrafficLight1GreenStatus;
				break;
			case 2:
				redLightMessage = TrafficLight2RedStatus;
				greenLightMessage = TrafficLight2GreenStatus;
				break;
			case 3:
				redLightMessage = TrafficLight3RedStatus;
				greenLightMessage = TrafficLight3GreenStatus;
				break;
			case 4:
				redLightMessage = TrafficLight4RedStatus;
				greenLightMessage = TrafficLight4GreenStatus;
				break;	
		}

		std::string redLightReceived = simulation.sendMessage(redLightMessage);
		std::string greenLightReceived = simulation.sendMessage(greenLightMessage);

		std::cout << "Red light state: " << redLightReceived << std::endl;
		std::cout << "Green light state: " << greenLightReceived << std::endl;

		if (redLightReceived == "on" && greenLightReceived == "off")
		{
			lState = redLightOn;
		}
		else if (redLightReceived == "off" && greenLightReceived == "on")
		{
			lState = greenLightOn;
		}
	}

	return lState;
}

WaterLevel CommunicationHandler::getWaterLevel()
{
	WaterLevel wLevel = waterError;
	receivedMessage = simulation.sendMessage(GetWaterLevel);

	// Switch cases aren't possible for strings sadly.
	if (strcmp(receivedMessage, "low") == 0)
	{
		wLevel = low;
	}
	else if (strcmp(receivedMessage, "belowValve2") == 0)
	{
		wLevel = belowValve2;
	}
	else if (strcmp(receivedMessage, "aboveValve2") == 0)
	{
		wLevel = aboveValve2;
	}
	else if (strcmp(receivedMessage, "aboveValve3") == 0)
	{
		wLevel = aboveValve3;
	}
	else if (strcmp(receivedMessage, "high") == 0)
	{
		wLevel = high;
	}

	return wLevel;
}

// int main(int argc, char const *argv[])
// {
// 	CommunicationHandler henk(5555);
// 	std::cout << henk.getDoorState(left) << std::endl;
// 	return 0;
// }