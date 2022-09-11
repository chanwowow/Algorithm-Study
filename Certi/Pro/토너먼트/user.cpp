extern void swapOrder(int mIndexA, int mIndexB);

#include <iostream>
#include <memory.h>
#include <math.h>
using namespace std;

int solution[131072];
int r2idx[131075];

void makin(int M, int interval, int n) {

	for (int idx = 0; idx < M; idx += interval) {
		solution[idx + interval / 2] = solution[idx] + pow(2, n);
	}
	interval /= 2;
	if (interval == 1) return;
	makin(M, interval, ++n);

}

void modifyTournament(int N, int mOrder[]){
	// 초기화
	memset(solution, 0, sizeof(solution));
	memset(r2idx, 0, sizeof(r2idx));

	// 초기설정
	for (int idx = 0; idx < N; idx++) {
		r2idx[mOrder[idx]] = idx;
	}
	int ini[4] = { 1,4,3,2 };
	int interval = N / 4;

	int idx = 0;
	for (int i = 0; i < 4; i++) {
		solution[idx] = ini[i];
		idx += interval;
	}
	if(N!=4) makin(N, interval, 2);
	cout << "Solution" << endl;
	for (auto itr : solution) cout << itr;
	for (int idx = 0; idx < N; idx++) {
		if (r2idx[solution[idx]] != idx) {
			int wrong_idx = r2idx[solution[idx]];
			int right_idx = idx;
			swapOrder(right_idx, wrong_idx);

			swap(mOrder[wrong_idx], mOrder[right_idx]);
			swap(r2idx[mOrder[wrong_idx]], r2idx[mOrder[right_idx]]);

		}
	}


}

