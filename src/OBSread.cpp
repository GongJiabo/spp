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

//打开文件
	Oinfile.open("crao2860.17o");
	if (!Oinfile.is_open())  // failed to open file
	{
		std::cout << "Could not open the file " << endl;
		std::cout << "Program terminating.\n";
		cin.get();    // keep window open
		exit(EXIT_FAILURE);
	}



//===========================读取观测数据文件的文件头=========================//
	string begin;

	//读取文件头中的数据
	do
	{
		getline(Oinfile, begin);
	} while (begin.substr(60) != "APPROX POSITION XYZ");
	//
	//
//读取测站近似坐标
	otagdata.StationPosition.x = stod(begin.substr(0, 14));
	otagdata.StationPosition.y = stod(begin.substr(14, 14));
	otagdata.StationPosition.z = stod(begin.substr(28, 14));

//读取天线高及偏移(近似坐标数据后即为天线数据)
	Oinfile >> otagdata.AerialHight >> otagdata.AerialEastBias >> otagdata.AerialNorthBias;

//读取观测类型
	getline(Oinfile, begin);          //转到下一行
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
//观测值类型超过9个时 确定续行行数
	int i;                      
		if (otagdata.ObserveDataTypeSum % 9 == 0)
			i = otagdata.ObserveDataTypeSum / 9 - 1;
		else
			i = otagdata.ObserveDataTypeSum / 9;
//读取行观测值类型   
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

//=======================读取观测数据记录=======================//
		string line;
////读取到头文件尾
		do
		{
			getline(Oinfile, line);
		} while (line.substr(60) != "END OF HEADER");
//按类型读取各卫星的P1,P2观测值,num_P1,P2=30时则不存在该观测值
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
//判断是否存在P1,P2观测值
		if (num_P1 == 30)
			std::cout << "NO P1 VALUE!";
		else if (num_P2 == 30)
			std::cout << "NO P2 VALUE!";
//确定P1，P2所在观测类型的位置
		int P1_line, P2_line;
		if (num_P1 % 5 == 0)
			P1_line = num_P1 / 5;
		else
			P1_line = num_P1 / 5 + 1;
		if (num_P2 % 5 == 0)
			P2_line = num_P2 / 5;
		else
			P2_line = num_P2 / 5 + 1;
//读取文件尾后的观测数据
		int o_num = 0;  
		 OBSERVATION *odata=new OBSERVATION[3000];
	     //读取观测历元
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
			//判断卫星数所占行数 每颗卫星数据占的行数
			int col_sat, col_data;
			if (odata[o_num].ObserveSatSum % 12 == 0)
				col_sat = odata[o_num].ObserveSatSum / 12;
			else
				col_sat = odata[o_num].ObserveSatSum / 12 + 1;

			if (col_data = otagdata.ObserveDataTypeSum % 5 == 0)
				col_data = otagdata.ObserveDataTypeSum / 5;
			else
				col_data = otagdata.ObserveDataTypeSum / 5 + 1;
			//一行 即观测到1-12颗卫星
			if (col_sat == 1)
			{
				for (int i = 0; i < odata[o_num].ObserveSatSum; i++)
				{
					odata[o_num].SatellitePRN[i] = stoi(line.substr(33 + 3 * i, 2));
				}
			}
			//两行 即观测到13-24颗卫星
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
			//三行 即观测到25-36颗卫星
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
			//当P1，P2位于同一行时 按顺序读取
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
				//当P1，P2不在同一行 先读取位于前面的
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
