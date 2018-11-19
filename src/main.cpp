#include<iostream>
#include <iomanip>
#include"TimeSystem.h"
#include"Calculate_SatPostion.h"

int main()
{    
	 using namespace std;
	 readOfile();
	 readNfile();
	 IterationSat();
	 ToResult();
	 ToENU();
	 cout << "done!" << endl;
	 system("pause");
	 return 0;
}