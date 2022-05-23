#include <map>
#include <memory.h>
#include <iostream>

using namespace std;

map <int, int> MP[101]; // X좌표 0 ~ 100. 저장값은 dX

void init(){
	for (int i = 0; i < 101; i++) {
		MP[i].clear();
	}
}


void add(int mX, int mY){
	MP[mX][mY] = mX + 1;
	MP[mX + 1][mY] = mX;
}

void remove(int mX, int mY){
	MP[mX].erase(mY);
	MP[mX + 1].erase(mY);
}

int numberOfCross(int mID){
	int result = 0;
	int X = mID;
	int Y = 0;
	auto nextPos = MP[X].upper_bound(Y);

	while (nextPos != MP[X].end()) {
		result++;
		X = nextPos->second;
		Y = nextPos->first;
		nextPos = MP[X].upper_bound(Y);
	}

	return result;
}

int participant(int mX, int mY){
	int X = mX;
	int Y = mY;
	// 시작점이 비어있는 경우 예외처리 필요하다.
	if (MP[X].empty()) return X;

	auto currPos = MP[X].lower_bound(Y);

	while (currPos != MP[X].begin()) {
		currPos--;
		X = currPos->second;
		Y = currPos->first;
		currPos = MP[X].lower_bound(Y);
	}

	return X;
}