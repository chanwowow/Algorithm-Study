#define MAXTRIE 20000  ///>>>>"??? 26^3
#define MAXUNION 100000

#include <unordered_map>
#include <unordered_set>
#include <list>
#include <vector>
#include <memory.h>

using namespace std;

int parent[MAXUNION];
int newTrieID;

unordered_set <int> trieID_Q;
//vector <int> strID_Q;

int findParent(int trieID) {
	int parentID = parent[trieID];
	while (parentID != parent[parentID]) {
		parentID = parent[parentID];
	}
	parent[trieID] = parentID;// 경로압축
	return parentID;
}
void unionParent(int trieID1, int trieID2) {  /// 얘는 불필요할지도?
	int parent1 = findParent(trieID1);
	int parent2 = findParent(trieID2);
	parent[parent2] = parent1;
}

int newStrID, gStrID;
list <int> wordList[MAXTRIE];  /// wordList[트라이부모ID] = {hID, hID2 };

struct myStr1 { int num, dieTime; };
unordered_map <int, myStr1> groupInfo;

struct TRIE {
	int trieID;
	TRIE* child[26];
} triePool[MAXTRIE]; int PoolCnt;
TRIE* newTRIE() {
	TRIE* ptr = &triePool[PoolCnt++];
	*ptr = {};
	return ptr;
} TRIE* Root;

void insertTRIE(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		if (t2s->trieID == 0) {
			// 처음나온 3문자 인 경우, ID 할당 후 union 초기화
			t2s->trieID = newTrieID++;
			parent[t2s->trieID] = t2s->trieID; 
		}
		int parentTrieID = findParent(t2s->trieID);
		trieID_Q.insert(parentTrieID);
	}
	int curr = *str - 'a';
	if (t2s->child[curr] == NULL) t2s->child[curr] = newTRIE();
	insertTRIE((str + 1), t2s->child[curr]);
}

int dieTimeDB[MAXTRIE];  // parentID들의 마지막 업뎃된 dieTIme;
unordered_set <int> timeLine[1000000];  /// => 저장정보 : 해당 시간에 삭제될 parentTrieID 리스트
///////////////////////////
void init(int N){

	PoolCnt = 0;
	Root = newTRIE();
	newTrieID = 1;
	newStrID = 1;
}

int generateString(int mTimestamp, int mLifespan, int mLen, char mStr[]){

	gStrID = newStrID++; 
	trieID_Q = {};
	int maxStart = mLen - 3;
	for (int i = 0; i <= maxStart; i++) {
		// 3개씩 짜개서 집어넣음
		char str1[4] = {mStr[i], mStr[i+1], mStr[i+2], '\0'};
		insertTRIE(str1, Root);
	}

	auto itr = trieID_Q.begin();
	int trieID_1 = *itr;
	int newNum = groupInfo[trieID_1].num;
	int newDieTime = groupInfo[trieID_1].dieTime;
	advance(itr, 1);
	for (itr; itr != trieID_Q.end(); itr++) {
		myStr1 temp = groupInfo[*itr];
		groupInfo[*itr] = {};

		newNum += temp.num;
		newDieTime = max(newDieTime, temp.dieTime);

		wordList[trieID_1].splice(wordList[trieID_1].end(), wordList[*itr]);
		wordList[trieID_1].push_back(*itr);
	}

	return -1;
}

int checkString(int mTimestamp, int mLen){
	return -1;
}