#include <string.h>

#include "CommunicationHandler.h"
#include "lib/commands.h"
#include "lib/enums.h"

CommunicationHandler::CommunicationHandler(int socket)
	: simulation(SimulationCommunicator(socket))
{

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

bool CommunicationHandler::openDoor(DoorSide side)
{
	// Hint: doors with a lock have doorLocked as state at this point

	// TODO: make this

	return false;
}

bool CommunicationHandler::closeDoor(DoorSide side)
{
	// TODO: make this

	// Hint: doors with a lock need to have doorLocked as state at this point

	return false;
}

bool CommunicationHandler::stopDoor(DoorSide side)
{
	// Only stop doors that are currently actually doing something,
	// and close any opened valves.
	// If this function is called when a door is already stopped,
	// the door is instead restored to its original status.

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
	if (strcmp(receivedMessage, "doorClosing") == 0 || strcmp(receivedMessage, "doorOpening") == 0)
	{
		// Needs to be changed: doorState needs to be saved.
		if (side == left)
		{
			receivedMessage = simulation.sendMessage(DoorLeftStop);
		}
		else // side == right
		{
			receivedMessage = simulation.sendMessage(DoorRightStop);
		}

		return -1;
	}
	else if (strcmp(receivedMessage, "doorClosed") == 0 || strcmp(receivedMessage, "doorLocked") == 0)
	{
		// TODO:
		// - get valve status
		// - save which valves were opened
		// - close any opened ones
		// - check if the messages were received correctly

		return -1;
	}
	else if (strcmp(receivedMessage, "doorStopped") == 0)
	{
		// Door was stopped, restore it to the way it was.

		// TODO: implement this

		return -1;
	}
	else
	{
		// There is nothing to stop.
		return 0;
	}

	// return -2; // This shouldn't even be reachable.
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
	// TODO: make this

	return false;
}

bool CommunicationHandler::valveClose(DoorSide side, int row)
{
	// TODO: make this

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

		char* redLightReceived = simulation.sendMessage(redLightMessage);
		char* greenLightReceived = simulation.sendMessage(greenLightMessage);

		if (strcmp(redLightReceived, "on") == 0 && strcmp(greenLightReceived, "off") == 0)
		{
			lState = redLightOn;
		}
		else if (strcmp(redLightReceived, "off") == 0 && strcmp(greenLightReceived, "on") == 0)
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

