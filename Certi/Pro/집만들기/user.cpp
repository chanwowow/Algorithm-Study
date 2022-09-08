const int maxN = 122;
const int maxPut = 8005;

#include <iostream>
#include <memory.h>
#include <queue>
#include <unordered_set>

using namespace std;

int map[maxN][maxN]; // ID �Ҵ����ش�
int newID;

// UnionFind��
int parent[maxPut];
int findParent(int ID) {
	int id = ID;
	while (parent[id] != id) id = parent[id];
	parent[ID] = id;//����
	return id;
}
// BFS Ž����
int VISIT[maxN][maxN];
struct mp { int r, c; };
//queue<mp> Q;
mp Q2[15000]; int head, tail;

int dir[2][8] = {
	{-1, -1, -1, 0, 1, 1, 1, 0},
	{-1, 0, 1, 1, 1,0, -1, -1}
};
int dir2[2][4] = {
	{-1,0,1,0},
	{0, 1, 0,-1}
};

int Nplus1;
void init(int N){
	Nplus1 = N + 1;
	memset(map, 0, sizeof(map));
	memset(parent, 0, sizeof(parent));
	newID = 1;
}

int BFS(int I);
int glb_pID;
int put(int mRow, int mCol){
	int ID = newID++;
	map[mRow][mCol] = ID;
	parent[ID] = ID;

	// 8�� Ž��
	unordered_set <int> SET;
	glb_pID = 0;
	

	for (register int i = 0; i < 8; i++) {
		int r = mRow + dir[0][i];
		int c = mCol + dir[1][i];

		if (r == 0 || r == Nplus1 || c == 0 || c == Nplus1) continue;
		if (map[r][c] == 0) continue;
		int chkID = findParent(map[r][c]);
		if (SET.find(chkID) != SET.end()) {
			// ���� 2�� ���°Ÿ� �̳��̴�!
			glb_pID = chkID;
		}
		SET.insert(chkID);
	}
	for (auto itr : SET) parent[itr] = ID; // ������� �ϴ� �뵿�ܰ�!

	int size = 0;
	if (glb_pID != 0) {
		// ������� �ִٸ� �Ʒ�����
		glb_pID = ID;
		memset(VISIT, 0, sizeof(VISIT));

		for (int i = 0; i < 4; i++) {
			int r = mRow + dir2[0][i];
			int c = mCol + dir2[1][i];
			if (r == 0 || r == Nplus1 || c == 0 || c == Nplus1) continue;
			if (map[r][c] != 0) continue;

			VISIT[mRow][mCol] = -i -1;
			VISIT[r][c] = -i - 1;
			Q2[0] = { r,c };

			size = BFS(-i - 1);
			if (size != 0) break;
		}
	}
	return size;
}

int BFS(int I) {
	register int size = 1;
	int pID = glb_pID;
	bool cant = false;
	/*
	* while (!Q.empty()) {
		mp q = Q.front();
		Q.pop();

		for (register int i = 0; i < 4; i++) {
			int r = q.r + dir2[0][i];
			int c = q.c + dir2[1][i];

			if (VISIT[r][c] != 0) {
				if (VISIT[r][c] == I) continue;
				cant = true; break;
			}
			if (r == 0 || r == Nplus1 || c == 0 || c == Nplus1) { cant = true; break; }// ��ȿ�ϴ�!!!
			if (map[r][c] != 0) {// ���� ������
				if (pID != findParent(map[r][c])) { cant = true; break; }// ��ȿ�ϴ�!!!
				continue; // �ƴϸ� ������ �Ѿ
			}
			// ��ĭ�̸�
			VISIT[r][c] = I;
			++size;
			Q.push({ r,c });
		}
		if (cant) { 
			size = 0; break; 
		}
	}
	*/
	
	register int hd = 0;
	register int tl = 1;

	while (hd!=tl) {
		mp q = Q2[hd++];

		for (register int i = 0; i < 4; i++) {
			register int r = q.r + dir2[0][i];
			register int c = q.c + dir2[1][i];

			if (VISIT[r][c] != 0) {
				if (VISIT[r][c] == I) continue;
				cant = true; break;
			}
			if (r == 0 || r == Nplus1 || c == 0 || c == Nplus1) { cant = true; break; }// ��ȿ�ϴ�!!!
			if (map[r][c] != 0) {// ���� ������
				if (pID != findParent(map[r][c])) { cant = true; break; }// ��ȿ�ϴ�!!!
				continue; // �ƴϸ� ������ �Ѿ
			}
			// ��ĭ�̸�
			VISIT[r][c] = I;
			++size;
			Q2[tl++] = { r,c };
		}
		if (cant) {
			size = 0; break;
		}
	}

	return size;
}