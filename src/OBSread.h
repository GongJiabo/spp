#ifndef OBSREAD_H_
#define _OBSREAD_H_

#include "TimeSystem.h"
#include<string>
#include<vector>


typedef struct POSITIONDATA
{
	double x;
	double y;
	double z;
}Position;



typedef struct tagOBSERVATTOINDATA
{
	Position        StationPosition;
	double          AerialHight;
	double          AerialEastBias;
	double          AerialNorthBias;
	int             ObserveDataTypeSum;
	std::string     datatype[30];
}tago;

struct OVALUE
{
	int prn;
	double P1;
	double P2;
};

typedef struct OBSERVATIONDATA
{
	COMMONTIME Observetime;
	int      ObserveHealth;
	int      ObserveSatSum;
	int      SatellitePRN[30];
	double   cvrClkBias;        
	OVALUE   ovalue[30];      //观测数据
}OBSERVATION;

extern std::vector<tagOBSERVATTOINDATA> TagoVec;    //存放O文件文件头的容器
extern std::vector<OBSERVATIONDATA> ObsVec;         //存放O文件观测数据的容器

void readOfile();
#endif