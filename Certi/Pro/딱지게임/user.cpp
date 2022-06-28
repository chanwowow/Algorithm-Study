
const int Max_TJ = 20005;

///#include <deque>
#include <memory.h>
#include <iostream>

using namespace std;

struct tj {
	int r1, r2;
	int c1, c2;
	int parentID;
	int belongTo;
	int stack;
} TJ[Max_TJ];

int findParent(int ID) {
	int id = ID;
	while (TJ[id].parentID != id) {
		id = TJ[id].parentID;
	}
	TJ[ID].parentID = id; // 압축
	return id;
}

void unionParent(int ID_1, int ID_2) {
	int id_1 = findParent(ID_1);
	int id_2 = findParent(ID_2);
	TJ[id_2].parentID = id_1;
	TJ[id_1].stack += TJ[id_2].stack;// 누계도 합쳐준다.
}

//deque <int> group[10][10]; // 해당그룹 ID 저장
int Group[10][10][Max_TJ];

int idMax; int gM;
int p_TJ[3];

void init(int N, int M){
	// 초기화
	gM = M;
	idMax = 1;
	p_TJ[1] = p_TJ[2] = 0;

	memset(Group, 0, sizeof(Group));
}


int addDdakji(int mRow, int mCol, int mSize, int mPlayer) {
	int sR = mRow; int eR = sR + mSize - 1;
	int sC = mCol; int eC = sC + mSize - 1;

	int M = gM;
	int rGroup1 = sR / M;
	int rGroup2 = eR / M;
	int cGroup1 = sC / M;
	int cGroup2 = eC / M;

	int parentBaseWe = 0;
	int parentBaseOppo = 0;
	// 1. 아군 처음나오면 parentBaseWE
	// 2. 적군 처음나오면 parentBaseOppo
	// 
	for (register int rG = rGroup1; rG <= rGroup2; rG++) {
		for (register int cG = cGroup1; cG <= cGroup2; cG++) {
			for (register int idx = 1; idx <= Group[rG][cG][0]; idx++) {
				int ID = Group[rG][cG][idx];

				if (eR<TJ[ID].r1 || sR>TJ[ID].r2 || eC<TJ[ID].c1 || sC>TJ[ID].c2) continue;

				int parentID = findParent(ID);
				int player = TJ[parentID].belongTo;

				if (player == mPlayer) {
					// 플레이어가 같다면
					if (parentBaseWe == 0) 	parentBaseWe = parentID;
					else {
						if (parentID != parentBaseWe) {
							unionParent(parentBaseWe, parentID);
						}
					}
				}
				else {
					// 다른 플레이어라면
					if (parentBaseOppo == 0) parentBaseOppo = parentID;
					else {
						if (parentID != parentBaseOppo) {
							unionParent(parentBaseOppo, parentID);
						}
					}

				}
			}
		}
	}

	int newID = idMax++;
	int changeN = 0;
	TJ[newID] = { sR,eR, sC, eC, newID , mPlayer, 1 };
	if (parentBaseOppo != 0) {
		changeN = TJ[parentBaseOppo].stack;
		unionParent(newID, parentBaseOppo);
	}
	if (parentBaseWe != 0) {
		unionParent(newID, parentBaseWe);
	}
	for (int rG = rGroup1; rG <= rGroup2; rG++) {
		for (int cG = cGroup1; cG <= cGroup2; cG++) {
			++Group[rG][cG][0];
			int idx = Group[rG][cG][0];
			Group[rG][cG][idx] = newID;
		}
	}

	if (mPlayer == 1) {
		p_TJ[1] += (changeN + 1);
		p_TJ[2] -= changeN;
	}
	else {
		p_TJ[2] += (changeN + 1);
		p_TJ[1] -= changeN;
	}
	return p_TJ[mPlayer];
}


int check(int mRow, int mCol){
	int ret = 0;

	int M = gM;
	int rGroup = mRow / M;
	int cGroup = mCol / M;

	for (register int idx = 1; idx <= Group[rGroup][cGroup][0]; idx++) {
		int ID = Group[rGroup][cGroup][idx];
		if (mRow<TJ[ID].r1 || mRow>TJ[ID].r2 || mCol<TJ[ID].c1 || mCol>TJ[ID].c2) continue;

		int parentID = findParent(ID);
		ret = TJ[parentID].belongTo;
		break;
	}

	return ret;
}


