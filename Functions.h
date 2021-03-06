///////////////////////////////
//Glen Fields Function.h
///////////////////////////////

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stack>
#include <queue>
#include <array>
#include <time.h>
#include <iostream>


bool isReady(int rate) //Determines if a plane is ready
{
	// rate/60 gives us the likelihood that the indicated
	//action will happen.
	rate = (int)(((float)rate / 60.0) * 100);

	if (rand() % 100 <= rate) {return true;}
	else {return false;}
}

std::array<int, 4> airportSim(int totalSimTime, int landingTime, int takeoffTime,
	int landingRate, int takeoffRate);
void displayArray(const std::array<int, 4> &array); //displays result of airportSim()
///////////////////////////////////////////////////////////////////////

std::array<int, 4> airportSim(int totalSimTime, int landingTime, int takeoffTime,
	//needed variables
	int landingRate, int takeoffRate){
	srand(time(NULL));
	std::array<int, 4> array;
	std::queue<int> takeoffQueue, landingQueue;
	int takeoffLength = 0, totalTakeoff = 0;
	int landingLength = 0, totalLanding = 0;
	int lT = 0;
	int tT = 0;
	int totalLandingTime = 0, totalTakeoffTime = 0;
	int averageLandingLength = 0, averageTakeoffLength = 0;

	//for begins simulation
	for (int clock = 0; clock < totalSimTime; ++clock){
		if (isReady(takeoffRate)) //takeoff check
		{
			takeoffQueue.push(clock);
			++takeoffLength;
		}

		if (isReady(landingRate)) //landing check
		{
			landingQueue.push(clock);
			++landingLength;
		}

		averageLandingLength += landingQueue.size();
		averageTakeoffLength += takeoffQueue.size();

		//checks if plane can land
		if (!landingQueue.empty() && lT == 0 && tT == 0){
			totalLandingTime += (clock - landingQueue.front());
			landingQueue.pop();
			lT = landingTime;
			continue;
		}
		//checks if plane can takeoff 
		else if (!takeoffQueue.empty() && landingQueue.empty() && lT == 0 && tT == 0){
			totalTakeoffTime += (clock - takeoffQueue.front());
			takeoffQueue.pop();
			tT = takeoffTime;
			continue;
		}
		//next two else-if tell if runway is busy
		else if (lT != 0){
			--lT;
			continue;
		}
		else if (tT != 0){
			--tT;
			continue;
		}

	}
	//next find plane information after sim has ended
	while (!landingQueue.empty())
	{
		totalLandingTime += (totalSimTime - landingQueue.front());
		landingQueue.pop();
	}
	while (!takeoffQueue.empty())
	{
		totalTakeoffTime += (totalSimTime - takeoffQueue.front());
		takeoffQueue.pop();
	}

	//if no planes have arrived, prevents division by zero
	if (landingLength == 0)
	{
		landingLength = 1;
	}
	if (takeoffLength == 0){
		takeoffLength = 1;
	}

	//fills array with proper values
	array[0] = (averageLandingLength / totalSimTime) + 1;
	array[1] = (totalLandingTime / landingLength) + 1;
	array[2] = (averageTakeoffLength / totalSimTime) + 1;
	array[3] = (totalTakeoffTime / takeoffLength) + 1;

	return array;
}


//functions after this are helper functions

void displayArray(const std::array<int, 4> &array){
	std::cout << "Average arriving queue length: " << array[0] << " planes\n";
	std::cout << "Average arriving time spent in queue: " << array[1] << " minutes\n";
	std::cout << "Average leaving queue length: " << array[2] << " planes\n";
	std::cout << "Average leaving time spent in queue: " << array[3] << " minutes" << std::endl;
}

#endif