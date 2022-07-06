#define MAX_N 10
#define MAX_LEN 11
#define MAX_pp 1001
#define MAX_word 100000

#include <unordered_map>
#include <map>
#include <queue>
#include <memory.h>

#include <iostream>

using namespace std;

map <int, int> winner;

int playerWordMap[MAX_pp][MAX_N][MAX_N];// 각자 빙고판 단어위치 저장
unordered_map <string, int> hashMap; int newWordID;
vector<int> ppOfWord[MAX_word];
string wordTable[MAX_word];

struct str2 { int r, c; };
struct pp {
	unordered_map <int, str2> wordPos;// 단어좌표들 보유
	int row[MAX_N], col[MAX_N], x1, x2;
	int bingoRow[MAX_N], bingoCol[MAX_N], bingox1, bingox2;
} player[MAX_pp];

int pow2[10] = { 1,2,4,8,16,32,64,128,256,512 };
int tenTable[1050];
int numOf1(int n) {
	int ret = tenTable[n];
	if (n != 0 && !ret) {
		int nOri = n;
		while (n != 0) {
			if (n & 1) ret++;
			n >>= 1;
		}
		tenTable[nOri] = ret;
	}
	
	return ret;
}

struct str1 {
	int xNum, r, c;
};
struct cmp {
	bool operator()(str1 a, str1 b) {
		if (a.xNum == b.xNum) {
			if (a.r == b.r) return a.c > b.c;
			return a.r > b.r;
		}return a.xNum < b.xNum;
	}
};
priority_queue <str1, vector<str1>, cmp> PQ;

int gameGo();
int eraseWord(int ppID, int wordID);
int maxN, maxPlayer,roundN;
/////////////////////////////////////
void init(int N, int M){
	memset(playerWordMap, 0, sizeof(playerWordMap));
	hashMap = {};
	winner = {};
	for (int i = 1; i < newWordID; i++) {
		ppOfWord[i] = {};
		wordTable[i] = "";
	}
	newWordID = 1;
	// 참가자는 아래에서 초기화
	maxN = N;
	maxPlayer = M;
	roundN = 0;
}

void join(int mID, char mBingo[MAX_N][MAX_N][MAX_LEN]){
	player[mID] = {};// 초기화
	for (int r = 0; r < maxN; r++) {
		for (int c = 0; c < maxN; c++) {
			int wordID = hashMap[mBingo[r][c]];
			if (wordID == 0) wordID = hashMap[mBingo[r][c]] = newWordID++;
			ppOfWord[wordID].push_back(mID);

			if (wordTable[wordID].empty()) wordTable[wordID] = mBingo[r][c];
			playerWordMap[mID][r][c] = wordID;

			player[mID].wordPos[wordID] = { r,c };
		}
	}

}

void selectWord(int mRound, char mWord[]){
	int wordID = 0;

	while (roundN < mRound) {
		wordID = gameGo();
	}
	strcpy(mWord, wordTable[wordID].c_str());
	//cout << "selected" << wordTable[wordID] << endl;
}

int findWinners(){
	int a = 1;
	while (winner.empty()) gameGo();
	
	return winner.rbegin()->second;
}

int gameGo() {
	int ppID = roundN % maxPlayer + 1;
	int maxCheck=0;
	for (int i = 0; i < maxN; i++) {
		maxCheck = max(maxCheck, player[ppID].bingoRow[i]);
		maxCheck = max(maxCheck, player[ppID].bingoCol[i]);
	}
	maxCheck = max(maxCheck, player[ppID].bingox1); maxCheck = max(maxCheck, player[ppID].bingox2);

	bool x1max = (player[ppID].bingox1 == maxCheck) ? true : false;
	bool x2max = (player[ppID].bingox2 == maxCheck) ? true : false;

	PQ = {};
	for (auto& itr : player[ppID].wordPos) {
		int r = itr.second.r; int c = itr.second.c;
		int xNum = (r == c) ? 1 : 0; 	if (r + c == maxN - 1) xNum++;

		if (player[ppID].bingoRow[r] == maxCheck || player[ppID].bingoCol[c] == maxCheck) {
			PQ.push({ xNum, r, c });
		}
		else if(r==c&&x1max) PQ.push({ xNum, r, c });
		else if((r+c==maxN-1)&&x2max) PQ.push({ xNum, r, c });
	}
	str1 top = PQ.top();
	int wordID = playerWordMap[ppID][top.r][top.c];

	for (auto ppID : ppOfWord[wordID]) {
		int bingoNum = eraseWord(ppID, wordID);
		if (bingoNum != 0) {
			winner[bingoNum] += ppID;
		}
	}
	roundN++;
	return wordID;
}
int eraseWord(int ppID, int wordID) {
	auto itr = player[ppID].wordPos.find(wordID);
	int r =itr->second.r;
	int c =itr->second.c;
	player[ppID].wordPos.erase(itr);

	// 빙고현황 업데이트 bit idx 연산 부분
	player[ppID].row[r] += pow2[maxN - 1 - c];
	player[ppID].col[c] += pow2[maxN - 1 - r];
	player[ppID].bingoRow[r] = numOf1(player[ppID].row[r]);
	player[ppID].bingoCol[c] = numOf1(player[ppID].col[c]);
	if (r == c) {
		player[ppID].x1 += pow2[maxN - 1 - c];
		player[ppID].bingox1 = numOf1(player[ppID].x1);
	}
	if (r + c == maxN - 1) {
		player[ppID].x2 += pow2[maxN - 1 - c];
		player[ppID].bingox2 = numOf1(player[ppID].x2);
	}

	int numOfBingo = 0;
	for (int i = 0; i < maxN; i++) {
		if (player[ppID].bingoRow[i] == maxN )numOfBingo++;
		if (player[ppID].bingoCol[i] == maxN )numOfBingo++;
	}
	if (player[ppID].bingox1 == maxN ) numOfBingo++;
	if (player[ppID].bingox2 == maxN ) numOfBingo++;
	
	return numOfBingo;
}