
#include <unordered_map>
#include <vector>
#include <queue>
#include <memory.h>

using namespace std;

// 지도성분
int earthMap[1002][1002];
struct pos { int r; int c; };
vector<pos> earthComp[60002]; // idx별로 좌표성분(r,c) 저장

// 해시 및 인덱스
unordered_map <int, int> mID2_idx; // mID를 할당하여 idx++
int idx2_mID[60002]; // idx로 mID를 역탐색;
int new_idx = 1;

//주변탐색 PQ
struct nb { int nb_id; int nb_n; };
nb nb_arr[4];

struct cmp {
	bool operator ()(nb a, nb b) {
		if (a.nb_n == b.nb_n) return a.nb_id > b.nb_id;
		return a.nb_n < b.nb_n;
	}
};
priority_queue <nb, vector<nb>, cmp> Nearby_PQ;

int dir[2][4] = {
	{0, 1, 0, -1},
	{1, 0, -1, 0}
};

int G_N;
void init(int N){
	G_N = N;
	memset(earthMap, 0, sizeof(earthMap));
	for (int i = 0; i < new_idx; i++) {
		earthComp[i].clear();
	}
	mID2_idx = {};
	memset(idx2_mID, 0, sizeof(idx2_mID));
	new_idx = 1;

}

int newCivilization(int r, int c, int mID){
	//1. 인접 문명 판단
	for (int i = 0; i < 4; i++) nb_arr[i] = {};

	int x, y, mID_temp; int ind = 0;
	for (int i = 0; i < 4; i++) {
		x = r + dir[0][i]; y = c + dir[1][i];
		if (x<1 || x>G_N || y<1 || y>G_N) continue;
		if (earthMap[x][y] == 0) continue;

		mID_temp = idx2_mID[earthMap[x][y]];
		bool exist = false;
		for (int j = 0; j < ind; j++) {
			if (nb_arr[j].nb_id == mID_temp) { // 이미 있는값이면
				nb_arr[j].nb_n++;
				exist = true; 
				break;
			}
		}
		if (exist) continue;
		// 없었던 값이면
		nb_arr[ind++] = { mID_temp,1 };
	}

	/// 2. 인접 문명이 있다면
	if (ind != 0) {
		Nearby_PQ = {};

		for (int i = 0; i < ind; i++) {
			Nearby_PQ.push(nb_arr[i]);
		}

		mID = Nearby_PQ.top().nb_id;

		int idx = earthMap[r][c] = mID2_idx[mID];
		earthComp[idx].push_back({ r,c });
	}
	// 3. 인접 문명 없다면
	else {
		mID2_idx[mID] = new_idx; // 인덱스 할당
		earthMap[r][c] = new_idx;
		earthComp[new_idx].push_back({ r,c });
		idx2_mID[new_idx++] = mID;
	}

	return mID;
}

int removeCivilization(int mID){
	int idx = mID2_idx[mID]; 
	// 예외처리 1.
	if (idx==0) return 0;
	
	// 존재하는놈이라면 테이블 삭제
	mID2_idx[mID] = 0;
	idx2_mID[idx] = 0;

	//지도상 comp들도 삭제
	int num = 0;
	for (auto &itr1 : earthComp[idx]) {
		earthMap[itr1.r][itr1.c] = 0;
		num++;
	}
	return num;
}

int getCivilization(int r, int c){
	int idx = earthMap[r][c];
	int mID = idx2_mID[idx];
	
	return mID;
}

int getCivilizationArea(int mID){
	int idx = mID2_idx[mID];
	//예외처리 1.
	if (idx2_mID[idx] == 0) return 0;

	return earthComp[idx].size();
}

int mergeCivilization(int mID1, int mID2){
	int idx1 = mID2_idx[mID1]; int idx2 = mID2_idx[mID2];

	// 각각 문명의 크기 저장
	int size_mID1 = earthComp[idx1].size();
	int size_mID2 = earthComp[idx2].size();

	/// 크기 비교
	if (size_mID1 >= size_mID2) {
		for (auto& itr : earthComp[idx2]) {
			earthMap[itr.r][itr.c] = idx1;
			earthComp[idx1].push_back({ itr.r, itr.c });
		}
		mID2_idx[mID2] = 0;
		idx2_mID[idx2] = 0;
		size_mID1 = earthComp[idx1].size();
	}
	else { // idx2가 더 크면 idx1을 idx2로 합병한다
		for (auto& itr : earthComp[idx1]) {
			earthMap[itr.r][itr.c] = idx2;
			earthComp[idx2].push_back({ itr.r, itr.c });
		}
		idx2_mID[idx1] = 0;
		idx2_mID[idx2] = mID1;

		mID2_idx[mID1] = idx2;
		mID2_idx[mID2] = 0;

		size_mID1 = earthComp[idx2].size();
	}
	// 합병 후 크기 반환

	return size_mID1;

}
