#define Max_ppl 100000
#define Max_job 1000
#define Max_train 10

#include <set>
#include <vector>
#include <memory.h>

using namespace std;

struct person {
	int point;
	int compart;
} pInfo[Max_ppl];

vector<int> jobDB[Max_job];

struct myStr1 {
	int mID;
	int point;
};
struct cmp {
	bool operator() (myStr1 a, myStr1 b)const {
		if (a.point == b.point) return a.mID < b.mID;
		return a.point > b.point;
	}
};
set<myStr1, cmp> Train[Max_train];

int gN, gM, gC, gJ;

vector <myStr1> upV[10];
vector <myStr1> dnV[10];

void destroy() {}
void init(int N, int M, int J, int mPoint[], int mJobID[]){
	// 1. 초기화 코드 삽입필요!!
	memset(pInfo, 0, sizeof(pInfo));
	for (int j = 0; j < gJ; j++) jobDB[j] = {};
	for (int c = 0; c < gC; c++) Train[c].clear();
	

	// 2. 승객정보 저장
	gN = N; gM = M; gC = N / M; gJ = J;

	int c = 0;
	int i = 0;
	for (int mID = 0; mID < N; mID++) {
		Train[c].insert({ mID, mPoint[mID] });
		pInfo[mID] = { mPoint[mID],c };
		jobDB[mJobID[mID]].push_back(mID);

		if (++i == M) {
			i = 0;
			c++;
		}
	}
}
//$30
int update(int mID, int mPoint){
	//1. 열차내 정보 수정
	Train[pInfo[mID].compart].erase({ mID, pInfo[mID].point });
	Train[pInfo[mID].compart].insert({ mID, pInfo[mID].point + mPoint });

	// 2. 사람 DB도 수정
	pInfo[mID].point += mPoint;

	return pInfo[mID].point;
}
//#40
int updateByJob(int mJobID, int mPoint){
	int result = 0;
	for (auto mID : jobDB[mJobID]) {
		result += update(mID, mPoint);
	}
	return result;
}

//#5
int move(int mNum){

	for (int c = 0; c < gC - 1; c++) {
		auto bot = Train[c].end(); bot--;
		auto top = Train[c + 1].begin();

		upV[c].clear(); dnV[c].clear();
		for (int i = 0; i < mNum; i++) {
			dnV[c].push_back(*bot--);
			upV[c].push_back(*top++);
		}
	}
	int result = 0;
	for (int c = 0; c < gC - 1; c++) {
		for (auto &dn : dnV[c]) {
			Train[c].erase(dn);
			Train[c + 1].insert(dn);
			pInfo[dn.mID].compart = c + 1;
			result+= dn.point;
		}
		for (auto& up : upV[c]) {
			Train[c].insert(up);
			Train[c + 1].erase(up);
			pInfo[up.mID].compart = c;
			result += up.point;
		}
	}
	return result;
}
