#define MAXCase 9000

#include <list>
#include <vector>
#include <unordered_map> 
#include <unordered_set>
#include <memory.h>
#include <string>

using namespace std;

unordered_map<string, int> hashMap;
int newhID;

struct str1 {
	int r, c;
	int ppl;
	bool del;
} DB[MAXCase];

int allyUnion[MAXCase];
list<int> allyList[MAXCase];
unordered_set <int> opponent[MAXCase];

struct str2 {
	int hID;
} map[27][27];
int dir[2][8] = {
	{1,0,-1,1,-1,1,0,-1},
	{1,1,1,0,0,-1,-1,-1}
};

vector<int> Bally;
vector<int> BOppo;

void init(int N, int mSoldier[25][25], char mMonarch[25][25][11]){
	// 초기화 코드
	memset(DB, 0, sizeof(DB));
	memset(allyUnion, 0, sizeof(allyUnion));
	memset(map, 0, sizeof(map));
	for (int i = 0; i < newhID; i++) {
		allyList[i] = {};
		opponent[i] = {};
	}

	newhID = 1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			int hID = hashMap[mMonarch[i][j]] = newhID++;
			map[i + 1][j + 1] = { hID };
			DB[hID] = { i + 1, j + 1,mSoldier[i][j], false };

			allyUnion[hID] = hID;
			allyList[hID].push_back(hID);
		}
	}
}

int getMother(int hID) {
	int orighID = hID;
	while (allyUnion[hID] != hID) {
		hID = allyUnion[hID];
	}
	allyUnion[orighID] = hID; // 경로압축!!	return hID;
	return hID;
}

int ally(char mMonarchA[], char mMonarchB[]){
	int result = 0;
	int AhID = hashMap[mMonarchA];
	int BhID = hashMap[mMonarchB];
	int motherA = getMother(AhID);
	int motherB = getMother(BhID);
	
	if (motherA == motherB) result = -1;
	else if (opponent[motherA].find(motherB) != opponent[motherA].end()) result = -2;
	else {
		// A-> B로 합치는 과정, 더 작은놈을 큰놈으로 합친다
		if (opponent[motherA].size() > opponent[motherB].size()) {
			swap(motherA, motherB);
		}
		// 동맹유니온, 동맹총몽록, 적대국목록
		allyUnion[motherA] = motherB;
		allyList[motherB].splice(allyList[motherB].begin(), allyList[motherA]);
		for (auto& itr : opponent[motherA]) {
			int motherOppo = getMother(itr);
			opponent[motherOppo].insert(motherB);
			opponent[motherB].insert(motherOppo);
		}

		result = 1;
	}
	return result;
}

int attack(char mMonarchA[], char mMonarchB[], char mGeneral[]){
	int AhID = hashMap[mMonarchA];
	int BhID = hashMap[mMonarchB];
	int motherA = getMother(AhID);
	int motherB = getMother(BhID);
	// CASE1. 동맹이라 전쟁 안남
	if (motherA == motherB) return -1;

	Bally = {};
	BOppo = {};
	int r = DB[BhID].r; int c = DB[BhID].c;
	for (int i = 0; i < 8; i++) {
		int x = r + dir[0][i];
		int y = c + dir[1][i];
		int nearhID = map[x][y].hID;

		if (nearhID == 0) continue;
		int nearMother = getMother(nearhID);
		if (nearMother == motherB) Bally.push_back(nearhID);
		if (nearMother == motherA) BOppo.push_back(nearhID);
	}
	// CASE2. 적대국 인접하지 않다면
	if (BOppo.empty()) return -2;

	///#전투발생 시점. 마지막에 적대국 처리 필요
	int result = 0;
	int pplDefence = DB[BhID].ppl; int pplAttack = 0;
	for (auto itr : Bally) {
		int pplGo = DB[itr].ppl / 2;
		DB[itr].ppl -= pplGo;
		pplDefence += pplGo;
	}
	for (auto itr : BOppo) {
		int pplGo = DB[itr].ppl / 2;
		DB[itr].ppl -= pplGo;
		pplAttack += pplGo;
	}
	// CASE3 공격 승리
	if (pplDefence < pplAttack) {
		DB[BhID].del = true;// 지움 처리
		// 새국가 건설
		int hID = hashMap[mGeneral] = newhID++;
		map[r][c] = { hID };
		DB[hID] = { r,c, pplAttack - pplDefence, false };
		allyUnion[hID] = motherA;
		allyList[motherA].push_back(hID);

		result = 1;
	}
	else {// CASE4 공격 패배
		DB[BhID].ppl = pplDefence - pplAttack;
		result = 0;
	}
	
	// 적대국 처리
	opponent[motherA].insert(motherB);
	opponent[motherB].insert(motherA);

	return result;
}

int recruit(char mMonarch[], int mNum, int mOption){
	int result = 0;
	int hID = hashMap[mMonarch];
	if (mOption == 0) {
		DB[hID].ppl += mNum;
		result = DB[hID].ppl;
	}
	else {
		int hIDMother = getMother(hID);

		for (auto itr : allyList[hIDMother]) {
			if (DB[itr].del == true) continue;
			DB[itr].ppl += mNum;
			result += DB[itr].ppl;
		}
	}
	return result;
}


void destroy() {

}