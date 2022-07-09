extern bool checkCards(int mIndexA, int mIndexB, int mDiff);

#include <vector>
#include<iostream>


using namespace std;

vector <int> offsetG[50001];
int offsetMax;

void playGame(int N) {
	// 초기화 코드
	for (int i = 0; i <= offsetMax; i++) {
		offsetG[i].clear();
	}
	offsetMax = 0;

	for (int idx = 1; idx < 2 * N; idx++) {

		// idx에 해당하는 놈이 몇 차이인지 binary search
		int left = 0;
		int right = N - 1; // 차이값의 최대
		int mid;
		int offset = 50000;
		
		//mid 값이 0인 경우도 주의하라
		while (left <=  right) {
			mid = (left + right) / 2;
			if (checkCards(0, idx, mid)) {
				right = mid - 1;
				offset = min(mid, offset);
			}
			else 
				left = mid + 1;
		}
		offsetG[offset].push_back(idx);
		offsetMax = max(offset, offsetMax);
	}

	for (int offset=1; offset <= offsetMax; offset++) {
		if (offsetG[offset].size() == 4) {
			int second[2] = {}; int idx = 0;
			for (int i = 1; i < 4; i++) {
				if (checkCards(offsetG[offset][0], offsetG[offset][i], 0) == false)
					second[idx++] = offsetG[offset][i];
			}
			checkCards(second[0], second[1], 0);
		}
		else {
			checkCards(offsetG[offset][0], offsetG[offset][1], 0);
		}
	}




}