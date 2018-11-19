#include"OBSread.h"
#include"Timesystem.h"
#include<string>
#include<iostream>
#include<fstream>
#include <cstdlib>
//
 std::vector<tagOBSERVATTOINDATA> TagoVec;
 std::vector<OBSERVATIONDATA> ObsVec;
//
void readOfile()
{
	using namespace std;
	ifstream Oinfile;

	tago otagdata;

//���ļ�
	Oinfile.open("crao2860.17o");
	if (!Oinfile.is_open())  // failed to open file
	{
		std::cout << "Could not open the file " << endl;
		std::cout << "Program terminating.\n";
		cin.get();    // keep window open
		exit(EXIT_FAILURE);
	}



//===========================��ȡ�۲������ļ����ļ�ͷ=========================//
	string begin;

	//��ȡ�ļ�ͷ�е�����
	do
	{
		getline(Oinfile, begin);
	} while (begin.substr(60) != "APPROX POSITION XYZ");
	//
	//
//��ȡ��վ��������
	otagdata.StationPosition.x = stod(begin.substr(0, 14));
	otagdata.StationPosition.y = stod(begin.substr(14, 14));
	otagdata.StationPosition.z = stod(begin.substr(28, 14));

//��ȡ���߸߼�ƫ��(�����������ݺ�Ϊ��������)
	Oinfile >> otagdata.AerialHight >> otagdata.AerialEastBias >> otagdata.AerialNorthBias;

//��ȡ�۲�����
	getline(Oinfile, begin);          //ת����һ��
	do
	{
		getline(Oinfile, begin);
	} while (begin.substr(64) != "TYPES OF OBSERV");
	otagdata.ObserveDataTypeSum = stoi(begin.substr(0, 6));

	for (int i = 0; i < 9; i++)
	{   
		
		otagdata.datatype[i] = begin.substr(10+6*i , 2);
		if (otagdata.datatype[i] == "\0")
			break;
	}
//�۲�ֵ���ͳ���9��ʱ ȷ����������
	int i;                      
		if (otagdata.ObserveDataTypeSum % 9 == 0)
			i = otagdata.ObserveDataTypeSum / 9 - 1;
		else
			i = otagdata.ObserveDataTypeSum / 9;
//��ȡ�й۲�ֵ����   
		if (i > 0)
		{
			for (int j = 0; j < i; j++)
			{
				getline(Oinfile, begin);
				for (int k = 0; k < 9; k++)
				{
					otagdata.datatype[9 * (j + 1) + k] = begin.substr(10 + 6 * k, 2);

				}
			}
	    }
		
		TagoVec.push_back(otagdata);

//=======================��ȡ�۲����ݼ�¼=======================//
		string line;
////��ȡ��ͷ�ļ�β
		do
		{
			getline(Oinfile, line);
		} while (line.substr(60) != "END OF HEADER");
//�����Ͷ�ȡ�����ǵ�P1,P2�۲�ֵ,num_P1,P2=30ʱ�򲻴��ڸù۲�ֵ
		int num_P1 = 0, num_P2 = 0;
		for (num_P1 = 0; num_P1 < 30; num_P1++)
		{
			if (otagdata.datatype[num_P1] == "P1")
				break;
		}
		for (num_P2 = 0; num_P2 < 30; num_P2++)
		{
			if (otagdata.datatype[num_P2] == "P2")
				break;
		}
		num_P1++;
		num_P2++;
//�ж��Ƿ����P1,P2�۲�ֵ
		if (num_P1 == 30)
			std::cout << "NO P1 VALUE!";
		else if (num_P2 == 30)
			std::cout << "NO P2 VALUE!";
//ȷ��P1��P2���ڹ۲����͵�λ��
		int P1_line, P2_line;
		if (num_P1 % 5 == 0)
			P1_line = num_P1 / 5;
		else
			P1_line = num_P1 / 5 + 1;
		if (num_P2 % 5 == 0)
			P2_line = num_P2 / 5;
		else
			P2_line = num_P2 / 5 + 1;
//��ȡ�ļ�β��Ĺ۲�����
		int o_num = 0;  
		 OBSERVATION *odata=new OBSERVATION[3000];
	     //��ȡ�۲���Ԫ
		do
		{
			getline(Oinfile, line);
			if (Oinfile.eof())
				break;
			odata[o_num].Observetime.year = stoi(line.substr(0, 3)) + 2000;
			odata[o_num].Observetime.month = stoi(line.substr(3, 3));
			odata[o_num].Observetime.day = stod(line.substr(6, 3));
			odata[o_num].Observetime.hour = stod(line.substr(9, 3));
			odata[o_num].Observetime.minute = stod(line.substr(12, 3));
			odata[o_num].Observetime.second = stod(line.substr(15, 11));
			odata[o_num].ObserveHealth = stoi(line.substr(26, 3));
			odata[o_num].ObserveSatSum = stoi(line.substr(29, 3));
			//�ж���������ռ���� ÿ����������ռ������
			int col_sat, col_data;
			if (odata[o_num].ObserveSatSum % 12 == 0)
				col_sat = odata[o_num].ObserveSatSum / 12;
			else
				col_sat = odata[o_num].ObserveSatSum / 12 + 1;

			if (col_data = otagdata.ObserveDataTypeSum % 5 == 0)
				col_data = otagdata.ObserveDataTypeSum / 5;
			else
				col_data = otagdata.ObserveDataTypeSum / 5 + 1;
			//һ�� ���۲⵽1-12������
			if (col_sat == 1)
			{
				for (int i = 0; i < odata[o_num].ObserveSatSum; i++)
				{
					odata[o_num].SatellitePRN[i] = stoi(line.substr(33 + 3 * i, 2));
				}
			}
			//���� ���۲⵽13-24������
			if (col_sat == 2)
			{
				for (int i = 0; i < 12; i++)
				{
					odata[o_num].SatellitePRN[i] = stoi(line.substr(33 + 3 * i, 2));
				}
				getline(Oinfile, line);
				for (int i = 0; i < 12; i++)
				{
					odata[o_num].SatellitePRN[12 + i] = stoi(line.substr(33 + 3 * i, 2));
				}
			}
			//���� ���۲⵽25-36������
			if (col_sat == 3)
			{
				for (int i = 0; i < 12; i++)
				{
					odata[o_num].SatellitePRN[i] = stoi(line.substr(33 + 3 * i, 2));
				}
				getline(Oinfile, line);
				for (int i = 0; i < 12; i++)
				{
					odata[o_num].SatellitePRN[12 + i] = stoi(line.substr(33 + 3 * i, 2));
				}
				getline(Oinfile, line);
				for (int i = 0; i < 12; i++)
				{
					odata[o_num].SatellitePRN[24 + i] = stoi(line.substr(33 + 3 * i, 2));
				}
			}
			//��P1��P2λ��ͬһ��ʱ ��˳���ȡ
			for (int j = 0; j < odata[o_num].ObserveSatSum; j++)
			{
				if (P1_line == P2_line)
				{
					odata[o_num].ovalue[j].prn = odata[o_num].SatellitePRN[j];
					if (P1_line == 1)
					{
						getline(Oinfile, line);
						string sp1,sp2;
						sp1 = line.substr((num_P1 - 1) * 16, 14);
						sp2 = line.substr((num_P2 - 1) * 16, 14);
						if (sp1 == "              ")
							odata[o_num].ovalue[j].P1 = 0;
						else
							odata[o_num].ovalue[j].P1 = stod(sp1);

						if (sp2 == "              ")
							odata[o_num].ovalue[j].P2 = 0;
						else
							odata[o_num].ovalue[j].P2 = stod(sp2);

						for (int i = 0; i < col_data - 1; i++)
							getline(Oinfile, line);
					}
					else if (P1_line == 2)
					{
						getline(Oinfile, line);
						getline(Oinfile, line);
						string sp1, sp2;
						sp1 = line.substr((num_P1 - 1) * 16, 14);
						sp2 = line.substr((num_P2 - 1) * 16, 14);
						if (sp1 == "              ")
							odata[o_num].ovalue[j].P1 = 0;
						else
							odata[o_num].ovalue[j].P1 = stod(sp1);

						if (sp2 == "              ")
							odata[o_num].ovalue[j].P2 = 0;
						else
							odata[o_num].ovalue[j].P2 = stod(sp2);

						for (int i = 0; i < col_data - 2; i++)
							getline(Oinfile, line);
					}
					else if (P1_line == 3)
					{
						getline(Oinfile, line);
						getline(Oinfile, line);
						getline(Oinfile, line);
						string sp1, sp2;
						sp1 = line.substr((num_P1 - 1) * 16, 14);
						sp2 = line.substr((num_P2 - 1) * 16, 14);
						if (sp1 == "              ")
							odata[o_num].ovalue[j].P1 = 0;
						else
							odata[o_num].ovalue[j].P1 = stod(sp1);

						if (sp2 == "              ")
							odata[o_num].ovalue[j].P2 = 0;
						else
							odata[o_num].ovalue[j].P2 = stod(sp2);

						for (int i = 0; i < col_data - 3; j++)
							getline(Oinfile, line);
					}

				}
				//��P1��P2����ͬһ�� �ȶ�ȡλ��ǰ���
				else if (P1_line != P2_line)
				{
					odata[o_num].ovalue[j].prn = odata[o_num].SatellitePRN[j];
					if (P1_line < P2_line)
					{
						for (int i = 0; i < P1_line; i++)
							getline(Oinfile, line);
						odata[o_num].ovalue[j].P1 = stod(line.substr((num_P1 - 1) * 16, 14));
						for (int i = 0; i < P2_line - P1_line; i++)
							getline(Oinfile, line);
						odata[o_num].ovalue[j].P2 = stod(line.substr((num_P2 - 5 - 1) * 16, 14));
					}
					if (P2_line < P1_line)
					{
						for (int i = 0; i < P2_line; i++)
							getline(Oinfile, line);
						odata[o_num].ovalue[j].P2 = stod(line.substr((num_P2 - 1) * 16, 14));
						for (int i = 0; i < P1_line - P2_line; i++)
							getline(Oinfile, line);
						odata[o_num].ovalue[j].P1 = stod(line.substr((num_P1 - 5 - 1) * 16, 14));
					}
				}
			}
			ObsVec.push_back(odata[o_num]);
			o_num++;
		}while (o_num < 3000);
		//
		//
		//test	
		ofstream test;
		test.open("P1_P2_Value.txt");
		for (int k = 0; k < o_num; k++)
		{
			for (int l = 0; l < odata[k].ObserveSatSum; l++)
			{
				test << odata[k].ovalue[l].P1 << "   " << odata[k].ovalue[l].P2 << endl;
			}
		}
		test.close();

		//
		std::cout << "Number of Obsversation data:" << o_num << endl;
		//
	    Oinfile.close();
		//
		delete[]odata;
}
