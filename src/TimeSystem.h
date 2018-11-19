#ifndef _TIMESYSTEM_H_
#define _TIMESYSTEM_H_

//  ʱ��ϵͳ


// ͨ��ʱ��
typedef struct COMMONTIME
{
	int	    year;
	int		month;
	double	day;
	double	hour;
	double	minute;
	double  second;
} COMMONTIME;
//������
typedef struct tagJULIANDAY
{
	double     lDay;
}JULIANDAY;
//GPSʱ
typedef struct tagGPSTIME
{
	double     lWeek;
	double   lSecond;
} GPSTIME;
//�����
typedef struct tagDAYOFYEAR
{
	double  year;
	double  ddoy;
}DAYOFYEAR;

//ͨ��ʱ��������
void CommonTimeToMJulianDay(const COMMONTIME& ct, JULIANDAY& jd);
//�����յ�GPSʱ
void MJulianDayToGPSTime(const JULIANDAY& jd, GPSTIME& gt);
//ͨ��ʱ��GPSʱ
void CommonTimeToGPSTime(const COMMONTIME& ct, GPSTIME& gt);
//GPSʱ��������
void GPSTimeToMJulianDay(const GPSTIME& gt, JULIANDAY& jd);
//�����յ�ͨ��ʱ
void MJulianDayToCommonTime(const JULIANDAY& jd, COMMONTIME& ct);
//GPSʱ��ͨ��ʱ
void GPSTimeToCommonTime(const GPSTIME& gt, COMMONTIME& ct);
//ͨ��ʱ�������
void CommonTimeToDayofyear(const COMMONTIME& ct, DAYOFYEAR& dt);
//����յ�GPSʱ����������
void DayofyearToGPSTime(const DAYOFYEAR& dt, GPSTIME& gt);

#endif
