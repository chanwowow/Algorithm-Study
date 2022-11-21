#include <string>
#include <memory.h>
#include <iostream>

using namespace std;

string Memo;
int cur;

int wordCnt[26];
int curMax;
int gW;

void init(int H, int W, char mStr[]){
	Memo = {};
	memset(wordCnt, 0, sizeof(wordCnt));
	cur = 0;
	gW = W;

	int i = 0;
	for (i; ; i++) {
		if (mStr[i] == '\0') break;
		wordCnt[mStr[i] - 'a']++;
		Memo += mStr[i];
	}

	curMax = i; // 이곳은 공란이다
}

void insert(char mChar){
	char str1[2] = { mChar, '\0'};
	Memo.insert(cur++, str1);
	wordCnt[mChar - 'a']++;
	curMax++;
}

char moveCursor(int mRow, int mCol){
	cur = (mRow - 1) * gW + mCol - 1;
	if (cur >= curMax) {
		cur = curMax;
		return '$';
	}
	return Memo[cur];
}

int countCharacter(char mChar){
	int wordN = 0;

	if (cur > curMax / 2) {
		int idx = cur;
		while (1)  {
			idx = Memo.find(mChar, idx);
			if (idx == -1) break;
			idx++;
			wordN++;
		}
	}
	else {
		wordN = wordCnt[mChar - 'a'];
		int idx = 0;
		while (1) {
			idx = Memo.find(mChar, idx);
			if (idx >= cur || idx == -1)break;
			idx++;
			wordN--;
		}
	}
	/*
	if (cur > curMax / 2) {
		auto st = Memo.begin();
		auto ed = Memo.end();
		advance(st, cur);
		wordN = count(st, ed, mChar);

	}
	else {
		auto st = Memo.begin();
		auto ed = Memo.begin();
		advance(ed, cur);
		wordN = wordCnt[mChar - 'a'] - count(st, ed, mChar);
	}
		*/ //=> 더 느리다
	return wordN;
}
