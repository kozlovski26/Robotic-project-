#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include "Robot.h"
#include "LocalizationManager.h"
#include "Map.h"

using namespace std;
using namespace HamsterAPI;
HamsterAPI::Hamster * hamster;

int main() {
	try {
		hamster = new HamsterAPI::Hamster(1);
		sleep(3);

		Robot robot(hamster);
		OccupancyGrid ogrid = hamster->getSLAMMap();
		Map map(ogrid);
		map.initMap();
		LocalizationManager locManager(ogrid, hamster);

		locManager.initParticles();

		while (hamster->isConnected()) {
			try {
				map.showMap();
				robot.robotMovement();
				robot.updatePose();
				locManager.updateParticles(robot.getDeltaX(), robot.getDeltaY(), robot.getDeltaYaw());

				map.drawParticles(locManager.getParticles());
				sleep(0.5);

			} catch (const HamsterAPI::HamsterError & message_error) {
				HamsterAPI::Log::i("Client", message_error.what());
			}
		}
	} catch (const HamsterAPI::HamsterError & connection_error) {
		HamsterAPI::Log::i("Client", connection_error.what());
	}
	return 0;
}

