#include <iostream>
#include "lib/enums.h"
#include "CommunicationHandler.h"
#include "TrafficLight.h"

TrafficLight::TrafficLight(CommunicationHandler* existingHandler, int Location)
	: cHandler(existingHandler)
{
	location = Location;
}

TrafficLight::~TrafficLight()
{

}

LightState TrafficLight::getLightState()
{
	// Calls the getLightState function from the Communication Handler,
	// passing it the location of this specific light.
	return cHandler->getLightState(location);
}

int TrafficLight::redLight()
{
	int lightsChanged;

	switch(getLightState())
	{
		case redLightOn:
			lightsChanged = 0; // Done. Nothing to change.
		case greenLightOn:
			messageReceived = cHandler->greenLight(location);
			if (messageReceived)
			{
				messageReceived = cHandler->redLight(location);
				if (messageReceived)
				{
					lightsChanged = 0; // Success
				}
				else
				{
					lightsChanged = -1; // Message was not received by simulator
				}
			}
			else
			{
				lightsChanged = -1; // Message was not received by simulator
			}
		case lightError:
			lightsChanged = -2; // Invalid light state received from simulator
	}

	return lightsChanged;
}

int TrafficLight::greenLight()
{
	int lightsChanged;

	switch(getLightState())
	{
		case greenLightOn:
			// std::cout << "[DBG] GreenLight on " << std::endl;
			lightsChanged = 0; // Done. Nothing to change.
			break;
		case redLightOn:
			// std::cout << "[DBG] GreenLight being turned on" << std::endl;
			switch (cHandler->greenLight(location))
			{
				case 0:
					lightsChanged = 0; // Success
					break;
				case -1:
					lightsChanged = -1; // One of the messages was not received by the simulator
					break;
				case -2:
					lightsChanged = -2; // Invalid light state received from simulator
				default:
					lightsChanged = -1; // Message was not received by simulator
					break;
			}
			break;
		case lightError:
			std::cout << "Hecking what " << std::endl;
			lightsChanged = -2; // Invalid light state received from simulator
			break;
	}

	return lightsChanged;
}