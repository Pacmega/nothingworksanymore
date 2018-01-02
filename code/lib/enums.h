#ifndef ENUMS_H_
#define ENUMS_H_

enum WaterLevel
{
	low,
	belowValve2,
	aboveValve2,
	aboveValve3,
	high,
	waterError
};

enum DoorState
{
	doorLocked,
	doorClosed,
	doorOpen,
	doorClosing,
	doorOpening,
	doorStopped,
	motorDamage,
	doorStateError
};

enum LightState
{
	redLightOn,
	greenLightOn,
	lightError
};

enum SluiceState
{
	allowingEntryLeft,
	allowingEntryRight,
	allowingExitLeft,
	allowingExitRight,
	sluicingUp,
	sluicingDown
};

enum DoorType
{
	noLock,
	fastLock // Has to be locked fast, otherwise it breaks the motor.
};

enum DoorSide
{
	left,
	right
};

#endif