#ifndef _CALCULATE_SATPOSTION_H_
#define _CALCULATE_SATPOSTION_H_

#include<cmath>
#include<Eigen/Dense>  
#include"NAVread.h"
#include"OBSread.h"
struct STATIONPOSTION
{
	double X;
	double Y;
	double Z;
	double Vtr;
	double m0;
};

using Eigen::MatrixXd; 

void IterationSat();
void ToResult();
void ToENU();

extern std::vector<STATIONPOSTION> StationVec;
#endif