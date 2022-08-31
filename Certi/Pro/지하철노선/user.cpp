#include <queue>	
#include<memory.h>


using namespace std;

int stn12[5][40000];
int stn21[5][40000];
vector<int> Trans[40000];

struct sub {
	int line;
	int stnN;
	int dir;
	int time;
};
sub Q1[60000];
int head;
int tail;
bool Visit[40000];

bool VisitL[5];
int minTrans;
bool lineConn[5][5];

void init(int N, int mLastStation1[5], int mLastStation2[5]){
	// 초기화 코드 삽입
	memset(stn12, 0, sizeof(stn12));
	memset(stn21, 0, sizeof(stn21));
	memset(lineConn, 0, sizeof(lineConn));
	for (int i = 0; i < 40000; i++) {
		Trans[i].clear();
	}
	
	for (int i = 0; i < 5; i++) {
		stn12[i][mLastStation1[i]] = mLastStation2[i];
		stn21[i][mLastStation1[i]] = mLastStation1[i];//종착역
		stn21[i][mLastStation2[i]] = mLastStation1[i];
		stn12[i][mLastStation2[i]] = mLastStation2[i];//종착역
		Trans[mLastStation1[i]].push_back(i);
		Trans[mLastStation2[i]].push_back(i);
		if (Trans[mLastStation1[i]].size() > 1) 
			for (auto Line : Trans[mLastStation1[i]]) {
				lineConn[Line][i] = true; lineConn[i][Line] = true;
			}
		if (Trans[mLastStation2[i]].size() > 1)
			for (auto Line : Trans[mLastStation2[i]]) {
				lineConn[Line][i] = true; lineConn[i][Line] = true;
			}
		
	}

}

void add(int mLine, int mPrevStation, int mStation){
	int nextStn = stn12[mLine][mPrevStation];

	stn12[mLine][mPrevStation] = mStation;
	stn21[mLine][mStation] = mPrevStation;
	stn12[mLine][mStation] = nextStn;
	stn21[mLine][nextStn] = mStation;

	Trans[mStation].push_back(mLine);
	if (Trans[mStation].size() > 1) {
		for (auto Line : Trans[mStation]) {
			lineConn[Line][mLine] = true;
			lineConn[mLine][Line] = true;
		}
	}
}



int minTravelTime(int mStartStation, int mEndStation) {
	if (mStartStation == mEndStation) return 0;

	memset(Visit, 0, sizeof(Visit));
	head = tail = 0;
	int result = 50000;
	sub q;  int nextStn;
	Visit[mStartStation] = true;

	for (auto Line : Trans[mStartStation]) {
		Q1[tail++] = { Line, mStartStation,0 , 0 };
		Q1[tail++] = { Line, mStartStation,1 , 0 };
	}
	while (head!=tail) {

		q = Q1[head++];

		// 환승때문에 최초 1회에 한해서 아래 내용 실행
		if (Visit[q.stnN] == false) {
			for (auto Line : Trans[q.stnN]) {
				if (Line != q.line) {
					Q1[tail++] = { Line, q.stnN, 0, q.time + 1 };
					Q1[tail++] = { Line, q.stnN, 1, q.time + 1 };
				}
			}
			Visit[q.stnN] = true;
		}

		nextStn = (q.dir == 0) ? stn12[q.line][q.stnN] : stn21[q.line][q.stnN];
		if (nextStn == mEndStation) {
			result = q.time + 1;
			break;
		}
		if (Visit[nextStn] == true) continue;
		Q1[tail++] = { q.line, nextStn, q.dir, q.time + 1 };

	}
	return (result == 50000) ? -1 : result;

}


void DFS(int Line, int eLine, int step) {
	VisitL[Line] = true;

	for (int i = 0; i < 5; i++) {
		if (VisitL[i] == true) continue;

		if (lineConn[Line][i] == true) {
			if (i == eLine) {
				minTrans = min(minTrans, step);
				VisitL[Line] = false;
				return;
			}
			else {
				DFS(i, eLine, step + 1);
			}
		}
	}
	VisitL[Line] = false;
	return;
}

int minTransfer(int mStartStation, int mEndStation){
	minTrans = 10;
	

	for (auto sLine : Trans[mStartStation]) {
		
		for (auto eLine : Trans[mEndStation]) {
			if (sLine == eLine) return 0;
			memset(VisitL, 0, sizeof(VisitL));
			DFS(sLine, eLine, 1);

		}
	}

	return (minTrans == 10) ? -1 : minTrans;
}


