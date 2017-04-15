#ifndef LOCALIZATIONMANAGER_H_
#define LOCALIZATIONMANAGER_H_

#include "Particle.h"
#include <vector>
#include <HamsterAPIClientCPP/Hamster.h>
#include <iostream>
#include <cmath>

#define PARTICLES_NUM 500
#define DEL_PARTICLE 200
#define RADIUS 8
#define ROULETTE 5
#define TRASHOLD 0.5

using namespace std;
using namespace HamsterAPI;

class LocalizationManager {
private:
	vector<Particle *> particlesVector;
	const OccupancyGrid &ogrid;//map
	Hamster *hamster;
	void getRandomLocation(Particle *particle);
	double computeBelief(Particle *particle);

	struct myclass //comparator for the sort
	{
		bool operator()(Particle * par1 , Particle * par2){
			return (par1->belief < par2->belief);
		}
	}lessThanBelief;

public:
	void getParticleIfCellNotFree(Particle *particle);
	int rouletteWheel();
	void getRandomNearGoodParticles();
	LocalizationManager(const OccupancyGrid &ogrid, Hamster *hamster);
	void initParticles();
	void updateParticles(double deltaX, double deltaY, double deltaYaw);

	void printParticles() const;
	vector<Particle *> getParticles() const;

	virtual ~LocalizationManager();
};

#endif /* LOCALIZATIONMANAGER_H_ */
