#ifndef _TIMESYSTEM_H_
#define _TIMESYSTEM_H_

//  时间系统


// 通用时间
typedef struct COMMONTIME
{
	int	    year;
	int		month;
	double	day;
	double	hour;
	double	minute;
	double  second;
} COMMONTIME;
//儒略日
typedef struct tagJULIANDAY
{
	double     lDay;
}JULIANDAY;
//GPS时
typedef struct tagGPSTIME
{
	double     lWeek;
	double   lSecond;
} GPSTIME;
//年积日
typedef struct tagDAYOFYEAR
{
	double  year;
	double  ddoy;
}DAYOFYEAR;

//通用时到儒略日
void CommonTimeToMJulianDay(const COMMONTIME& ct, JULIANDAY& jd);
//儒略日到GPS时
void MJulianDayToGPSTime(const JULIANDAY& jd, GPSTIME& gt);
//通用时到GPS时
void CommonTimeToGPSTime(const COMMONTIME& ct, GPSTIME& gt);
//GPS时到儒略日
void GPSTimeToMJulianDay(const GPSTIME& gt, JULIANDAY& jd);
//儒略日到通用时
void MJulianDayToCommonTime(const JULIANDAY& jd, COMMONTIME& ct);
//GPS时到通用时
void GPSTimeToCommonTime(const GPSTIME& gt, COMMONTIME& ct);
//通用时到年积日
void CommonTimeToDayofyear(const COMMONTIME& ct, DAYOFYEAR& dt);
//年积日到GPS时及周内天数
void DayofyearToGPSTime(const DAYOFYEAR& dt, GPSTIME& gt);

#endif
