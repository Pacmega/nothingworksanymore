#ifndef ENUMS_H_
#define ENUMS_H_

enum waterLevel
{
	low,
	belowValve2,
	aboveValve2,
	aboveValve3,
	high,
	waterError
};

enum doorState
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

enum doorType
{
	noLock,
	fastLock, // Has to be locked fast, otherwise it breaks the motor.
	doorTypeError
};

enum motorType
{
	continuous,
	pulse,
	motorError
};

enum doorSide
{
	left,
	right
};

#endif