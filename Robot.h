#ifndef ROBOT_H_
#define ROBOT_H_

#include <HamsterAPIClientCPP/Hamster.h>
using namespace HamsterAPI;

class Robot {
private:
	//current location of the robot
	double currX, currY, currYaw;
	// previous location
	double prevX, prevY, prevYaw;
	//the robot
	Hamster *hamster;

public:
	void robotMovement();
	Robot(Hamster *hamster);
	double getDeltaX() const;
	double getDeltaY() const;
	double getDeltaYaw() const;
	void updatePose();
	virtual ~Robot();
};

#endif /* ROBOT_H_ */
