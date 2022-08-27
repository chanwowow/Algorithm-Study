#define maxN 3005

#include <unordered_map>
#include <set>
#include <unordered_set>
#include <queue>
#include <vector>
#include <memory.h>

using namespace std;

unordered_set <int> canConn[3]; // 도시 3개~  도시간 연결 되는지
struct myStr3 { int dis, ID; };
struct cmp3 {
	bool operator()(myStr3 a, myStr3 b) const {
		if (a.dis == b.dis) return a.ID < b.ID;
		return a.dis < b.dis;
	}
};
//set <myStr3, cmp3> disFromA, disFromB, disFromC, disFromAB, disFromBC, disFromAC, disFromABC;
unordered_map <int, int> sub[maxN];  // A-B 도시간의 직통거리

struct {
	int disABC[3];
	int rentP;
} cityInfo[maxN];

struct myStr1 { int dis; int ID; };
struct cmp1 {
	bool operator()(myStr1 a, myStr1 b) {
		if (a.dis == b.dis) return a.ID > b.ID;
		return a.dis > b.dis;
	}
};
struct myStr2 { int rentP; int disTotal; int ID; };
struct cmp2 {
	bool operator()(myStr2 a, myStr2 b) {
		if (a.rentP == b.rentP) {
			if (a.disTotal == b.disTotal) {
				return a.ID > b.ID;
			}return a.disTotal > b.disTotal;
		}return a.rentP > b.rentP;
	}
};
priority_queue<myStr1, vector<myStr1>, cmp1> djkPQ;
priority_queue<myStr2, vector<myStr2>, cmp2> recmPQ_A, recmPQ_B, recmPQ_C, recmPQ_AB, recmPQ_AC, recmPQ_BC, recmPQ_ABC;
vector<myStr2> temp;

bool visit[maxN];

int checkCity(int cityID);
void djk(int cityID);
void update();

int gN, gLimitDistance;
int cityA, cityB, cityC;
void init(int N, int mDownTown[]){
	gN = N;
	for (int i = 1; i <= N; i++) {
		cityInfo[i] = {};
		sub[i].clear();
	}
	for (int i = 0; i < 3; i++) canConn[i].clear();
	cityA = mDownTown[0]; cityB = mDownTown[1]; cityC = mDownTown[2];
	cityInfo[cityA].rentP = -1; cityInfo[cityB].rentP = -1; cityInfo[cityC].rentP = -1;
}
void newLine(int M, int mCityIDs[], int mDistances[]){
	for (int i = 0; i < M-1; i++) {
		int disPrev = sub[mCityIDs[i]][mCityIDs[i + 1]];
		if (disPrev == 0 || disPrev > mDistances[i]) {
			sub[mCityIDs[i]][mCityIDs[i + 1]] = mDistances[i];
			sub[mCityIDs[i + 1]][mCityIDs[i]] = mDistances[i];
		}
	}

	djk(cityA);	djk(cityB);	djk(cityC);
	int j = 1;
}
void changeLimitDistance(int mLimitDistance){
	gLimitDistance = mLimitDistance;
	update();
}
int findCity(int mOpt, int mDestinations[]){
	int ansCityID = -1;

	if (mOpt == 1) {
		int idxID = checkCity(mDestinations[0]);
		if (idxID == 0) {
			if (!recmPQ_A.empty()) {
				while (cityInfo[recmPQ_A.top().ID].rentP != recmPQ_A.top().rentP) {
					myStr2 top = recmPQ_A.top(); recmPQ_A.pop();
					recmPQ_A.push({ cityInfo[top.ID].rentP, top.disTotal, top.ID });
				}
				myStr2 top = recmPQ_A.top();
				recmPQ_A.pop();
				top.rentP++;
				recmPQ_A.push(top);
				ansCityID = top.ID;
			}
		}
		else if (idxID == 1) {
			if (!recmPQ_B.empty()) {
				while (cityInfo[recmPQ_B.top().ID].rentP != recmPQ_B.top().rentP) {
					myStr2 top = recmPQ_B.top(); recmPQ_B.pop();
					recmPQ_B.push({ cityInfo[top.ID].rentP, top.disTotal, top.ID });
				}
				myStr2 top = recmPQ_B.top();
				recmPQ_B.pop();
				top.rentP++;
				recmPQ_B.push(top);
				ansCityID = top.ID;
			}
		}
		else {
			if (!recmPQ_C.empty()) {
				while (cityInfo[recmPQ_C.top().ID].rentP != recmPQ_C.top().rentP) {
					myStr2 top = recmPQ_C.top(); recmPQ_C.pop();
					recmPQ_C.push({ cityInfo[top.ID].rentP, top.disTotal, top.ID });
				}
				myStr2 top = recmPQ_C.top();
				recmPQ_C.pop();
				top.rentP++;
				recmPQ_C.push(top);
				ansCityID = top.ID;
			}
		}
	}
	else if (mOpt == 2) {
		int idx1 = checkCity(mDestinations[0]);
		int idx2 = checkCity(mDestinations[1]);
		if ((idx1 == 0 && idx2 == 1) || (idx1 == 1 && idx2 == 0)) {
			if (!recmPQ_AB.empty()) {
				while (cityInfo[recmPQ_AB.top().ID].rentP != recmPQ_AB.top().rentP) {
					myStr2 top = recmPQ_AB.top(); recmPQ_AB.pop();
					recmPQ_AB.push({ cityInfo[top.ID].rentP, top.disTotal, top.ID });
				}
				myStr2 top = recmPQ_AB.top();
				recmPQ_AB.pop();
				top.rentP++;
				recmPQ_AB.push(top);
				ansCityID = top.ID;
			}
		}
		else if ((idx1 == 0 && idx2 == 2) || (idx1 == 2 && idx2 == 0)) {
			if (!recmPQ_AC.empty()) {
				while (cityInfo[recmPQ_AC.top().ID].rentP != recmPQ_AC.top().rentP) {
					myStr2 top = recmPQ_AC.top(); recmPQ_AC.pop();
					recmPQ_AC.push({ cityInfo[top.ID].rentP, top.disTotal, top.ID });
				}
				myStr2 top = recmPQ_AC.top();
				recmPQ_AC.pop();
				top.rentP++;
				recmPQ_AC.push(top);
				ansCityID = top.ID;
			}
		}
		else {
			if (!recmPQ_BC.empty()) {
				while (cityInfo[recmPQ_BC.top().ID].rentP != recmPQ_BC.top().rentP) {
					myStr2 top = recmPQ_BC.top(); recmPQ_BC.pop();
					recmPQ_BC.push({ cityInfo[top.ID].rentP, top.disTotal, top.ID });
				}
				myStr2 top = recmPQ_BC.top();
				recmPQ_BC.pop();
				top.rentP++;
				recmPQ_BC.push(top);
				ansCityID = top.ID;
			}
		}
	}
	else {
		if (!recmPQ_ABC.empty()) {
			while (cityInfo[recmPQ_ABC.top().ID].rentP != recmPQ_ABC.top().rentP) {
				myStr2 top = recmPQ_ABC.top(); recmPQ_ABC.pop();
				recmPQ_ABC.push({ cityInfo[top.ID].rentP, top.disTotal, top.ID });
			}
			myStr2 top = recmPQ_ABC.top();
			recmPQ_ABC.pop();
			top.rentP++;
			recmPQ_ABC.push(top);
			ansCityID = top.ID;
		}
	}

	if (ansCityID==-1) return -1;

	cityInfo[ansCityID].rentP++;
	return ansCityID;
}

int checkCity(int cityID) {
	int idxID = 0;
	if (cityID == cityA) idxID = 0;
	else if (cityID == cityB) idxID = 1;
	else idxID= 2;
	return idxID;
}

void djk(int cityID) {
	// city ID 부터 가능한 모든곳에 djk 방문작업
	int idxID = checkCity(cityID);

	memset(visit, 0, sizeof(visit));
	
	for (auto &itr : sub[cityID]) {
		djkPQ.push({ itr.second, itr.first });
	}
	visit[cityID] = true;

	while (!djkPQ.empty()) {
		myStr1 top = djkPQ.top(); djkPQ.pop();
		if (visit[top.ID] == true)continue;
		visit[top.ID] = true;
		// 여기서 도시간의 거리 및 연결정보도 저장한다
		canConn[idxID].insert(top.ID);
		cityInfo[top.ID].disABC[idxID] = top.dis;

		for (auto& itr : sub[top.ID]) {
			if (visit[itr.first] == true)continue;
			djkPQ.push({ itr.second + top.dis, itr.first });
		}
	}
}

void update() {
	//disFromA = disFromB = disFromC = disFromAB = disFromBC = disFromAC = disFromABC = {};
	recmPQ_A = recmPQ_B = recmPQ_C = recmPQ_AB = recmPQ_AC = recmPQ_BC = recmPQ_ABC = {};

	int LD = gLimitDistance;
	for (int cityID = 1; cityID <= gN; cityID++) {
		int rentP = cityInfo[cityID].rentP;
		if (rentP == -1) continue;
		int A = cityInfo[cityID].disABC[0];
		int B = cityInfo[cityID].disABC[1];
		int C = cityInfo[cityID].disABC[2];
		if (A != 0) {
			if(A<=LD) recmPQ_A.push({rentP, A, cityID});
			if (B != 0) {
				if(A+B<=LD) recmPQ_AB.push({rentP, A+B, cityID});
				if (C != 0)
					if(A+B+C<=LD) recmPQ_ABC.push({rentP, A+B+C, cityID});
			}
			if (C != 0)
				if(A+C<=LD) recmPQ_AC.push({rentP, A+C, cityID});
		}
		if (B != 0) {
			if(B<=LD) recmPQ_B.push({rentP, B, cityID});
			if (C != 0)
				if(B+C<=LD) recmPQ_BC.push({rentP, B+C, cityID});
		}
		if (C != 0)
			if(C<=LD) recmPQ_C.push({rentP, C, cityID});

	}

}