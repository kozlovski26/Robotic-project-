#include "Map.h"

Map::Map(OccupancyGrid &ogrid) : ogrid(ogrid), matrix(ogrid.getWidth(), ogrid.getHeight(), CV_8UC3, cv::Scalar(255, 255, 255)) {
	cv::namedWindow("Map");
}

void Map::initMap() {
	unsigned char pixel;
	for (int i = 250; i < ogrid.getWidth()-350; i++) {
		for (int j = 250; j < ogrid.getHeight()-350; j++) {

			if (ogrid.getCell(i, j) == CELL_FREE)
			{
				pixel = 255;//white
			}
			else if (ogrid.getCell(i, j) == CELL_OCCUPIED)
			{
				pixel = 0;//black
			}
			else if (ogrid.getCell(i, j) == CELL_UNKNOWN)
				pixel = 128;//grey

			matrix.at<cv::Vec3b>(2*(i-250),2*(j-250))[0] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250),2*(j-250))[1] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250),2*(j-250))[2] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250)+1,2*(j-250))[0] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250)+1,2*(j-250))[1] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250)+1,2*(j-250))[2] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250),2*(j-250)+1)[0] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250),2*(j-250)+1)[1] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250),2*(j-250)+1)[2] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250)+1,2*(j-250)+1)[0] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250)+1,2*(j-250)+1)[1] = pixel;
			matrix.at<cv::Vec3b>(2*(i-250)+1,2*(j-250)+1)[2] = pixel;
		}
	}

}

void Map::drawParticles(vector<Particle *> particles) {
	initMap();

	int size = particles.size();
	int best = size * 0.5;
	int i = 0;

	for (; i < size - best; i++) {//draw the low-belief half in red color
		int x = particles[i]->i;
		int y = particles[i]->j;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250))[0] = 0;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250))[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250))[2] = 255;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250))[0] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250))[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250))[2] = 255;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250)+1)[0] = 0;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250)+1)[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250)+1)[2] = 255;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250)+1)[0] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250)+1)[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250)+1)[2] = 255;
	}

	for (; i < size; i++) {//draw the high-belief half in blue color
		int x = particles[i]->i;
		int y = particles[i]->j;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250))[0] = 255;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250))[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250))[2] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250))[0] = 255;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250))[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250))[2] = 0;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250)+1)[0] = 255;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250)+1)[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250),2*(y-250)+1)[2] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250)+1)[0] = 255;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250)+1)[1] = 0;
		matrix.at<cv::Vec3b>(2*(x-250)+1,2*(y-250)+1)[2] = 0;
	}
}


void Map::showMap() {
	// Show the matrix on the window
	cv::imshow("Map", matrix);

	// Delay for 1 millisecond to allow the window to process
	// incoming events
	cv::waitKey(1);
}

Map::~Map() {
}

