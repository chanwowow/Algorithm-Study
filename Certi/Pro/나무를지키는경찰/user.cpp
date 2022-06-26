#define maxN 351
#define maxCase 100005

#include <memory.h>
#include <queue>
#include <vector>

using namespace std;

vector <int> interCity[maxN];
bool visit[maxN];

int policePos;
int nextPosTable[maxN][maxN];
int distTable[maxN][maxN];

struct myStr1 { int priority, occurTime, caseID; };
struct cmp1 {
	bool operator()(myStr1 a, myStr1 b) {
		if (a.priority == b.priority) return a.occurTime > b.occurTime;
		return a.priority < b.priority;
	}
};
priority_queue <myStr1, vector<myStr1>, cmp1> targetPQ;
struct {
	int occurTime, town, priority;
} caseDB[maxCase];

void DFS(int cityID, int startID, int nextID, int dist);
void update(int mTimeStamp);

int prevCheckTime;
void init(int N, int mParent[]) {
	// 초기화코드
	for (int i = 0; i < N; i++) {
		interCity[i].clear();
	}
	targetPQ = {};
	memset(nextPosTable, 0, sizeof(nextPosTable));
	memset(distTable, 0, sizeof(distTable));
	memset(caseDB, 0, sizeof(caseDB));

	policePos = 0;
	// 초기값 삽입
	for (int i = 1; i < N; i++) {
		interCity[i].push_back(mParent[i]);
		interCity[mParent[i]].push_back(i);
	}
	// DFS로 거리 및 nextPos 구해놓기
	for (int i = 0; i < N; i++) {
		memset(visit, 0, sizeof(visit));
		visit[i] = true;
		DFS(i, i, -1, 1);
	}
	int a = 1;
}

void occur(int mTimeStamp, int mID, int mTown, int mPriority){
	update(mTimeStamp);

	caseDB[mID] = { mTimeStamp, mTown,mPriority };
	targetPQ.push({ mPriority, mTimeStamp, mID });
}

void cancel(int mTimeStamp, int mID){
	update(mTimeStamp);

	caseDB[mID].occurTime = -1; // 지움표시
}

int position(int mTimeStamp){
	update(mTimeStamp);

	return policePos;
}

void update(int mTimeStamp) {
	int prevTime = prevCheckTime;

	while (!targetPQ.empty()) {
		if (prevTime == mTimeStamp) break;
		myStr1 top = targetPQ.top();
		if (caseDB[top.caseID].occurTime != top.occurTime) {
			targetPQ.pop(); continue;
		}

		int tagetCity = caseDB[top.caseID].town;
		int timeForMoving = distTable[policePos][tagetCity];
		if (timeForMoving + 1 + prevTime <= mTimeStamp) {
			targetPQ.pop();
			prevTime += (timeForMoving + 1);
			policePos = caseDB[top.caseID].town;
		}
		else {
			while (prevTime < mTimeStamp) {
				policePos = nextPosTable[policePos][tagetCity];
				++prevTime;
			} break;
		}

	}

	prevCheckTime = mTimeStamp;
}

void DFS(int cityID,int startID, int nextID, int dist) {
	
	for (auto itr : interCity[cityID]) {
		if (visit[itr] == true)continue;
		visit[itr] = true;
		if (cityID == startID) nextID = itr;
		nextPosTable[startID][itr] = nextID;
		distTable[startID][itr] = dist;

		DFS(itr, startID, nextID, dist + 1);
	}

}