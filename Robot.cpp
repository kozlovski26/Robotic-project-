#include "Robot.h"
using namespace std;

Robot::Robot(Hamster *hamster) :hamster(hamster), prevX(0), prevY(0), prevYaw(0), currX(0), currY(0), currYaw(0) {
	updatePose();
}

double Robot::getDeltaX() const {
	return currX - prevX;
}

double Robot::getDeltaY() const {
	return currY - prevY;
}

double Robot::getDeltaYaw() const {
	return currYaw - prevYaw;
}

void Robot::robotMovement() {

		LidarScan ld = hamster->getLidarScan();

		if (ld.getDistance(180) < 0.4) {
			hamster->sendSpeed(-0.5, 0);
		} else if (ld.getDistance(180) < 0.8) {
			hamster->sendSpeed(0.5, 45);
		}
		else
			hamster->sendSpeed(1.0, 0);

}
void Robot::updatePose() {
	// get the position of the robot
	Pose pose = hamster->getPose();

	prevX = currX;
	prevY = currY;
	prevYaw = currYaw;

	currX = pose.getX();
	currY = pose.getY();
	currYaw = pose.getHeading();
}

Robot::~Robot() {
}

