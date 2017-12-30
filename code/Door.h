#ifndef DOOR_H_
#define DOOR_H_

class Door
{
public:
	Door();
	~Door();
	
	int allowExit();
	int allowEntry();
	int openDoor();
	int closeDoor();
	int stopDoor();

private:
	bool messageReceived;
	DoorType type;
	DoorSide side;
	DoorMotor motor;

	bool lockDoor();
	bool unlockDoor();
};

#endif