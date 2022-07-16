
#define MAXN 1000

#include<queue>
#include <memory.h>
#include <iostream>

using namespace std;

int glbMap[1002][1002];
bool mapVisit[1002][1002];
bool qVisit[1002][1002];

int dir[2][4] = {
	{1, 0, -1,0},
	{0, 1, 0, -1}
};
struct blk {
	int r; int c;
	int depth;
};
struct blk2 {
	int r; int c;
};
queue<blk> Q;
vector<blk> V;

int rRange;
int cRange;
int color;

void Qinsert(int r, int c);
int block4();
int block5();

void countBlock(int N, int mBoard[MAXN][MAXN], int mTetromino[5], int mPentomino[12]){
	memset(glbMap, 0, sizeof(glbMap));
	for (int r = 0; r < N; r++) {
		for (int c = 0; c < N; c++) {
			glbMap[r + 1][c + 1] = mBoard[r][c];
		}
	}
	memset(mapVisit, 0, sizeof(mapVisit));
	memset(qVisit, 0, sizeof(qVisit));


	for (int r = 1; r <= N; r++) {
		for (int c = 1; c <= N; c++) {
			if (mapVisit[r][c] == true) continue;
			mapVisit[r][c] = true;
			if (glbMap[r][c] == 0) continue;

			color = glbMap[r][c];
			Qinsert(r, c );
			int val = 0;
			if (V.size() == 4) {
				// 4개블럭 판정함수
				val = block4() - 1;
				mTetromino[val]++;
			}
			else {
				// 5개블럭 판정함수
				val = block5() - 1;
				mPentomino[val]++;
			}

		}
	}

}

void Qinsert(int r, int c) {
	Q = {}; V = {}; 
	while (1) {
		bool can = false;
		for (int i = 0; i < 4; i++) {
			if (glbMap[r + dir[0][i]][c + dir[1][i]] == color && mapVisit[r + dir[0][i]][c + dir[1][i]] == false) {
				r = r + dir[0][i];
				c = c + dir[1][i];
				mapVisit[r][c] = true;
				can = true;
				break;
			}
		}
		if (can == false)break;
	}
	int rMax = 0, cMax = 0;
	int rMin = 1000, cMin = 1000;

	Q.push({ r, c, 0 });
	qVisit[r][c] = true;
	while (!Q.empty()) {
		blk q = Q.front();
		V.push_back(q);
		rMax = max(q.r, rMax);
		rMin = min(q.r, rMin);
		cMax = max(q.c, cMax);
		cMin = min(q.c, cMin);
		Q.pop();

		for (int i = 0; i < 4; i++) {
			int r1 = q.r + dir[0][i];
			int c1 = q.c + dir[1][i];
			if (glbMap[r1][c1] == color&& qVisit[r1][c1] == false) {
				Q.push({ r1,c1, q.depth + 1 });
				mapVisit[r1][c1] = true;
				qVisit[r1][c1] = true;
			}
		}
	}
	rRange = rMax - rMin;
	cRange = cMax - cMin;

}
int dirCnt() {
	int rSub = 0, cSub = 0, dirCnt = 0;
	for (auto itr = V.begin(); itr != V.end() - 1; itr++) {
		if (rSub != (itr->r - (itr + 1)->r) || cSub != (itr->c - (itr + 1)->c)) {
			rSub = itr->r - (itr + 1)->r;
			cSub = itr->c - (itr + 1)->c;
			dirCnt++;
		}
	}
	return dirCnt - 1;
}
int block4() {
	if (V.back().depth == 2) {
		if (max(rRange, cRange) == 1) return 1;
		else return 5; //(5번 블록은 rang가 2)
	}
	else {
		if (max(rRange, cRange) == 2) {
			if (dirCnt() == 1) return 4;
			else return 3;
			//3과 4번블록 중 방향전환 카운트
		}
		else return 2; // 2번블록 range 3;
	}

}

int oneOrsix() {
	int r = V.back().r;
	int c = V.back().c;
	for (int i = 0; i < 4; i++) {
		int r1 = r + dir[0][i];
		int c1 = c + dir[1][i];
		if (glbMap[r1][c1] == color) {
			r1 = r1 + dir[0][i];
			c1 = c1 + dir[1][i];
			if (glbMap[r1][c1] == color) {
				int cnt = 0;
				for (int j = 0; j < 4; j++) {
					if (glbMap[r1 + dir[0][j]][c1 + dir[1][j]] == color)
						cnt++;
				}
				if (cnt == 2) return 1;
				else return 6;
			} 
			else return 1;
			
		}
	}
}
int block5() {
	if (V.back().depth == 2) {
		if (rRange != cRange)return 5;
		else return 10;
	}
	else if (V.back().depth == 3) {
		if (max(rRange, cRange) == 3) return 11;
		else if (rRange != cRange) return 5;
		else {
			return oneOrsix();
			// 1과 6번 블럭 중
		}
	}
	else {
		if (max(rRange, cRange) == 4) return 2;
		else if (max(rRange, cRange) == 3) {
			if (dirCnt() == 1) return 3;
			else return 4;
			// 3과 4번 블록 중 방향전환
		}
		else if (rRange == cRange) {
			int cnt = dirCnt();
			if (cnt == 1) return 8;
			else if (cnt == 2) return 12;
			else return 9;
			// 8, 9 , 12번 블록 중  방향전환
		}
		else return 7;
	}

}
