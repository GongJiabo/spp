#include<cmath>
#include"TimeSystem.h"

//通用时到儒略日
void CommonTimeToMJulianDay(const COMMONTIME& ct, JULIANDAY& jd)
{    
	jd.lDay = 1721013.5 + 367 * ct.year - int(7.0 / 4.0*(ct.year + int((ct.month + 9.0) / 12.0))) + ct.day + (ct.hour + ct.minute / 60.0 + ct.second / 3600.0) / 24.0 + int(275.0*ct.month / 9.0);
}

//儒略日到通用时
void MJulianDayToCommonTime(const JULIANDAY& jd, COMMONTIME& ct)
{
	int a, b, c, d, e;
	a = int(jd.lDay + 0.5);
	b = a + 1537;
	c = int((b - 122.1) / 365.25);
	d = int(365.25*c);
	e = int((b - d) / 30.600);
	ct.day = b - d - int(30.6001*e) + jd.lDay + 0.5 - int(jd.lDay + 0.5);
	ct.month = e - 1 - 12 * int(e / 14.0);
	ct.year = c - 4715 - int((7 + ct.month) / 10.0);
}

//GPS时到儒略日
void GPSTimeToMJulianDay(const GPSTIME& gt, JULIANDAY& jd)
{
	jd.lDay = gt.lWeek * 7 + gt.lSecond / 86400.0 + 2444244.5;
}

//儒略日到GPS时
void MJulianDayToGPSTime(const JULIANDAY& jd, GPSTIME& gt)
{
	gt.lWeek = (int)((jd.lDay - 2444244.5) / 7.0);
	gt.lSecond = (jd.lDay - 2444244.5 - gt.lWeek * 7) * 86400;
}

//通用时到GPS时
void CommonTimeToGPSTime(const COMMONTIME& ct, GPSTIME& gt)
{
	JULIANDAY  jday;
	CommonTimeToMJulianDay(ct, jday);
	MJulianDayToGPSTime(jday, gt);
}

//GPS时到通用时
void GPSTimeToCommonTime(const GPSTIME& gt, COMMONTIME& ct)
{
	JULIANDAY  jday;
	GPSTimeToMJulianDay(gt, jday);
	MJulianDayToCommonTime(jday, ct);
}
//通用时到年积日
void CommonTimeToDayofyear(const COMMONTIME& ct, DAYOFYEAR& dt)
{    
	double jyear;
	JULIANDAY jday;
	CommonTimeToMJulianDay(ct, jday);
	jyear = 1721013.5 + 367 * ct.year - int(7.0 / 4.0*(ct.year + int((1 + 9) / 12.0))) + 1 + int(275.0 * 1 / 9.0);
	dt.ddoy = jday.lDay - jyear;
	dt.year = ct.year;
}

//年积日到GPS时及周内天数
void DayofyearToGPSTime(const DAYOFYEAR& dt, GPSTIME& gt)
{   
	JULIANDAY jyear;
	jyear.lDay = 1721013.5 + 367 * dt.year - int(7.0 / 4.0*(dt.year + int((1.0 + 9.0) / 12.0))) + 1.0 + int(275.0 * 1.0 / 9.0);
	jyear.lDay = jyear.lDay + dt.ddoy;
	MJulianDayToGPSTime(jyear, gt);
} 