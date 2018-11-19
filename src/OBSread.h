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
	OVALUE   ovalue[30];      //�۲�����
}OBSERVATION;

extern std::vector<tagOBSERVATTOINDATA> TagoVec;    //���O�ļ��ļ�ͷ������
extern std::vector<OBSERVATIONDATA> ObsVec;         //���O�ļ��۲����ݵ�����

void readOfile();
#endif