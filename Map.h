#ifndef MAP_H_
#define MAP_H_

#include <HamsterAPIClientCPP/Hamster.h>
#include "Particle.h"
#include <vector>
using namespace std;
using namespace HamsterAPI;

class Map {
private:
	cv::Mat matrix;
	OccupancyGrid &ogrid;

public:
	Map(OccupancyGrid &ogrid);
	void showMap();
	void initMap();
	void drawParticles(vector<Particle *> particles);
	virtual ~Map();
};

#endif /* MAP_H_ */
