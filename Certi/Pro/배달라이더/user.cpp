
//////////
/*
15000 이상 TC들에서 fail 나는게 있는데 원인을 모르겠다
1. 자료형의 터짐은 아닌것같고
2. 동시처리의 문제? 근데 나는 그 타임에 끝내져야하는거 끝까지 다 하고 시작하는데
3. 저거 탐색하는 시점 시간의 문제일까?
*/


#define max_N 35
#define max_U 505
#define max_R 2005

#include <queue>
#include <memory.h>
#include <iostream>

using namespace std;

struct myStr1 { int dist, ID; };
struct cmp1 {
	bool operator()(myStr1 a, myStr1 b) {
		if (a.dist == b.dist) return a.ID > b.ID;
		return a.dist > b.dist;
	}
};
struct myStr2 { int finTime, ID, destDist; };
struct cmp2 {
	bool operator()(myStr2 a, myStr2 b) {
		if (a.finTime == b.finTime) {
			if (a.ID == b.ID) {
				return a.destDist > b.destDist;
			}return a.ID > b.ID;
		}return a.finTime > b.finTime;
	}
};

priority_queue <myStr1, vector<myStr1>, cmp1> watingPQ_R;
priority_queue <myStr2, vector<myStr2>, cmp2> workingPQ;
queue<int> orderQ; // 현재 대기주문 (집ID)
int waitingN_C; // 현재 카운터 잉여인력수

int userDB[max_U]; // 가게와의 거리 DB저장

int pastTimeStamp;
int gTimeStamp;
int cCnt;
void update();

void init(int N, int U, int uX[], int uY[],	int R, int rX[], int rY[]){
	gTimeStamp = pastTimeStamp = 0;
	cCnt = 0;
	watingPQ_R = {}; workingPQ = {}; orderQ = {};
	memset(userDB, 0, sizeof(userDB));
	/// 초기값 저장 ///
	waitingN_C = N;
	for (int uID = 0; uID < U; uID++) {
		userDB[uID] = uX[uID] + uY[uID];
	}
	for (int rID = 0; rID < R; rID++) {
		//int riderDistance = rX[rID] + rY[rID];
		watingPQ_R.push({rX[rID]+rY[rID], rID});
	}
}

int order(int mTimeStamp, int uID){
	gTimeStamp = mTimeStamp;
	update();

	orderQ.push(uID);
	update();

	return waitingN_C;
}

int checkWaitingRiders(int mTimeStamp){
	gTimeStamp = mTimeStamp;
	update();

	return watingPQ_R.size();
}

void update() {
	int curr = pastTimeStamp;
	int checkEndTime = gTimeStamp;

	while (curr <= checkEndTime) {
		myStr1 r; myStr2 work;
		//A
		while (waitingN_C > 0 && (!orderQ.empty())&& !(watingPQ_R.empty())) {
			// 직원 0/ 주문 0/ 라이더 0 => 주문 배정
			int uID = orderQ.front(); orderQ.pop();
			int uDist = userDB[uID];
			r = watingPQ_R.top(); watingPQ_R.pop();

			--waitingN_C;
			workingPQ.push({ curr + r.dist, --cCnt , 0 }); // 직원작업
			workingPQ.push({ curr + r.dist + uDist, r.ID , uDist }); // 배달원작업
		}

		//B
		if (workingPQ.empty()) break;
		// 다음 작업이 체크할 시점보다 이후면 그만보고 종료
		curr = workingPQ.top().finTime;
		if (curr > checkEndTime) break;

		work = workingPQ.top(); workingPQ.pop();
		if (work.ID < 0) { // 1. 직원 작업
			++waitingN_C;
		}
		else { // 2. 배달원작업
			watingPQ_R.push({ work.destDist, work.ID });
		}

	}
	pastTimeStamp = gTimeStamp;
}