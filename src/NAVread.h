#include"Timesystem.h"
#include<vector>

#ifndef _Nread_H_
#define _Nread_H_
 
typedef struct  SAPOSITIONDATA
{   
	double prn;
	GPSTIME Sgps;
	double x;
	double y;
	double z;
	double dt_relative;
}SatPosition;

typedef struct tagNAVIGATIONDATA
{
	//
	double	PRN;              /*  卫星的Prn号*/
	COMMONTIME	    TOC;                  /*  clock data reference time(sec) */
	double		ClkBias;              /*  clock correction(sec/sec2) */
	double		ClkDrift;             /*  clock correction(sec/sec) */
	double		ClkDriftRate;         /*  clock correction(sec) */
	//Radio Track 1
	double IODE;           //数据、星历发布时间
	double Crs;            // m
	double Δn;            // rad/s
	double M0;             // rad
	//Radio Track 2
	double Cuc;            // rad
	double e;              // 轨道偏心率
	double Cus;            // radians
	double sqrtA;          // sqrt(m)
	//Radio Track 3
	double Toe;            // GPS周内的秒数
	double Cic;            // rad
	double Ω;             // rad 
	double Cis;            // rad
	//Radio Track 4
	double i0;             // rad
	double Crc;            // m
	double w;              // rad
	double dΩ;            // rad/s
	//Radio Track 5
	double di;             // rad/s
	double L2Channel;      // Code
	double GPSweek;        // s
	double L2PDataFlag;    // data flag
	//Radio Track 6
	double SAccuracy;            // User range accuracy 
	double SHealth;              //  Satellite health 
	double TGD;                  //  grup delay (s)
	double IODC;                 //  clock date issue 
	//Radio Track 7
	double		Senttime;        //  signal sent(s) 
	double		Fitinterval;     //  Fit interval
	double		Spare1;
	double		Spare2;
}NAVIGATION;


extern std::vector<tagNAVIGATIONDATA> NavVec;     //存放广播星历的容器
extern std::vector<SatPosition> SatVec;

void readNfile();  //读取导航电文
void getsatpostion(const std::vector<NAVIGATION> NavVec,int i, double t,double dt);

#endif