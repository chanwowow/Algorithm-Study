#include <set>
#include <unordered_map>
#include <queue>
#include <memory.h>
#include <iostream>

using namespace std;

unordered_map <int, int> ID2hID;
int newhID = 1;

struct {
	int ID; int score; int grade; int gender;
} stdDB[20001];

struct student {
	int ID;
	int score;
};
struct cmp1 {
	bool operator()(student a, student b) const {
		if (a.score == b.score)return a.ID < b.ID;
		return a.score < b.score;
	}
};
struct cmp {
	bool operator()(student a, student b) const {
		if (a.score == b.score)return a.ID > b.ID;
		return a.score > b.score;
	}
};
set <student, cmp1> ST[3][2];
priority_queue <student, vector<student>, cmp> PQ;

void init() {
	// 초기화 코드 삽입
	ID2hID.clear();
	memset(stdDB, 0, sizeof(stdDB)); //오 시발 구조체에도 이게 되는구나?
	newhID = 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 2; j++) {
			ST[i][j].clear();
		}
	}
}

int add(int mId, int mGrade, char mGender[7], int mScore) {
	int hID = ID2hID[mId];
	if (hID == 0) {// 처음 들어온 놈이면
		hID = ID2hID[mId] = newhID++;
	}
	int grade = mGrade - 1;
	int gender = (mGender[0] == 'm') ? 0 : 1; // 남자면 0, 여자면 1

	stdDB[hID] = { mId, mScore, grade, gender };
	ST[grade][gender].insert({ mId, mScore });
	
	return ST[grade][gender].rbegin()->ID;
}

int remove(int mId) {
	int hID = ID2hID[mId];
	// 1. mId 학생이 없는 경우 0
	if (hID == 0) return 0;
	// 2. 점수가 기록되어있지 않은 경우 0
	if (stdDB[hID].score == -1) return 0; 

	// 학생 기록 삭제
	int grade = stdDB[hID].grade;
	int gender = stdDB[hID].gender;
	ST[grade][gender].erase({mId, stdDB[hID].score});
	stdDB[hID].score = -1;

	// 3. 삭제 후 동일 조건의 학생이 남이있지 않은 경우 0
	if (ST[grade][gender].empty()) return 0;
	return ST[grade][gender].begin()->ID;

}

int query(int mGradeCnt, int mGrade[], int mGenderCnt, char mGender[][7], int mScore) {
	PQ = {};

	for (int i = 0; i < mGradeCnt; i++) {
		for (int j = 0; j < mGenderCnt; j++) {
			int grade = mGrade[i] - 1;
			int gender = (mGender[j][0] == 'm') ? 0 : 1; // 남자면 0, 여자면 1

			auto low = ST[grade][gender].lower_bound({ 0, mScore });
			if (low == ST[grade][gender].end()) continue;

			int init_score = low->score;
			for (low; low != ST[grade][gender].end(); low++) {
				if (low->score > init_score) break;
				PQ.push({ low->ID, low->score });
			}
		}
	}
	
	if (PQ.empty()) return 0;
	return PQ.top().ID;

}