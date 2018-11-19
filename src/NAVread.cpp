#include"NAVread.h"
#include"Timesystem.h"
#include<string>
#include<iostream>
#include <iomanip>
#include<fstream>
#include <cstdlib>
#include<vector>

std::vector<NAVIGATION> NavVec;
std::vector<SatPosition> SatVec;

void readNfile()
{   

	NAVIGATION *ndata = new NAVIGATION[500];
	//
	using namespace std;
	ifstream Ninfile;
	Ninfile.open("brdc2860.17n");
	if (!Ninfile.is_open())  // failed to open file
	{
		cout << "Could not open the file " << endl;
		cout << "Program terminating.\n";
		cin.get();    // keep window open
		exit(EXIT_FAILURE);
	}

	//读到头文件尾
	string begin, line;
	//广播星历个数
	int n_num=0;    
	//
	getline(Ninfile, begin);
	while (begin.substr(60)!="END OF HEADER       ")
	{
		getline(Ninfile, begin);
	}	
	//

	//
	do
	{    
		//PRN号，历元，卫星钟参数
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].PRN = stod(line.substr(0, 2));
		ndata[n_num].TOC.year = stoi(line.substr(2, 3)) + 2000;
		ndata[n_num].TOC.month = stoi(line.substr(5, 3));
		ndata[n_num].TOC.day = stod(line.substr(8, 3));
		ndata[n_num].TOC.hour = stod(line.substr(11, 3));
		ndata[n_num].TOC.minute = stod(line.substr(14, 3));
		ndata[n_num].TOC.second = stod(line.substr(17, 5));
		ndata[n_num].ClkBias = stod(line.substr(22, 19));
		ndata[n_num].ClkDrift = stod(line.substr(41, 19));
		ndata[n_num].ClkDriftRate = stod(line.substr(60, 19));
		//广播轨道1
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(18, 1, "E");
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].IODE = stod(line.substr(3, 19));
		ndata[n_num].Crs = stod(line.substr(22, 19));
		ndata[n_num].Δn = stod(line.substr(41, 19));
		ndata[n_num].M0 = stod(line.substr(60, 19));
		//广播频道2
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(18, 1, "E");
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].Cuc = stod(line.substr(3, 19));
		ndata[n_num].e = stod(line.substr(22, 19));
		ndata[n_num].Cus = stod(line.substr(41, 19));
		ndata[n_num].sqrtA = stod(line.substr(60, 19));
		//广播频道3
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(18, 1, "E");
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].Toe = stod(line.substr(3, 19));
		ndata[n_num].Cic = stod(line.substr(22, 19));
		ndata[n_num].Ω = stod(line.substr(41, 19));
		ndata[n_num].Cis = stod(line.substr(60, 19));
		//广播频道4
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(18, 1, "E");
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].i0 = stod(line.substr(3, 19));
		ndata[n_num].Crc = stod(line.substr(22, 19));
		ndata[n_num].w = stod(line.substr(41, 19));
		ndata[n_num].dΩ = stod(line.substr(60, 19));
		//广播频道5
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(18, 1, "E");
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].di = stod(line.substr(3, 19));
		ndata[n_num].L2Channel = stod(line.substr(22, 19));
		ndata[n_num].GPSweek = stod(line.substr(41, 19));
		ndata[n_num].L2PDataFlag = stod(line.substr(60, 19));
		//广播频道6
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(18, 1, "E");
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].SAccuracy = stod(line.substr(3, 19));
		ndata[n_num].SHealth = stod(line.substr(22, 19));
		ndata[n_num].TGD = stod(line.substr(41, 19));
		ndata[n_num].IODC = stod(line.substr(60, 19));
		//广播频道7
		getline(Ninfile, line);
		if (Ninfile.eof())
			break;
		line.replace(18, 1, "E");
		line.replace(37, 1, "E");
		line.replace(56, 1, "E");
		line.replace(75, 1, "E");
		ndata[n_num].Senttime = stod(line.substr(3, 19));
		ndata[n_num].Fitinterval = stod(line.substr(22, 19));
		ndata[n_num].Spare1 = stod(line.substr(41, 19));
		ndata[n_num].Spare2 = stod(line.substr(60, 19));
		NavVec.push_back(ndata[n_num]);
		n_num++;		
	} while (1);
	//
	cout << "Number of Navgation data:" << n_num << endl;
	Ninfile.close();
	delete[]ndata;
}


void getsatpostion(const std::vector<NAVIGATION> NavVec, int i, double t, double dt)  //i为第几个星历 t为所求时间 dt为地球自转改正所用的时间差
{   
	using namespace std;
	SatPosition Postion;
	const double GM = 3.986005e14;
	const double we = 7.2921151467e-5;
  
		//计算相对于星历参考历元的时间
		double tk;
		tk = t - NavVec[i].Toe;
		if (tk>302400)
			tk = tk - 604800;
		else if (tk < -302400)
			tk = tk + 604800;
		else
			tk = tk;

		//计算卫星运动的平均角速度n
		double n0, n;
		n0 = sqrt(GM) / (pow(NavVec[i].sqrtA, 3));
		n = n0 + NavVec[i].Δn;

		//计算观测瞬间卫星的平近点角M
		double M;
		M = NavVec[i].M0 + n*tk;

		//计算偏近点角
		double E, E0, dE;
		E0 = M;
		do
		{
			E = M + NavVec[i].e * sin(E0);
			dE = abs(E - E0);
			E0 = E;
		} while (dE > 1e-10);
		while (E < 0)
			E += atan(1.0) * 8;
		//计算真近点角
		double cosf, sinf;
		cosf = (cos(E) - NavVec[i].e) / (1.0 - NavVec[i].e*cos(E));
		sinf = sqrt(1.0 - NavVec[i].e*NavVec[i].e)*sin(E) / (1.0 - NavVec[i].e*cos(E));

		//计算升交角u_
		double sinu_, cosu_, sin2u_, cos2u_;
		sinu_ = sin(NavVec[i].w)*cosf + cos(NavVec[i].w)*sinf;
		cosu_ = cos(NavVec[i].w)*cosf - sin(NavVec[i].w)*sinf;
		sin2u_ = 2 * sinu_*cosu_;
		cos2u_ = cosu_*cosu_ - sinu_*sinu_;

		//计算摄动改正向
		double δu, δr, δi;
		δu = NavVec[i].Cuc*cos2u_ + NavVec[i].Cus*sin2u_;
		δr = NavVec[i].Crc*cos2u_+ NavVec[i].Crs*sin2u_;
		δi = NavVec[i].Cic*cos2u_ + NavVec[i].Cis*sin2u_;

		//对u_、r_、i0进行改正
		double  r, ii;
		r = NavVec[i].sqrtA*NavVec[i].sqrtA*(1 - NavVec[i].e*cos(E)) + δr;
		ii = NavVec[i].i0 + δi + NavVec[i].di*tk;

		//计算卫星在轨道面坐标系中的位置
		double x, y;
		x = r*(cosu_*cos(δu) - sinu_*sin(δu));
		y = r*(cosu_*sin(δu) + cos(δu)*sinu_);

		//计算观测瞬间升交点的经度L
		double  Ω0, L;
		Ω0 = NavVec[i].Ω;
		L = Ω0 + (NavVec[i].dΩ - we)*tk - we*NavVec[i].Toe;

		//计算卫星在瞬时地球坐标系中的位置
		double X, Y, Z;
		X = x*cos(L) - y*cos(ii)*sin(L);
		Y = x*sin(L) + y*cos(ii)*cos(L);
		Z = y*sin(ii);

		//地球自转改正
		double a = we*dt;

		//计算位置
		Postion.prn = NavVec[i].PRN;
		Postion.Sgps.lSecond = NavVec[i].Toe;
		Postion.x = cos(a)*X + sin(a)*Y;
		Postion.y = -sin(a)*X + cos(a)*Y;
		Postion.z = Z;
		//相对论改正
		Postion.dt_relative = -4.442807633e-10*NavVec[i].e*NavVec[i].sqrtA*sin(E);

		//存入容器
		SatVec.push_back(Postion);
}

 