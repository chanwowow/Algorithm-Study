#define MAX_MAP 22

#include <queue>
#include <vector>
#include <math.h>
#include <memory.h>
#include <unordered_map>

#include <iostream>

using namespace std;

int G_N, G_M, G_SL; // 글로벌 사용목적 N, M , 해수면

int MAP[MAX_MAP][MAX_MAP];
bool VISIT[MAX_MAP][MAX_MAP];
int dir[2][4] = {
	{0, 1, 0, -1},
	{1, 0, -1, 0}
};

struct pos {
	int x, y;
} my_Que1[500];
int head1, tail1;

vector <int> fwd_Bar, rev_Bar;
struct bar_q {
	int x, y;
	int dir, fw_rev;  // 가로0세로1, fw0 rev1
} can_Case[5200];
int head2, tail2;
int candidate_N;

unordered_map <int, int> Shiball;

int calc_Flood() {
	// 침수영역 갯수 세는 함수
	memset(VISIT, 0, sizeof(VISIT));
	head1 = tail1 = 0;
	my_Que1[tail1++] = { 0,0 };
	int x, y, X, Y;
	int flood_n = 0;

	while (head1 != tail1) {
		x = my_Que1[head1].x;
		y = my_Que1[head1].y;
		//cout << " HERE  1" << endl;
		for (int i = 0; i < 4; i++) {
			X = x + dir[0][i];
			Y = y + dir[1][i];
			
			// 방문했거나 ,범위밖이면 뺌
			if (VISIT[X][Y] == true) continue;
			if (X < 0 || X >= G_N + 2 || Y < 0 || Y >= G_N + 2) continue;
			
			// 조건 맞는애들 중, 해수면 아래거나 바다인곳들은 추가
			if (MAP[X][Y] < G_SL || MAP[X][Y] == 0) {
				my_Que1[tail1++] = { X, Y };
				VISIT[X][Y] = true;
				flood_n++;
			} 
		}

		head1++;
	}
	//cout << "FLOOD N : " << flood_n << endl;
	return (pow(G_N + 2, 2) -flood_n );
}

void bar_Placing(int balanced, int candidate) {
	//1. 정방향
	head2 = tail2 = 0;
	candidate_N = 0;
	int check_f = 0;
	int x, y;
	for (int i = 1; i <= G_N; i++) {
		for (int j = 1; j <= G_N-G_M+1; j++) {
			y = j; check_f = 0;

			for (auto& bar : fwd_Bar) {
				if (check_f == 0) { check_f = MAP[i][y++] + bar; continue; }
				if (check_f != MAP[i][y++] + bar) {
					check_f = -1; // 불가하다
					break;
				}
			}
			if (check_f != -1) {
				if (candidate) candidate_N++;
				else can_Case[tail2++] = { i, j, 0, 0 };
			} 
		}
	}
	if (fwd_Bar.size() == 1) return;
	for (int i = 1; i <= G_N - G_M +1 ; i++) {
		for (int j = 1; j <= G_N; j++) {
			x = i; check_f = 0;
			for (auto& bar : fwd_Bar) {
				if (check_f == 0) { check_f = MAP[x++][j] + bar; continue; }
				if (check_f != MAP[x++][j] + bar) {
					check_f = -1; // 불가하다
					break;
				}
			}
			if (check_f != -1) {
				if (candidate) candidate_N++;
				else can_Case[tail2++] = { i, j, 1, 0 };
			}
		}
	}
	if (balanced == 1) return;
	// 2. 역방향 placing
	for (int i = 1; i <= G_N; i++) {
		for (int j = 1; j <= G_N - G_M +1; j++) {
			y = j; check_f = 0;
			//cout << "NEW Position" << endl;
			for (auto& bar : rev_Bar) {
				if (check_f == 0) { check_f = MAP[i][y++] + bar; continue; }
				if (check_f != MAP[i][y++] + bar) {
					check_f = -1; // 불가하다
					break; 
				}
			}
			if (check_f != -1) {
				if (candidate) candidate_N++;
				else can_Case[tail2++] = { i, j, 0, 1 };
			}
		}
	}
	for (int i = 1; i <= G_N - G_M +1; i++) {
		for (int j = 1; j <= G_N; j++) {
			x = i; check_f = 0;
			for (auto& bar : rev_Bar) {
				if (check_f == 0) { check_f = MAP[x++][j] + bar; continue; }
				if (check_f != MAP[x++][j] + bar) {
					check_f = -1; // 불가하다
					break;
				}
			}
			if (check_f != -1) {
				if (candidate) candidate_N++;
				else can_Case[tail2++] = { i, j, 1, 1 };		
			}
		}
	}

}

void init(int N, int mMap[20][20]){
	G_N = N;
	memset(MAP, 0, sizeof(MAP));

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			MAP[i][j] = mMap[i - 1][j - 1];
		}
	}
	Shiball.clear();
}

int numberOfCandidate(int M, int mStructure[5]){
	int idx = 0;
	int j = 0;
	for (int i = M-1; i >= 0; i--) {
		idx += mStructure[i] * pow(10, j); j++;
	}
	if (Shiball[idx] != 0) { return Shiball[idx]; }
	/*
	char arr1[5] = { mStructure[0]+96, mStructure[1] + 96,mStructure[2] + 96,mStructure[3] + 96, mStructure[4] + 96 };
	if (Shiball[arr1] != 0) {  return Shiball[arr1]; } */  // ㅅㅂ 이거 왜 깨지냐??? 한자리 일때 꺠지는거같은데 >>
	G_M = M;
	fwd_Bar.clear();
	rev_Bar.clear();
	for (int i = 0; i < M; i++) fwd_Bar.push_back(mStructure[i]);
	for (int i = M - 1; i >= 0; i--)rev_Bar.push_back(mStructure[i]);
	
	int balanced = 1; // 0:비대칭구조, 1:대칭구조
	auto rev = rev_Bar.begin();
	for (auto& fwd : fwd_Bar) {
		if (fwd != *rev++) {
			balanced = 0; break;
		}
	}

	bar_Placing(balanced, 1); 
	//cout << "## Candidate N : " << candidate_N << endl << endl;;
	Shiball[idx] = candidate_N;
	return candidate_N;
}

int maxArea(int M, int mStructure[5], int mSeaLevel){
	G_M = M;
	G_SL = mSeaLevel;
	fwd_Bar.clear();
	rev_Bar.clear();
	for (int i = 0; i < M; i++) fwd_Bar.push_back(mStructure[i]);
	for (int i = M - 1; i >= 0; i--)rev_Bar.push_back(mStructure[i]);

	int balanced = 1; // 0:비대칭구조, 1:대칭구조
	auto rev = rev_Bar.begin();
	for (auto& fwd : fwd_Bar) {
		if (fwd != *rev++) balanced = 0; break;
	}

	bar_Placing(balanced, 0);

	int max_Area = 0;
	if (head2==tail2) max_Area= -1;
	int result = 0;
	bar_q temp;

	while (head2 != tail2) {
		temp = can_Case[head2];
		head2++;
		int x = temp.x;
		int y = temp.y;
		//cout << "TEMP INFO : " << temp.x << temp.y << temp.dir << temp.fw_rev << endl;
		if (temp.dir == 0) {
			if (temp.fw_rev == 0) {
				for (auto& bar : fwd_Bar) MAP[x][y++] += bar;
				result = calc_Flood();
				max_Area = (max_Area > result) ? max_Area : result;
				// 다시 MAP에 복구시킴
				y = temp.y;
				for(auto& bar : fwd_Bar) MAP[x][y++] -= bar;
			}
			else {
				for (auto& bar : rev_Bar) MAP[x][y++] += bar;
				result = calc_Flood();
				max_Area = (max_Area > result) ? max_Area : result;
				// 다시 MAP에 복구시킴
				y = temp.y;
				for (auto& bar : rev_Bar) MAP[x][y++] -= bar;
			}
		}
		else {
			if (temp.fw_rev == 0) {
				for (auto& bar : fwd_Bar) MAP[x++][y] += bar;
				result = calc_Flood();
				max_Area = (max_Area > result) ? max_Area : result;
				// 다시 MAP에 복구시킴
				x = temp.x;
				for (auto& bar : fwd_Bar) MAP[x++][y] -= bar;
			}
			else {
				for (auto& bar : rev_Bar) MAP[x++][y] += bar;
				result = calc_Flood(); 
				max_Area = (max_Area > result) ? max_Area : result;
				// 다시 MAP에 복구시킴
				x = temp.x;
				for (auto& bar : rev_Bar) MAP[x++][y] -= bar;
			}
		}
	}

	return max_Area;
}