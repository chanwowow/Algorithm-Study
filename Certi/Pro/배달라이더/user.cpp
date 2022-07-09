
//////////
/*
15000 �̻� TC�鿡�� fail ���°� �ִµ� ������ �𸣰ڴ�
1. �ڷ����� ������ �ƴѰͰ���
2. ����ó���� ����? �ٵ� ���� �� Ÿ�ӿ� ���������ϴ°� ������ �� �ϰ� �����ϴµ�
3. ���� Ž���ϴ� ���� �ð��� �����ϱ�?
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
queue<int> orderQ; // ���� ����ֹ� (��ID)
int waitingN_C; // ���� ī���� �׿��η¼�

int userDB[max_U]; // ���Կ��� �Ÿ� DB����

int pastTimeStamp;
int gTimeStamp;
int cCnt;
void update();

void init(int N, int U, int uX[], int uY[],	int R, int rX[], int rY[]){
	gTimeStamp = pastTimeStamp = 0;
	cCnt = 0;
	watingPQ_R = {}; workingPQ = {}; orderQ = {};
	memset(userDB, 0, sizeof(userDB));
	/// �ʱⰪ ���� ///
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
			// ���� 0/ �ֹ� 0/ ���̴� 0 => �ֹ� ����
			int uID = orderQ.front(); orderQ.pop();
			int uDist = userDB[uID];
			r = watingPQ_R.top(); watingPQ_R.pop();

			--waitingN_C;
			workingPQ.push({ curr + r.dist, --cCnt , 0 }); // �����۾�
			workingPQ.push({ curr + r.dist + uDist, r.ID , uDist }); // ��޿��۾�
		}

		//B
		if (workingPQ.empty()) break;
		// ���� �۾��� üũ�� �������� ���ĸ� �׸����� ����
		curr = workingPQ.top().finTime;
		if (curr > checkEndTime) break;

		work = workingPQ.top(); workingPQ.pop();
		if (work.ID < 0) { // 1. ���� �۾�
			++waitingN_C;
		}
		else { // 2. ��޿��۾�
			watingPQ_R.push({ work.destDist, work.ID });
		}

	}
	pastTimeStamp = gTimeStamp;
}