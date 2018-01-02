#ifndef COMMANDS_H_
#define COMMANDS_H_ 

static const char DoorLeftOpen[] = "SetDoorLeft:open;\0";
static const char DoorLeftClose[] = "SetDoorLeft:close;\0";
static const char DoorLeftStop[] = "SetDoorLeft:stop;\0";

static const char DoorRightOpen[] = "SetDoorRight:open;\0";
static const char DoorRightClose[] = "SetDoorRight:close;\0";
static const char DoorRightStop[] = "SetDoorRight:stop;\0";

static const char GetDoorLeft[] = "GetDoorLeft;\0";
static const char GetDoorRight[] = "GetDoorRight;\0";

// Open left door valves
static const char DoorLeftOpenBottomValve[] = "SetDoorLeftValve1:open;\0";
static const char DoorLeftOpenMiddleValve[] = "SetDoorLeftValve2:open;\0";
static const char DoorLeftOpenTopValve[] = "SetDoorLeftValve3:open;\0";

// Close left door valves
static const char DoorLeftCloseBottomValve[] = "SetDoorLeftValve1:close;\0";
static const char DoorLeftCloseMiddleValve[] = "SetDoorLeftValve2:close;\0";
static const char DoorLeftCloseTopValve[] = "SetDoorLeftValve3:close;\0";

// Open right door valves
static const char DoorRightOpenBottomValve[] = "SetDoorRightValve1:open;\0";
static const char DoorRightOpenMiddleValve[] = "SetDoorRightValve2:open;\0";
static const char DoorRightOpenTopValve[] = "SetDoorRightValve3:open;\0";

// Close right door valves
static const char DoorRightCloseBottomValve[] = "SetDoorRightValve1:close;\0";
static const char DoorRightCloseMiddleValve[] = "SetDoorRightValve2:close;\0";
static const char DoorRightCloseTopValve[] = "SetDoorRightValve3:close;\0";

// Get left door valve states
static const char DoorLeftGetBottomValve[] = "GetDoorLeftValve1;\0";
static const char DoorLeftGetMiddleValve[] = "GetDoorLeftValve2;\0";
static const char DoorLeftGetTopValve[] = "GetDoorLeftValve3;\0";

// Get right door valve states
static const char DoorRightGetBottomValve[] = "GetDoorRightValve1;\0";
static const char DoorRightGetMiddleValve[] = "GetDoorRightValve2;\0";
static const char DoorRightGetTopValve[] = "GetDoorRightValve3;\0";

// Enable red traffic lights
static const char TrafficLight1RedOn[] = "SetTrafficLight1Red:on;\0";
static const char TrafficLight2RedOn[] = "SetTrafficLight2Red:on;\0";
static const char TrafficLight3RedOn[] = "SetTrafficLight3Red:on;\0";
static const char TrafficLight4RedOn[] = "SetTrafficLight4Red:on;\0";

// Disable red traffic lights
static const char TrafficLight1RedOff[] = "SetTrafficLight1Red:off;\0";
static const char TrafficLight2RedOff[] = "SetTrafficLight2Red:off;\0";
static const char TrafficLight3RedOff[] = "SetTrafficLight3Red:off;\0";
static const char TrafficLight4RedOff[] = "SetTrafficLight4Red:off;\0";

// Enable green traffic lights
static const char TrafficLight1GreenOn[] = "SetTrafficLight1Green:on;\0";
static const char TrafficLight2GreenOn[] = "SetTrafficLight2Green:on;\0";
static const char TrafficLight3GreenOn[] = "SetTrafficLight3Green:on;\0";
static const char TrafficLight4GreenOn[] = "SetTrafficLight4Green:on;\0";

// Disable green traffic lights
static const char TrafficLight1GreenOff[] = "SetTrafficLight1Green:off;\0";
static const char TrafficLight2GreenOff[] = "SetTrafficLight2Green:off;\0";
static const char TrafficLight3GreenOff[] = "SetTrafficLight3Green:off;\0";
static const char TrafficLight4GreenOff[] = "SetTrafficLight4Green:off;\0";

// Get red traffic light status
static const char TrafficLight1RedStatus[] = "GetTrafficLight1Red;\0";
static const char TrafficLight2RedStatus[] = "GetTrafficLight2Red;\0";
static const char TrafficLight3RedStatus[] = "GetTrafficLight3Red;\0";
static const char TrafficLight4RedStatus[] = "GetTrafficLight4Red;\0";

// Get green traffic light status
static const char TrafficLight1GreenStatus[] = "GetTrafficLight1Green;\0";
static const char TrafficLight2GreenStatus[] = "GetTrafficLight2Green;\0";
static const char TrafficLight3GreenStatus[] = "GetTrafficLight3Green;\0";
static const char TrafficLight4GreenStatus[] = "GetTrafficLight4Green;\0";

static const char GetWaterLevel[] = "GetWaterLevel;\0";

static const char DoorLeftLock[] = "SetDoorLockLeft:on;\0";
static const char DoorLeftUnlock[] = "SetDoorLockLeft:off;\0";

static const char DoorRightLock[] = "SetDoorLockRight:on;\0";
static const char DoorRightUnlock[] = "SetDoorLockRight:off;\0";

static const char DoorLeftLockState[] = "GetDoorLockStateLeft;\0";
static const char DoorRightLockState[] = "GetDoorLockStateRight;\0";

#endif