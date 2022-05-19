#define MaxCase 10000

#include <string>	
#include <list>
#include <unordered_map>
#include <map>
#include <memory.h>

#include <iostream>

using namespace std;

unordered_map <string, int> branchToIdx;

struct str1 {
	int editTime;
	string data;
	int genTime;
};
struct fold1 {
	int motherIdx;
	map <int, string> fileQ; 
	unordered_map<string, str1> fileInfo;
	list<int> childFolder;
} folder[MaxCase]; int poolCnt;

int newFolder(int motherIdx) {
	folder[poolCnt] = { motherIdx, {}, {}, {} };
	return poolCnt++;
}

void init() {
	branchToIdx = {};
	poolCnt = 0;

	branchToIdx["root"] = 0;
	newFolder(0);
}

void create(int mTime, char mBranch[], char mFile[], char mData[]) {
	int idx = branchToIdx[mBranch];
	if (folder[idx].fileQ.size() == 50) {
		auto itr = folder[idx].fileQ.begin();
		folder[idx].fileInfo.erase(itr->second);
		folder[idx].fileQ.erase(itr);
	}
	folder[idx].fileQ[mTime] = mFile;
	folder[idx].fileInfo[mFile] = { mTime ,mData, mTime };
}

void edit(int mTime, char mBranch[], char mFile[], char mData[]) {
	int idx = branchToIdx[mBranch];
	folder[idx].fileInfo[mFile].editTime = mTime;
	folder[idx].fileInfo[mFile].data = mData;
	// 생성시각은 안건듦
}

void branch(int mTime, char mBranch[], char mChild[]) {
	int idx = branchToIdx[mBranch];
	int idxChild = newFolder(idx);
	branchToIdx[mChild] = idxChild;
	folder[idxChild].motherIdx = idx;
	folder[idx].childFolder.push_back(idxChild);

	// 파일 모두 복사하기
	for (auto& itr : folder[idx].fileQ) folder[idxChild].fileQ[itr.first] = itr.second;
	for (auto& itr : folder[idx].fileInfo) folder[idxChild].fileInfo[itr.first] = itr.second;
}


void merging(int idxThis) {
	if (!folder[idxThis].childFolder.empty()) {
		for (auto itr : folder[idxThis].childFolder) {
			merging(itr);
		}

	}
	// 그다음 얘를 마더로 복사하는 작업
	int idxMother = folder[idxThis].motherIdx;

	for (auto& itr : folder[idxThis].fileInfo) {
		// 1. 부모에게 없다면 파일 복사
		int genTimeMother = folder[idxMother].fileInfo[itr.first].genTime;
		int editTimeMother = folder[idxMother].fileInfo[itr.first].editTime;
		if (genTimeMother == 0) {
			//	cout << "idxMom :" << idxMother << " doesnthave :" << itr.first << folder[idxMother].fileInfo[itr.first].data << endl;
			folder[idxMother].fileInfo[itr.first] = itr.second;
			folder[idxMother].fileQ[itr.second.genTime] = itr.first;
		}
		// 2. 부모에게 파일 중복될때
		// 아래의 경우에만 복사
		else if (genTimeMother == itr.second.genTime && editTimeMother < itr.second.editTime) {
			folder[idxMother].fileInfo[itr.first] = itr.second;
		}
	}
	// 생성파일 50개 맞추기
	while (folder[idxMother].fileQ.size() > 50) {
		auto itr = folder[idxMother].fileQ.begin();
		folder[idxMother].fileInfo.erase(itr->second);
		folder[idxMother].fileQ.erase(itr);
	}

}

void merge(int mTime, char mBranch[]) {
	int idx = branchToIdx[mBranch];
	merging(idx);
}

int readfile(int mTime, char mBranch[], char mFile[], char retString[]) {
	int idx = branchToIdx[mBranch];
	string data = folder[idx].fileInfo[mFile].data;
	strcpy(retString, data.c_str());

	//cout << "       ===> MY ANS : " << data.length() << "  " << retString << endl;
	return data.length();
}
