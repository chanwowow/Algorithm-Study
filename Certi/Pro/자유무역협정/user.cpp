#define MAXN			(500)
#define MAXL			(11)

const int maxID = 10000;

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <memory.h>

using namespace std;

int parent[maxID];
list <int> unionList[maxID];
unordered_set <int> pjtList;
bool visit[maxID];

unordered_set <int> PJT[maxID];
unordered_map <string, int> hashMap; int nu_ID;

int findParent(int ID) {
	int id = ID;
	while (parent[id] != id) {
		id = parent[id];
	}
	parent[ID] = id;
	return id;
}

void unionParent(int pID_1, int pID_2) {
	if (unionList[pID_1].size() >= unionList[pID_2].size()) {
		unionList[pID_1].splice(unionList[pID_1].end(), unionList[pID_2]);
		parent[pID_2] = pID_1;
	}
	else {
		unionList[pID_2].splice(unionList[pID_2].end(), unionList[pID_1]);
		parent[pID_1] = pID_2;
	}
}

void init(int mNumA, char mCompanyListA[MAXN][MAXL], int mNumB, char mCompanyListB[MAXN][MAXL]){
	/// 초기화 코드
	nu_ID = 1;
	hashMap = {};
	int ID = 0;
	// 초기값 저장
	for (int i = 0; i < mNumA; i++) {
		ID = hashMap[mCompanyListA[i]] = nu_ID++;
		parent[ID] = ID;
		unionList[ID] = { ID };
		PJT[ID] = {};
	}
	for (int i = 0; i < mNumB; i++) {
		ID = hashMap[mCompanyListB[i]] = nu_ID++;
		parent[ID] = ID;
		unionList[ID] = { ID };
		PJT[ID] = {};
	}

}

void startProject(char mCompanyA[], char mCompanyB[]){
	int A_ID = hashMap[mCompanyA];
	int B_ID = hashMap[mCompanyB];
	PJT[A_ID].insert(B_ID);
	PJT[B_ID].insert(A_ID);
}

void finishProject(char mCompanyA[], char mCompanyB[]){
	int A_ID = hashMap[mCompanyA];
	int B_ID = hashMap[mCompanyB];
	PJT[A_ID].erase(B_ID);
	PJT[B_ID].erase(A_ID);
}

void ally(char mCompany1[], char mCompany2[]){
	int pID_1 = findParent(hashMap[mCompany1]);
	int pID_2 = findParent(hashMap[mCompany2]);
	if (pID_1 == pID_2) return;
	unionParent(pID_1, pID_2);
}

int conflict(char mCompany1[], char mCompany2[]){
	int pID_1 = findParent(hashMap[mCompany1]);
	int pID_2 = findParent(hashMap[mCompany2]);

	pjtList = {};
	memset(visit, 0, sizeof(visit));

	for (auto ID : unionList[pID_1]) {
		for (auto itr : PJT[ID]) {
			visit[findParent(itr)] = true;
		}
	}
	for (auto ID : unionList[pID_2]) {
		for (auto itr : PJT[ID]) {
			int pID = findParent(itr);
			if (visit[pID] == true) {
				pjtList.insert(pID);
			}
		}
	}

	int ret = 0;
	for (auto itr : pjtList) {
		ret += unionList[itr].size();
	}
	
	return ret;
}

