#include "stdafx.h"
using namespace std;
#define MAX 1000
const double R = 6378.0;
const double PI = 3.14159265359;
struct Coordinate
{
	double latitude;
	double longitude;
};
Coordinate coords[MAX];
double sphericalDistance(Coordinate A, Coordinate B);
double dist[MAX][MAX];
int connected[MAX][MAX] = { 0 };
int degree[MAX] = {0};
int root[MAX] = { 0 };
vector<int>connects[MAX];
vector<int> hosts;
map<double, int> closest[MAX]; //closest[i]:{i,j,k...}, where dist[i]<dist[j]...
vector<int> closest_host[MAX];
vector<int> closest_sub[MAX];
bool isHost[MAX] = { 0 };
int sectors[MAX] = { 0 };
bool inGraph[MAX] = { 0 };
int jump[MAX] = { 0 };
int countP[MAX] = { 0 };	// Prim
int father[MAX] = { 0 };	//Kruskal
int nodesOfHost(int host);
int rowSum(int i);
void connectPair(pair<int, int>front);
int main()
{
	freopen("C:\\Users\\Dmitri\\Documents\\python\\k251.txt", "r", stdin);
	freopen("k251_prim.out", "w", stdout);

	for (int i = 0; i < MAX; i++) //数据读入
	{
		double a, b;
		int bIsHost, sector;
		scanf("%lf%lf%d%d", &a, &b, &bIsHost, &sector);
		sector++; //{0,1}->{1,2}
		coords[i] = { a, b };
		if (bIsHost)
		{
			isHost[i] = 1;
			inGraph[i] = 1;
			hosts.push_back(i);
			sectors[i] = sector;
			root[i] = i;
		}
	}
	for (int i = 0; i < MAX; i++)
	{
		for (int j = 0; j < MAX; j++)
		{
			if (i != j)
			{
				double distIj = sphericalDistance(coords[i], coords[j]);
				dist[i][j] = distIj;
				dist[j][i] = distIj;
				closest[i].insert(make_pair(distIj, j));
				closest[j].insert(make_pair(distIj, i));
			}
			else
				dist[i][j] = 0x7fffffff;
		}
	}

	for (int i = 0; i < MAX; i++)
	{
		for (auto it = closest[i].begin(); it != closest[i].end(); it++)
		{
			if (!isHost[it->second])
				closest_sub[i].push_back(it->second);
			else
				closest_host[i].push_back(it->second);
		}
	}  

	for (int h : hosts)
	{
		/*	BFS
		vector<int> hConn;
		int count = 0;
		for (auto p : closest_sub[h])
		{
			if (count<4*sectors[h] && !inGraph[p]&&dist[h][p]<50)		//连接前4个点
			{
				connected[h][p]=1;
				connected[p][h]=1;
				connects[h].push_back(p);
				connects[p].push_back(h);
				degree[h]++;
				degree[p]++;
				jump[p] = 1;
				inGraph[p]=1;
				root[p] = h;
				hConn.push_back(p);
				count++;
			}
			else if (count>=4*sectors[h] && count < 6*sectors[h])		//连接后2个点到前4个点
			{
				map<float, int> pDist;
				for (auto v : hConn)
					pDist.insert(make_pair(dist[p][v],v));
				int pClosest = pDist.begin()->second;
				if (dist[p][pClosest]<50 && !inGraph[p] &&1==degree[pClosest])
				{
					connected[p][pClosest]=1;
					connected[pClosest][p]=1;
					connects[p].push_back(pClosest);
					connects[pClosest].push_back(p);
					degree[p]++;
					degree[pClosest]++;
					jump[p] = jump[pClosest] + 1;
					inGraph[p] = 1;
					root[p] = h;
					count++;
				}
				else
					continue;
			}
		}
		
		*/
	
		/*Prim:*/
		struct cmp 
		{
			bool operator ()(pair<int, int> &a, pair<int, int> &b)
			{
				return dist[a.first][a.second]>dist[a.first][a.second];//最近者优先 
			}
		};
		priority_queue<pair<int,int>,vector<pair<int, int>>,cmp> QclosestPairs;

		while (inGraph[closest_sub[h][countP[h]]])
			countP[h]++;
		QclosestPairs.push(make_pair(h,closest_sub[h][countP[h]]));	//host is the first
		
		while (!QclosestPairs.empty())
		{
			pair<int,int> front = QclosestPairs.top();
			QclosestPairs.pop();
			int a = front.first;
			int b = front.second;
			if (isHost[a])
			{
				if (21 == b || 610 == b || 659 == b)
					int WTF=1;
				if(!inGraph[b])
				{
					connectPair(front);
					root[b] = h;
				}				
				int next1 = closest_sub[a][countP[a]];
				int next2 = closest_sub[b][countP[b]];
				
				if (next1 == next2)
				{
					if (dist[a][next1] <= dist[b][next2] && dist[a][next1] < 50 && !inGraph[next1] && 0 == degree[next1] && degree[a] < 4 * sectors[h])
						QclosestPairs.push(make_pair(a, next1));
					else if (dist[b][next2] < 20 && !inGraph[next2] && 0 == degree[next2] && 1 == degree[b] && jump[b] < 3)
						QclosestPairs.push(make_pair(b, next2));
					continue;
				}
				else
				{

					if (dist[a][next1] < 50 && !inGraph[next1] && 0 == degree[next1] && degree[a] < 4 * sectors[h])
						QclosestPairs.push(make_pair(a, next1));

					if (dist[b][next2] < 20 && !inGraph[next2] && 0 == degree[next2] && 1 == degree[b] && jump[b] < 3)
						QclosestPairs.push(make_pair(b, next2));
					continue;
				}
			}
			else if(1==degree[a])
			{
				if (21 == b || 610 == b || 659 == b || 21 == a || 610 == a || 659 == a)
					int WTF=1;
				if (!inGraph[b])
				{
					connectPair(front);
					root[b] = h;
				}
				int next1 = closest_sub[a][countP[a]];
				int next2 = closest_sub[b][countP[b]];

				if (next1 == next2)
				{
					if (dist[a][next1] <= dist[b][next2] && dist[a][next1] < 10 && 1 == degree[a] && jump[a]<3 && !inGraph[next1])
						QclosestPairs.push(make_pair(a, next1));
					else if (dist[b][next2] < 10 && 1 == degree[b] && jump[b]<3 && !inGraph[next2])
						QclosestPairs.push(make_pair(b, next2));
					continue;
				}
				else
				{

					if (dist[a][next1] < 10 && 1 == degree[a] && jump[a]<3 && !inGraph[next1])
						QclosestPairs.push(make_pair(a, next1));

					if (dist[b][next2] < 10 && 1 == degree[b] && jump[b]<3 && !inGraph[next2])
						QclosestPairs.push(make_pair(b, next2));
					continue;
				}
			}
			
		} 

		
		

	}
	//处理离散点（作废）
	/*
	vector<int> notConn;

	for(int i=0;i<MAX;i++)
		if (0==rowSum(i))
			notConn.push_back(i);
	for (int v : notConn)
	{
		bool flag = 0;
		//int thisVertex = (closest[v].begin()+i)->second;
		for(auto it=closest[v].begin();it!=closest[v].end();it++)
		{
			int thisVertex = it->second;
			if (dist[thisVertex][v] > 50)
				break;
			if (isHost[thisVertex] && degree[thisVertex]<4)
			{
				connectPair(make_pair(thisVertex,v));
				root[v] = thisVertex;
				flag = 1;
				break;
			}
			else if (jump[thisVertex] < 3 && 1==degree[thisVertex] && inGraph[thisVertex] &&nodesOfHost(root[v])<6*sectors[root[v]])
			{
				connectPair(make_pair(thisVertex,v));
				root[v] = root[thisVertex];
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			//connected[v][v] = 1;
			//cout << v << "<->" << v << ',';
		}
		
	}*/
	/*
	for (int h : hosts)
	{
		int i = 0;
		int hClosest = closest_host[h][i];
		
	
		if (dist[h][hClosest] <= 100)
		{
			connected[h][hClosest]=2;
			connected[hClosest][h]=2;
		}
	}
	*/
	//输出
	for (int i = 0; i < MAX; i++)
	{
		for (int j = i+1; j < MAX; j++)
		{
			//cout << connected[i][j] << ' ';
			if (connected[i][j])
				cout << dist[i][j]<<endl;
				//cout << i << "<->" << j << ',';
		}
		//cout << endl;
	}
	for (int i = 0; i < MAX; i++)
	{
		for (int j = i + 1; j < MAX; j++)
		{
			//cout << connected[i][j] << ' ';
			if (connected[i][j])
				cout << i << "<->" << j << ',';
		}
		//cout << endl;
	}

	return 0;
}

double sphericalDistance(Coordinate A, Coordinate B)
{
	double b1 = A.latitude * PI / 180;
	double b2 = B.latitude * PI / 180;
	double a1 = A.longitude * PI / 180;
	double a2 = B.longitude * PI / 180;
	double d = (R * acos(cos(b1) * cos(b2) * cos(a1 - a2) + sin(b1) * sin(b2)));
	//cout << d << endl;
	return d;
}

int nodesOfHost(int host)
{
	int sum=0;
	
	/*
	while (!Q.empty())
	{
		int front = Q.front();
		Q.pop();
		sum++;
		for(int v:connects[front])
			Q.push(v);
		if (Q.size() > 12)
		{
			cout << "ERROR at" << host << endl;
			break;
		}
	}
	*/

	for (int v : connects[host])
	{
		sum += degree[v];
		for (int w : connects[v])
			sum += degree[w];
	}
	return sum;
}

int rowSum(int i)
{
	int sum = 0;
	for (int j = 0; j < MAX; j++)
		sum += connected[i][j];
	return sum/2;
}

void connectPair(pair<int, int>front)
{
	connected[front.first][front.second] = 1;
	connected[front.second][front.first] = 1;
	inGraph[front.first] = 1;
	inGraph[front.second] = 1;
	connects[front.first].push_back(front.second);
	connects[front.second].push_back(front.first);
	jump[front.second] = jump[front.first]+1;
	degree[front.first]++;
	degree[front.second]++;
	countP[front.first]++;
}

/*Kruskal*/
int find(int x)									//用递归的实现
{
	if (father[x] != x) father[x] = find(father[x]);  //路径压缩
	return father[x];
}
void unionn(int r1, int r2)
{
	father[r2] = r1;
}
