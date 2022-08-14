const int maxN = 100005;

#include <list>

using namespace std;

int parent[maxN];
int score[maxN];

list<int> teamMember[maxN];

int findParent(int ID) {
	register int id = ID;
	while (parent[id]!=id){
		id = parent[id];
	}
	parent[ID] = id;
	return id;
}

void init(int N){
	// 초기화 필요
	for (int i = 1; i <= N; i++) {
		parent[i] = i;
		score[i] = 0;
		teamMember[i] = {};
	}

}

void updateScore(int mWinnerID, int mLoserID, int mScore){
	int winPID = findParent(mWinnerID);
	int losPID = findParent(mLoserID);

	score[winPID] += mScore;
	score[losPID] -= mScore;
	int dummy = 1;
}

void unionTeam(int mPlayerA, int mPlayerB){
	int A_PID = findParent(mPlayerA);
	int B_PID = findParent(mPlayerB);

	if (teamMember[A_PID].size() >= teamMember[B_PID].size()) {
		parent[B_PID] = A_PID;
		register int offset = score[B_PID] -score[A_PID];
		score[B_PID] -= score[A_PID];
		
		for (auto itr : teamMember[B_PID]) {
			score[itr] += offset;
		}

		teamMember[A_PID].push_back(B_PID);
		teamMember[A_PID].splice(teamMember[A_PID].end(), teamMember[B_PID]);
	}
	else {
		parent[A_PID] = B_PID;
		register int offset = score[A_PID] - score[B_PID];
		score[A_PID] -= score[B_PID];

		for (auto itr : teamMember[A_PID]) {
			score[itr] += offset;
		}
		
		teamMember[B_PID].push_back(A_PID);
		teamMember[B_PID].splice(teamMember[B_PID].end(), teamMember[A_PID]);
	}

}

int getScore(int mID){
	int ret = 0;
	int PID = findParent(mID);

	if (PID == mID) ret = score[PID];
	else ret = score[PID] + score[mID];

	return ret;
}
