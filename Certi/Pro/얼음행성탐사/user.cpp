#include <set>
#include <queue>
#include <memory.h>
#include <iostream>

#define MAX_ROW 705
#define MAX_COL 10005

using namespace std;

set<int> rBlock[MAX_ROW];
set<int> cBlock[MAX_COL];

struct mp {
	int r; int c;
	int collision;
};
//queue<mp> Q;
mp sQ[700000];
int head;
int tail;
bool Visit[MAX_ROW][MAX_COL];

int maxR;
int maxC;


void init(int R, int C, int M, int mStructureR[], int mStructureC[]){
	for (int i = 0; i < maxR; i++) rBlock[i].clear();
	for (int i = 0; i < maxC; i++) cBlock[i].clear();
	maxR = R; maxC = C;

	for (int i = 0; i < M; i++) {
		rBlock[mStructureR[i]].insert(mStructureC[i]);
		cBlock[mStructureC[i]].insert(mStructureR[i]);
	}

}

int minDamage(int mStartR, int mStartC, int mEndR, int mEndC){
	int result = -1;
	mp q;

	memset(Visit, 0, sizeof(Visit));
	if (mStartR == mEndR && mStartC == mEndC) return 0;
	//1. BFS 시작
	head = tail = 0;
	sQ[tail++] = { mStartR, mStartC,0 };
	
	while (head!=tail) {
		q = sQ[head++];

		for (int i = 0; i < 4; i++) {
			if (i == 0) { // -> 이동
				auto landP = rBlock[q.r].upper_bound(q.c);
				// 1. 바다에 빠진다면 
				if (landP == rBlock[q.r].end()) {
					if (q.r == mEndR && q.c < mEndC) {
						result = q.collision;
						break;
					}continue;
				}
				if (*landP == q.c + 1) continue;  // 2. 이동 불가하다면

				// 3. 바다엔 안빠졌고 구조물 부딪혔다면
				if (q.r == mEndR && q.c < mEndC && mEndC < *landP) {
					result = q.collision;
					break;
				}
				// viist 한거 체크 후 안했다면 저장
				if (Visit[q.r][*landP-1] != true) {
					Visit[q.r][*landP-1] = true;
					sQ[tail++] = { q.r,*landP - 1, q.collision + 1 };
				}

			}
			else if (i == 1) { // V 이동
				auto verP = cBlock[q.c].upper_bound(q.r);

				if (verP == cBlock[q.c].end()) {
					if (q.c == mEndC && q.r < mEndR) {
						result = q.collision;
						break;
					}continue;
				}
				if (*verP == q.r + 1)continue;

				if (q.c == mEndC && q.r < mEndR && mEndR < *verP) {
					result = q.collision;
					break;
				}

				if (Visit[*verP-1][q.c] != true) {
					Visit[*verP-1][q.c] = true;
					sQ[tail++] = { *verP - 1, q.c, q.collision + 1 };
				}

			}
			else if (i == 2) { //<- 이동

				auto landM = rBlock[q.r].upper_bound(q.c);
				// 1. 바다에 빠진다면
				if (landM == rBlock[q.r].begin()) {
					if (q.r == mEndR && q.c > mEndC) {
						result = q.collision;
						break;
					}continue;
				}
				landM--;
				if (*landM == q.c - 1) continue;  // 2. 이동 불가하다면

				// 3. 구조물 부딪혔다면
				if (q.r == mEndR && q.c > mEndC && mEndC > *landM) {
					result = q.collision;
					break;
				}
				// viist 한거 체크 후 안했다면 저장
				if (Visit[q.r][*landM+1] != true) {
					Visit[q.r][*landM+1] = true;
					sQ[tail++] = { q.r,*landM + 1, q.collision + 1 };
				}

			}
			else { // ^ 이동
				auto verM = cBlock[q.c].upper_bound(q.r);
				// 1. 바다에 빠진다면
				if (verM == cBlock[q.c].begin()) {
					if (q.c == mEndC && q.r > mEndR) {
						result = q.collision;
						break;
					}continue;
				}
				verM--;
				if (*verM == q.r - 1) continue;  // 2. 이동 불가하다면

				// 3. 구조물 부딪혔다면
				if (q.c == mEndC && q.r > mEndR && mEndR > *verM) {
					result = q.collision;
					break;
				}
				// viist 한거 체크 후 안했다면 저장
				if (Visit[*verM+1][q.c] != true) {
					Visit[*verM+1][q.c] = true;
					sQ[tail++] = { *verM + 1, q.c, q.collision + 1 };
				}

			}


		}
		
		if (result != -1) break; // 정답 찾았다면

	}

	return result;
}
