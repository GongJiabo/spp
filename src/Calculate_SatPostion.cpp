#include"Calculate_SatPostion.h"
#include<iostream>
#include<cmath>
#include<iomanip>
#include<fstream>
#include<string>

std::vector<STATIONPOSTION> StationVec;
int space_sum = 0;

void IterationSat()
{
	using namespace std;
	//�������õ������Ǻ�
	ofstream sat_num_file;
	sat_num_file.open("Number.txt");
	sat_num_file.setf(ios::fixed);
	sat_num_file << fixed << std::setprecision(10);
	//
	STATIONPOSTION *StationPostion = new STATIONPOSTION[3000];
	using namespace std;
	const double f1 = 1575.42;
	const double f2 = 1227.6;

	//�����ʽΪС�����8λ
	cout.setf(ios::fixed);
	cout << fixed << std::setprecision(8);
	//
	double  X0, Y0, Z0,cVtr;
	cVtr = 0;

	//X0,Y0,Z0Ϊ���ջ���������
	X0 = TagoVec[0].StationPosition.x;
	Y0 = TagoVec[0].StationPosition.y;
	Z0 = TagoVec[0].StationPosition.z;
	
	for (int g = 0; g < ObsVec.size(); g++)
	{
		GPSTIME gpstime;
		//���۲���Ԫת��ΪGPSʱ
		CommonTimeToGPSTime(ObsVec[g].Observetime, gpstime);

		//��������(���ƽ��) M��(�۲�������)
		int M, N;
		int Snum, SNUM[20], R_[20];
		M = ObsVec[g].ObserveSatSum;
		N = 4;
		double R0, l, m, n;
		double dt;
		double Vts;

		MatrixXd B(M, N), L(M, 1), X(4, 1), dX(4, 1), P(M, M);

		//��ʼ����λȨ��
		for (int ii = 0; ii < M; ii++)
		{
			for (int jj = 0; jj < M; jj++)
				P(ii, jj) = 0;
		}
		for (int ii = 0; ii < M; ii++)
			P(ii, ii) = 1;

        
		// Ѱ������ĵ�������	
		for (int cir = 0; cir < M; cir++)
		{
			////�������P1����P2�۲�ֵΪ0�����ù۲�ֵ��Ȩֵ��Ϊ0
			if (ObsVec[g].ovalue[cir].P1 == 0 || ObsVec[g].ovalue[cir].P2 == 0)
			{
				P(cir, cir) = 0;
			}

			//�۲�����prn��
			int prn;
			prn = ObsVec[g].ovalue[cir].prn;
			//Ѱ�Ҹù۲���Ԫ��ÿ�����Ƕ�Ӧ����ĵ�������
			dt = 604800;
			for (int sat = 0; sat < NavVec.size(); sat++)
			{
				if (NavVec[sat].PRN == prn)
				{
					if (abs(gpstime.lSecond - NavVec[sat].Toe) <= abs(dt))
					{	
						dt = (gpstime.lSecond) - NavVec[sat].Toe;
						Snum = sat;
					}
				}
			}
			SNUM[cir] = Snum;  //ÿ�����Ƕ�Ӧ��������	
			sat_num_file << Snum << "  ";
		}
		sat_num_file << endl;

		//------------------------------------//
		//����ƽ��
		double tk, tk_, tk_2;
		double tkk[20], DT[20];
		tk = gpstime.lSecond;
		tk_ = tk;
		do
		{
			//�����źŵķ���ʱ��tk_
			for (int cir = 0; cir < M; cir++)
			{
				do
				{
					SatVec.clear();
					getsatpostion(NavVec, SNUM[cir], tk_, tk_ - tk);
					R_[cir] = sqrt((X0 - SatVec[0].x)*(X0 - SatVec[0].x) + (Y0 - SatVec[0].y)*(Y0 - SatVec[0].y) + (Z0 - SatVec[0].z)*(Z0 - SatVec[0].z));
					tk_2 = tk_;
					tk_ = tk + cVtr / 2.99792458e8 - R_[cir] / 2.99792458e8;
					//tk_ = tk - R_[cir] / 2.99792458e8;	
				} while (abs(tk_2 - tk_) > 1.0e-8);
				tkk[cir] = tk_;
				DT[cir] = tk - tk_;      //ÿ��������۲�ʱ���ʱ���
			}
 
 
			//���������������źŷ���ʱ�̴���ƽ��
			for (int cir = 0; cir < M; cir++)
			{
				SatVec.clear();
				getsatpostion(NavVec, SNUM[cir], tkk[cir], DT[cir]);
				Vts = -(NavVec[SNUM[cir]].ClkBias + NavVec[SNUM[cir]].ClkDrift*DT[cir] + NavVec[SNUM[cir]].ClkDriftRate*DT[cir] * DT[cir] + SatVec[0].dt_relative);
				R0 = sqrt((X0 - SatVec[0].x)*(X0 - SatVec[0].x) + (Y0 - SatVec[0].y)*(Y0 - SatVec[0].y) + (Z0 - SatVec[0].z)*(Z0 - SatVec[0].z));
				R_[cir] = R0;
				l = (X0 - SatVec[0].x) / R0;
				m = (Y0 - SatVec[0].y) / R0;
				n = (Z0 - SatVec[0].z) / R0;
				B(cir, 0) = l;
				B(cir, 1) = m;
				B(cir, 2) = n;
				B(cir, 3) = -1;
				L(cir, 0) = (f1*f1*ObsVec[g].ovalue[cir].P1 / (f1*f1 - f2*f2) - f2*f2* ObsVec[g].ovalue[cir].P2 / (f1*f1 - f2*f2)) - R0 - 2.99792458e8*Vts;
			}
			dX = ((B.transpose()*P*B).inverse())*B.transpose()*P*L;
			X(0, 0) = X0;
			X(1, 0) = Y0;
			X(2, 0) = Z0;
			X(3, 0) = 0;
			X = X + dX;
			X0 = X(0, 0);
			Y0 = X(1, 0);
			Z0 = X(2, 0);
			cVtr = X(3, 0);
		} while ((dX(0, 0)*dX(0, 0) + dX(1, 0)*dX(1, 0) + dX(2, 0)*dX(2, 0)) > 1.0e-7);

		//--------------------------------------------------------//
		//������
		//cout << "��վ����X��" << X(0, 0) << endl;
		//cout << "��վ����Y��" << X(1, 0) << endl;
		//cout << "��վ����Z��" << X(2, 0) << endl;
		//cout << "��վ��վ���ջ��ֲ" << X(3, 0) / 2.99792458e8 << "s" << endl;
		double m0, mpos;  //��λȨ����
		MatrixXd Q, D, V;
		V = B*dX - L;
		m0 = sqrt((V.transpose()*P*V)(0, 0) / (ObsVec[g].ObserveSatSum - 4));
		//cout << "��λȨ���" << m0 << endl;
		//Q = (B.transpose()*P*B).inverse();
		//D = m0*m0*Q;
		//mpos = m0*sqrt(Q(0, 0) + Q(1, 1) + Q(2, 2) + Q(3, 3));
		//cout << "��վλ�÷��" << mpos <<"   ��"<<g<<"����	��"<<endl << endl;

	    //�������������
		StationPostion[g].m0 = m0;
		StationPostion[g].X = X(0,0);
		StationPostion[g].Y = X(1, 0);
		StationPostion[g].Z = X(2, 0);
		StationPostion[g].Vtr = X(3, 0) / 2.99792458e8;
		StationVec.push_back(StationPostion[g]);
	}
	delete[]StationPostion;
}

void ToResult()
{   
	using namespace std;
	double X_sum = 0, X_aver = 0, Y_sum = 0, Y_aver = 0, Z_sum = 0, Z_aver = 0;
	int Station_num = StationVec.size();
	ofstream ofile;
	ofile.open("Xyz.txt");
	ofile.setf(ios::fixed);
	ofile << fixed << std::setprecision(10);
	for (int num = 0; num < Station_num; num++)
	{
		X_sum = X_sum + StationVec[num].X;
		Y_sum = Y_sum + StationVec[num].Y;
		Z_sum = Z_sum + StationVec[num].Z;
		ofile << num << "  " << StationVec[num].X << "  " << StationVec[num].Y << "  " << StationVec[num].Z << "  " << StationVec[num].m0 << endl;
	}
	ofile.close();


	X_aver = X_sum / Station_num;
	Y_aver = Y_sum / Station_num;
	Z_aver = Z_sum / Station_num;
	ofile.open("Average.txt");
	ofile << X_aver << "  " << Y_aver << "  " << Z_aver;
	ofile.close();
}

void ToENU()
{   
	using namespace std;
	const double ee = 0.00669437999013;
	const double a = 6378137;
	double X0, Y0, Z0, B0, B, L, H;
	double R, ��, W, N;
	//
	ifstream Averfile;
	Averfile.open("Average.txt");
	Averfile >> X0 >> Y0 >> Z0;
	Averfile.close();
	//���㸨����
	R = sqrt(X0*X0 + Y0*Y0 + Z0*Z0);
	�� = atan(Z0 / sqrt(X0*X0 + Y0*Y0));
    //����L
	L = atan(Y0 / X0);
	//��������γ��B
	B = atan(Z0 / sqrt(X0*X0 + Y0*Y0));
	do
	{
		B0 = B;
		W = sqrt(1 - ee*sin(B0)*sin(B0));
		B = atan(tan(��)*(1 + a*ee*sin(B0) / (Z0*W)));
	} while (abs(B - B0) > 1e-10);

	N = a / W;
	H = R*cos(��) / cos(B) - N;
    //���ENU����ϲ�
	ofstream enufile;
	enufile.open("Enu.txt");
	enufile.setf(ios::fixed);
	enufile << fixed << std::setprecision(10);
	double dN, dE, dU, dx, dy, dz;
	for (int i = 0; i < StationVec.size(); i++)
	{
		dx = StationVec[i].X - X0;
		dy = StationVec[i].Y - Y0;
		dz = StationVec[i].Z - Z0;
		dN = -sin(B)*cos(L)*dx - sin(B)*sin(L)*dy + cos(B)*dz;
		dE = -sin(L)*dx + cos(L)*dy;
		dU = cos(B)*cos(L)*dx + cos(B)*sin(L)*dy + sin(B)*dz;
		enufile << i << "  " << dE << "  " << dN << "  " << dU << endl;
	}
	enufile.close();

	//�۲���Ԫ��Ӧ����λ���ļ�
	ofstream stfile;
	stfile.open("Sat_Postion.txt");
	stfile.setf(ios::fixed);
	stfile << fixed << std::setprecision(10);
	
	//��������Ǹ߶Ƚ�
	ofstream gdfile;
	gdfile.open("Gaodujiao.txt");
	gdfile.setf(ios::fixed);
	gdfile << fixed << std::setprecision(10);

	//��ȡ�۲���Ԫ��Ӧ�����Ǳ��
	ifstream numfile;
	numfile.open("Number.txt");

	for (int j = 0; j < ObsVec.size(); j++)
	{
		GPSTIME gpstime;
		//���۲���Ԫת��ΪGPSʱ
		CommonTimeToGPSTime(ObsVec[j].Observetime, gpstime);
		//�����¼���
		int *S_number = new int[ObsVec[j].ObserveSatSum];
		stfile << "��" << j + 1 << "���۲���Ԫ" << "  " << "��" << ObsVec[j].ObserveSatSum << "���۲�����" << endl;
		gdfile << "��" << j + 1 << "���۲���Ԫ" << "  " << "��" << ObsVec[j].ObserveSatSum << "���۲�����" << endl;
		for (int k = 0; k < ObsVec[j].ObserveSatSum; k++)
		{   
			//�������
			numfile >> S_number[k];
			SatVec.clear();
			getsatpostion(NavVec, S_number[k], gpstime.lSecond, 0);
			stfile << int(SatVec[0].prn) << "  " << SatVec[0].x << "  " << SatVec[0].y << "  " << SatVec[0].z << endl;
			//���㲢����߶Ƚ�
			dx =SatVec[0].x - X0;
			dy = SatVec[0].y - Y0;
			dz = SatVec[0].z - Z0;
			dN = -sin(B)*cos(L)*dx - sin(B)*sin(L)*dy + cos(B)*dz;
			dE = -sin(L)*dx + cos(L)*dy;
			dU = cos(B)*cos(L)*dx + cos(B)*sin(L)*dy + sin(B)*dz;
			double p, E;
			p = sqrt(dN*dN + dE*dE + dU*dU);
			E = asin(dU / p) * 180 / (atan(1) * 4);
			if (E < 0) E = E + 360;
			gdfile << E << " ";
		}
		gdfile << endl;
		delete[]S_number;
	}
	stfile.close();
	numfile.close();
	gdfile.close();
}


