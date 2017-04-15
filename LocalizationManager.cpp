#include "LocalizationManager.h"
using namespace std;

LocalizationManager::LocalizationManager(const OccupancyGrid &ogrid, Hamster *hamster) : ogrid(ogrid), hamster(hamster) {
	cout << "width: " << ogrid.getWidth() << ", height: " << ogrid.getHeight() << endl;
}

//initialize the particle vector
void LocalizationManager::initParticles() {
	particlesVector.resize(PARTICLES_NUM);

	for (int i = 0; i < particlesVector.size(); i++) {
		particlesVector[i] = new Particle();
		getRandomLocation(particlesVector[i]);
	}
}

void LocalizationManager::getRandomLocation(Particle *particle) {
	do {
		particle->j = rand() % ogrid.getWidth();
		particle->i = rand() % ogrid.getHeight();
	} while (ogrid.getCell(particle->i, particle->j) != CELL_FREE);

	particle->x = (particle->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution() ;
	particle->y = ((double)ogrid.getHeight() / 2 - particle->i) * ogrid.getResolution();

	particle->yaw = rand() % 360;
}

double LocalizationManager::computeBelief(Particle *particle) {
	LidarScan scan = hamster->getLidarScan();

	int hits = 0;
	int misses = 0;

	for (int i = 0; i < scan.getScanSize(); i++) {
		double angle = scan.getScanAngleIncrement() * i * DEG2RAD;// the zavit of the robot

		//ignore the distances that out of the laser max range
		if (scan.getDistance(i) < scan.getMaxRange() - 0.001) {
			// calculate the obstacle pos for each degree
			double obsX = particle->x + scan.getDistance(i) * cos(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);
			double obsY = particle->y + scan.getDistance(i) * sin(angle + particle->yaw * DEG2RAD - 180 * DEG2RAD);

			int pixelsY = ((double)ogrid.getHeight() / 2 -(double)( obsY / ogrid.getResolution()) );
			int pixelsX = ((double)(obsX / ogrid.getResolution()) + (double)ogrid.getWidth() / 2);

			if (ogrid.getCell(pixelsY, pixelsX) == CELL_OCCUPIED) {
				hits++;
			} else {
				misses++;
			}
		}
	}
	return (float)hits / (hits + misses);
}

void LocalizationManager::updateParticles(double deltaX, double deltaY, double deltaYaw) {
	for (int i = 0; i < particlesVector.size(); i++) {
		Particle *particle = particlesVector[i];

		particle->x += deltaX ;
		particle->y += deltaY ;
		particle->yaw += deltaYaw;
		particle->i = (double)ogrid.getHeight() / 2 - particle->y / ogrid.getResolution() ;
		particle->j = particle->x / ogrid.getResolution() + ogrid.getWidth() / 2;
		particle->belief = computeBelief(particle);


		if (ogrid.getCell(particle->i, particle->j) != CELL_FREE) {
			if (particlesVector[i]->belief < TRASHOLD ) {
				getRandomLocation(particle);
			}
		}
	}
	getRandomNearGoodParticles();
}

void LocalizationManager::getRandomNearGoodParticles() {

	std::sort(this->particlesVector.begin(),this->particlesVector.end(),lessThanBelief);

	int j=0;

	for (int i = PARTICLES_NUM-1 ;i > PARTICLES_NUM -1 - DEL_PARTICLE ; i--) {
		int roulette = rouletteWheel();
		do{
			int angle = rand() % 360;
			int radius = rand() % RADIUS;
			particlesVector[j]->j = (particlesVector[roulette]->j + radius * cos (angle));
			particlesVector[j]->i = (particlesVector[roulette]->i + radius * sin (angle));

		} while (ogrid.getCell(particlesVector[j]->i, particlesVector[j]->j) != CELL_FREE);

		particlesVector[j]-> yaw = rand() % 360;
		particlesVector[j]-> x = (particlesVector[j]->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution();
		particlesVector[j]-> y = ((double)ogrid.getHeight() / 2 - particlesVector[j]->i) * ogrid.getResolution();
		particlesVector[j]->belief = computeBelief(particlesVector[j]);
		j++;
	}

}

int LocalizationManager::rouletteWheel() {

	double wheel_location =0;
	double total_weight=0;
	double curr_sum;
	int index=0;

	for (int i = 1 ; i<ROULETTE ; i++) {
		total_weight += particlesVector[PARTICLES_NUM-i]->belief;
	}

	//the selected belief value stored in wheel_location
	wheel_location = (double) (rand() / (double) (RAND_MAX)) *total_weight;
	index=PARTICLES_NUM-1;
	curr_sum = particlesVector[index]->belief;

	//find the index of the selected particle
	while (curr_sum < wheel_location && index > PARTICLES_NUM - 1 - ROULETTE ) {
		index--;
		curr_sum = curr_sum + particlesVector[index]->belief;
	}
	return index;
}

void LocalizationManager::getParticleIfCellNotFree(Particle *particle) {
	do{

		int angle = rand() % 360;
		int radius = rand() % RADIUS;
		particle->j = (particle->j + radius * cos (angle));
		particle->i = (particle->i + radius * sin (angle));

	} while (ogrid.getCell(particle->i, particle->j) != CELL_FREE);
	particle-> yaw = rand() % 360;
	particle-> x = (particle->j - (double)ogrid.getWidth() / 2) * ogrid.getResolution();
	particle-> y = ((double)ogrid.getHeight() / 2 - particle->i) * ogrid.getResolution();
	particle->belief = computeBelief(particle);

}

void LocalizationManager::printParticles() const {
	for (int i = 0; i < particlesVector.size(); i++) {
		Particle *particle = particlesVector[i];
		cout << "Particle " << i << ": " << particle->x << "," << particle->y << ", belief: " << particle->belief << endl;
	}
}

vector<Particle *> LocalizationManager::getParticles() const {
	return particlesVector;
}

LocalizationManager::~LocalizationManager() {
}

