#define MaxCity 51
#define MaxRest 10001
#define MaxStr 100000

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <string>
#include <memory.h>
#include <iostream>

using namespace std;

unordered_set <int> interCity[MaxCity]; // ���ð��� ��������
unordered_map <string, int>	hashMap; //�Ĵ��̸� hash
int newrID; int g_rID;

vector<int> localRest[MaxCity]; // ���� �� �Ĵ�list
int valueRest[MaxRest];// �Ĵ�����

// BFS �Ĵ簡ġ����PQ , �����ʱ�ȭ//
struct str2 { int step, cID; };
queue<str2> Q;
bool visit[MaxCity];

struct str3 { int valueRest, rID; };
struct cmp {
	bool operator()(str3 a, str3 b) {
		return a.valueRest < b.valueRest;
	}
};
priority_queue <str3, vector<str3>, cmp> PQ;
/////////////////////////////////////

// Ʈ���̷� �ذ��ϱ�
//unordered_set<int> strToRest[MaxStr]; int newStrID = 1;

struct TRIE {
	int maxValue;
	TRIE* child[26];
} TriePool[MaxStr]; int poolCnt;
TRIE* newTRIE() {
	TRIE* ptr = &TriePool[poolCnt++];
	memset(ptr, 0, sizeof(TRIE));
	return ptr;
}
TRIE* Root;

string glbStr;
void InsertTRIE(int strIdx, TRIE* t2s) {
	// ������� �Ĵ簡 �ְ�ġ ����
	t2s->maxValue = max(t2s->maxValue, valueRest[g_rID]);

	if (glbStr[strIdx] == NULL) return;

	int curr = glbStr[strIdx] - 'a';
	if (t2s->child[curr] == NULL) t2s->child[curr] = newTRIE();
	InsertTRIE( ++strIdx, t2s->child[curr]);
}
int FindTRIE(int strIdx, TRIE* t2s) {
	if (glbStr[strIdx] == NULL) {
		return t2s->maxValue;
	}

	int curr = glbStr[strIdx] - 'a';
	return FindTRIE(++strIdx, t2s->child[curr]);
}

void init(int N, int M, int mRoads[][2]){
	for (int i = 0; i < 51; i++) {
		interCity[i] = {};
		localRest[i] = {};
	}
	hashMap = {};
	memset(valueRest, 0, sizeof(valueRest));
	newrID = 0;
	poolCnt = 0;
	Root = newTRIE();

	/// �������� ���� //
	for (int i = 0; i < M; i++) {
		interCity[mRoads[i][0]].insert(mRoads[i][1]);
		interCity[mRoads[i][1]].insert(mRoads[i][0]);
	}
	
}

void addRestaurant(int mCityID, char mName[]){
	// 1. �ؽø����� ����
	string name = mName;
	int rID = hashMap[name] = newrID++;
	g_rID = rID;
	localRest[mCityID].push_back(rID);

	// // 2. �������� DB ����
	for (int i = 0; i < name.length(); i++) {
		glbStr = "";
		for (int j = i; j < name.length(); j++) {
			glbStr += mName[j];
		}
		InsertTRIE(0, Root);
	}
	

}

void addValue(char mName[], int mScore){
	string name = mName;
	int rID = hashMap[name];
	g_rID = rID;
	valueRest[rID] += mScore;

	// 2. ���������� �ֽ� value ����
	for (int i = 0; i < name.length(); i++) {
		glbStr = "";
		for (int j = i; j < name.length(); j++) {
			glbStr += mName[j];
		}
		InsertTRIE(0, Root);
	}
}

int bestValue(char mStr[]){
	glbStr = mStr;
	int result = FindTRIE( 0, Root);
	return result;
}

int regionalValue(int mCityID, int mDist){
	int ret = 0;

	Q = {};
	PQ = {};
	memset(visit, 0, sizeof(visit));
	// mCityID  : ���۵��ø� ,, ��� �Ĵ絵 ���Եž��Ѵ�.
	Q.push({ 0, mCityID });
	visit[mCityID] = true;

	while (!Q.empty()) {
		str2 q = Q.front(); Q.pop();
		if (q.step > mDist) break;

		for (auto rID : localRest[q.cID]) {
			PQ.push({ valueRest[rID], rID});
		}
		for (auto cID : interCity[q.cID]) {
			if (visit[cID] == true) continue;
			visit[cID] = true;
			Q.push({ q.step + 1, cID });
		}
	}

	int cnt = 0;
	while (!PQ.empty() && cnt++ < 3) {
		ret += PQ.top().valueRest;
		PQ.pop();
	}
	return ret;
}
