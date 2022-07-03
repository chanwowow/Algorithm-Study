#include<map>
#include<set>

using namespace std;

map<int, int> usingSpace; // first는 시작주소, second는 size
struct eInfo {
	int eSize; int eAddr;
};
struct cmp { // 사이즈와, 주소순의 오름차순
	bool operator()(eInfo a, eInfo b) const {
		if (a.eSize == b.eSize) return a.eAddr < b.eAddr;
		return a.eSize < b.eSize;
	}
};
set <eInfo, cmp> freeSpace;
int gN;

void init(int N) {
	//초기화
	usingSpace.clear();
	freeSpace.clear();

	gN = N;
	freeSpace.insert({ N, 0 });
}


int allocate(int mSize) {

	auto low = freeSpace.lower_bound({ mSize, 0 });
	if (low == freeSpace.end()) return -1;

	int eSize = low->eSize;
	int eAddr = low->eAddr;
	freeSpace.erase({ eSize, eAddr });

	// 1. 할당하고 남는 공간 있으면 다시넣어줌
	int leftSize = eSize- mSize;
	if (leftSize > 0) {
		int leftsAddr = eAddr + mSize;
		freeSpace.insert({ leftSize, leftsAddr });
	}
	// 2. 할당공간에 대한 정보저장
	usingSpace[eAddr] = mSize;

	return eAddr;
}

int release(int mAddr) {
	auto mapItr = usingSpace.find(mAddr); // 현재해제위치
	if (mapItr == usingSpace.end()) return -1;
	int fSize = mapItr->second;
	int frontAddr; int frontSize;
	bool eraseEn = false;

	// 1. 앞공간 정리하기
	if (mapItr == usingSpace.begin()) {
		if (mAddr == 0) {
			eraseEn = false;
			frontAddr = mAddr;
			frontSize = 0;
		}
		else {
			eraseEn = true;
			frontAddr = 0;
			frontSize = mAddr;
		}
	}
	else {
		mapItr--;
		if (mAddr == mapItr->first + mapItr->second) {
			eraseEn = false;
			frontAddr = mAddr;
			frontSize = 0;
		}
		else {
			eraseEn = true;
			frontAddr = mapItr->first + mapItr->second;
			frontSize = mAddr - frontAddr;
		}
	}
	if (eraseEn) freeSpace.erase({ frontSize, frontAddr });


	// 2/ 뒷공간 정리
	int backAddr = mAddr + fSize;
	int backSize;
	auto low = usingSpace.lower_bound(backAddr);
	if (backAddr == gN || (low!=usingSpace.end()&& backAddr==low->first)) {
		// 뒷청크가끝이라 없거나, 사용중인 공간이라면
		backSize = 0;
	}
	else {
		if (low == usingSpace.end()) backSize = gN - backAddr;
		else backSize = low->first - backAddr;

		freeSpace.erase({ backSize, backAddr });
	}
	freeSpace.insert({ frontSize + fSize + backSize, frontAddr });

	usingSpace.erase(mAddr);// 최종 할당 목록에서도 삭제.
	return fSize;
}


