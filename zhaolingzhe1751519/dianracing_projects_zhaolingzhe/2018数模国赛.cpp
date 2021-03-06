#include "pch.h"
#define SUCCESS 0
#define FAILED  1
#define UNDEF   65535
using namespace std;

//  #2  #4  #6  #8      //CNC
//  >>  >>  >>  >>      //bait
//  1   2   3   4       //RGV
//  >>  >>  >>  >>      //load
//  #1  #3  #5  #7      //CNC

int     t = 0;
int     n = 0;
int		cncState[9] = { 0 };    //0:Idle, 1:Working1, 2:Working2, 3:Repairing
int     cncIdleTime[9] = { 0 };
int     cncPos[9] = { UNDEF,1,1,2,2,3,3,4,4 };
int		cncType[9] = { UNDEF, 1,1,1,1,1,1,1,1 };	//1:Type1, 2:Type2
int		cncMat[9] = { UNDEF,0,0,0,0,0,0,0,0 };
int		workFinished[9] = { UNDEF, 0,0,0,0,0,0,0,0 };//0:Empty, 1:Work1Done, 2:Work2Done
int     rgvPos = 1;
int     rgvMoveTime[4] = { 0 ,UNDEF,UNDEF,UNDEF };
int     loadBaitTime[9] = { UNDEF,UNDEF,UNDEF,UNDEF,UNDEF,UNDEF,UNDEF,UNDEF,UNDEF };
int     cncProcessTime = UNDEF;
int     cncFirstProcTime = UNDEF;
int     cncSecondProcTime = UNDEF;
int     washTime = UNDEF;
bool	hasWork2 = false;
bool	hasError = false;
bool	verbose = false;
void	loop1();
void	loop2();
void	update();
int		abs(int a);
void	init(int group, bool bHasWork2, bool bHasError, bool bVerbose, int type);
void	output(int group, bool bHasWork2, bool bHasError);
vector<vector<int>> result(330);
vector<vector<int>> destoryReport(0);
int		arr[3][9] = { { 20,33,46,560,400,378,28,31,25 },
					 { 23,41,59,580,280,500,30,35,30 },
					 { 18,32,46,545,455,182,27,32,25 } };
struct Rgv
{
	void moveTo(int pos);
	int getMat(); //Re: MatNumber
	int fetchMat(int cncNum);//Re: MatNumber
	int load(int cncNum, int matNum);   //Re: success
	int bait(int cncNum, int matNum);   //Re: success
	void wash();
	int findClosestAvailableCncToPos(int type,int pos);
	int findClosestFinishedCnc(int type);
};

//假设
//机器人贪心（取当前时刻最短路径
//第一道工序做完后iff有第二道工序机器空闲，连续进行下料上料（相对于直接把料A->B

int main(int argc, char **argv)
{
	init(2,1,0,0,4);	//组号，是否有第二道工序，是否发生故障，是否调试
	//system("pause");
	return 0;
}

void init(int group, bool bHasWork2, bool bHasError, bool bVerbose, int type)
{
	rgvMoveTime[1] = arr[group - 1][0];
	rgvMoveTime[2] = arr[group - 1][1];
	rgvMoveTime[3] = arr[group - 1][2];
	cncProcessTime = arr[group - 1][3];
	cncFirstProcTime = arr[group - 1][4];
	cncSecondProcTime = arr[group - 1][5];
	loadBaitTime[1] = loadBaitTime[3] = loadBaitTime[5] = loadBaitTime[7] = arr[group - 1][6];
	loadBaitTime[2] = loadBaitTime[4] = loadBaitTime[6] = loadBaitTime[8] = arr[group - 1][7];
	washTime = arr[group - 1][8];
	hasError = bHasError;
	if (bVerbose)
		verbose = true;
	if (bHasWork2)
	{
		hasWork2 = true;
		switch (type)
		{
		default:
			break;
		case 4:
			cncType[2] = cncType[4] = cncType[6] = cncType[8] = 2;
			break;
		case 3:
			cncType[2] = cncType[6] = cncType[8] = 2;
			break;
		case 5:
			cncType[2] = cncType[3] = cncType[4] = cncType[6] = cncType[8] = 2;
			break;
		case 2:
			cncType[4] = cncType[6] = 2;
			break;
		case 6:
			cncType[2] = cncType[3] = cncType[4] = cncType[5] = cncType[6] = cncType[8] = 2;
			break;

			
		}
		/*
		if ((float)cncFirstProcTime / cncSecondProcTime < 0.8)
		{
			cncType[1] = 2;
			if ((float)cncFirstProcTime / cncSecondProcTime < 7.0 / 15.0)
				cncType[5] = 2;
				//cncType[3]=2;
		}
		else if ((float)cncSecondProcTime / cncFirstProcTime < 0.8)
		{
			cncType[2] = 1;
			if ((float)cncSecondProcTime / cncFirstProcTime < 7.0 / 15.0)
				cncType[8] = 1;
				//cncType[4]=2;
		}
		*/
		loop2();
	}
	else
		loop1();
	output(group, bHasWork2, bHasError);
}

void loop1()		//只有一道工序的主循环
{
	Rgv rgv;
	while (t < 8 * 3600)
	{
		bool flag = false;
		int cnc = rgv.findClosestAvailableCncToPos(1, rgvPos);
		if (cnc)
		{
			rgv.load(cnc, rgv.getMat());
			flag = true;
			update();
		}
		else
		{
			int cnc = rgv.findClosestFinishedCnc(1);
			if (cnc)
			{
				rgv.bait(cnc, rgv.fetchMat(cnc));
				flag = true;
				update();
			}
		}
		if (!flag)
		{
			t++;
			update();
		}
	}
}

void loop2()			//有两道工序的主循环
{
	Rgv rgv;
	while (t < 28800)
	{
		bool flag = false;

		int cnc1 = rgv.findClosestFinishedCnc(1);
		int cnc2 = rgv.findClosestAvailableCncToPos(2, cncPos[cnc1]);
		if (cnc1 && cnc2)
		{
			rgv.load(cnc2, rgv.fetchMat(cnc1));
			flag = true;
			update();
		}
		else
		{
			int cnc = rgv.findClosestAvailableCncToPos(1, rgvPos);
			if (cnc)
			{
				rgv.load(cnc, rgv.getMat());
				flag = true;
				update();
			}
			int cnc3 = rgv.findClosestFinishedCnc(2);
			if (cnc3)
			{
				rgv.bait(cnc3, rgv.fetchMat(cnc3));
				flag = true;
				update();
			}
		}
		if (!flag)
		{
			t++;
			update();
		}
	}
}

int Rgv::getMat()			//取得新物料
{
	n++;
	t += 0;					//不需要时间
	if(verbose) cout << t << "\tMat " << n << " get!\n";
	return n;
}

int Rgv::fetchMat(int cncNum)
{
	int mat;
	if (cncPos[cncNum] == rgvPos)
	{
		mat = cncMat[cncNum];
		result[mat].push_back(t);	//下料
		cncMat[cncNum] = 0;
		workFinished[cncNum] = 0;
		t += loadBaitTime[cncNum];	
	}
	else
	{
		moveTo(cncPos[cncNum]);
		return fetchMat(cncNum);
	}
	return mat;
}

void Rgv::moveTo(int pos)		//移动到指定位置
{
	t += rgvMoveTime[abs(pos - rgvPos)];
	rgvPos = pos;
	if(verbose)cout << t << "\tMove to " << pos << "\n";
}

int Rgv::load(int cncNum, int matNum)		//上料
{
	if (cncPos[cncNum] != rgvPos)
	{
		moveTo(cncPos[cncNum]);
		return load(cncNum, matNum);
	}
	result[matNum].push_back(cncNum);
	result[matNum].push_back(t);
	if (hasError)
	{
		random_device rd;
		default_random_engine e(rd());
		uniform_int_distribution<> u(1, 100);
		normal_distribution<>  n(900, 107.329);
		if (u(e) == 66)
		{
			int coolingTime = int(n(e));
			destoryReport.push_back({ matNum,cncNum,t,t+coolingTime });
			cncIdleTime[cncNum] = t + coolingTime;
			cncState[cncNum] = 3;
			if(verbose) cout << t<<"\tMat " << matNum << " destroyed CNC" << cncNum << "! Cooling for " << coolingTime << endl;
			return FAILED;
		}
	}
	if (!cncState[cncNum] && !cncMat[cncNum])
	{
		cncState[cncNum] = cncType[cncNum];
		cncMat[cncNum] = matNum;
		//cncIdleTime[cncNum] = t + cncProcessTime;
		cncIdleTime[cncNum] = t + ((!hasWork2) ? cncProcessTime : (1 == cncType[cncNum] ? cncFirstProcTime : cncSecondProcTime));
		t += loadBaitTime[cncNum];
		if(verbose) cout << t << "\tMat " << matNum << " loaded to CNC" << cncNum << endl;
		//wash();
		return SUCCESS;
	}
	cout << "load Failed\n";
	return FAILED;
}

int Rgv::bait(int cncNum, int matNum)		//下料
{
	if (cncPos[cncNum] = rgvPos)
	{
		//result[matNum].push_back(t);
		//t += loadBaitTime[cncNum];
		workFinished[cncNum] = 0;
		wash();
		if(verbose) cout << t << "\tMat " << n << " baited from CNC" << cncNum << endl;
		return SUCCESS;
	}
	cout << "bait Failed\n";
	return FAILED;
}

int Rgv::findClosestAvailableCncToPos(int type, int pos)
{
	int temp = 0;
	for (int cnc = 1; cnc <= 8; cnc++)
		if (cncType[cnc] == type && workFinished[cnc]==0 && !cncState[cnc] && abs(cncPos[cnc] - pos) < abs(cncPos[temp] - pos))
			temp = cnc;
	return temp;
}
int Rgv::findClosestFinishedCnc(int type)
{
	int temp = 0;
	for (int cnc = 1; cnc <= 8; cnc++)
		if (cncType[cnc] == type && workFinished[cnc]==type && abs(cncPos[cnc] - rgvPos) < abs(cncPos[temp] - rgvPos))
			temp = cnc;
	return temp;
}

void Rgv::wash()
{
	t += washTime;
}

void update()
{
	for (int cnc = 1; cnc <= 8; cnc++)
	{
		if (cncState && cncIdleTime[cnc] <= t)
		{
			if (1 == cncState[cnc])
			{
				workFinished[cnc] = 1;
				cncState[cnc] = 0;
			}
			else if (2 == cncState[cnc])
			{
				workFinished[cnc] = 2;
				cncState[cnc] = 0;
			}
			else if (3 == cncState[cnc])
				cncState[cnc] = 0;
		}
	}
}

int abs(int a)
{
	if (a < 0)
		return -a;
	else
		return a;
}

void output(int group, bool bHasWork2, bool bHasError)
{
	string fileName = "Case" + (string)(bHasError ? (bHasWork2 ? "3_2" : "3_1") : (bHasWork2 ? "2" : "1")) + "_Group"+ to_string(group);
	ofstream openfile(fileName+".csv", ios::out);
	for (auto it = result.begin()+1; !it->empty(); ++it)
	{
		int num = it - result.begin();
		openfile << num;
		for (auto i : *it)
			openfile <<','<< i;
		openfile << '\n';
	}
	if (bHasError)
	{
		string fileName2 = fileName + "_e";
		ofstream openfile(fileName2+".csv", ios::out);
		for (auto it : destoryReport)
		{
			for (auto i = it.begin(); i != it.end();)
			{
				openfile << *i;
				if (it.end() != ++i)
					openfile << ',';
			}
			openfile << '\n';
		}
	}
}