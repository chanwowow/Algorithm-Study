const int maxCity = 603;

#include <unordered_map>
#include <queue>
#include <memory.h>

using namespace std;

unordered_map <int, int> cityHash; int nu_hID;
struct myStr1 {
	int cost, city;
};
vector<myStr1> fwdWay[maxCity];
vector<myStr1> revWay[maxCity];

int minCost[maxCity]; bool visit[maxCity];

struct cmp1 {
	bool operator()(myStr1 a, myStr1 b) {
		if (a.cost == b.cost) return a.city > b.city;
		return a.cost > b.cost;
	}
};
priority_queue <myStr1, vector<myStr1>, cmp1> PQ;



int init(int N, int sCity[], int eCity[], int mCost[]) {
	//초기화
	cityHash = {};
	for (int i = 0; i < nu_hID; i++) {
		fwdWay[i] = revWay[i] = {};
	}
	nu_hID = 1;
	for (int i = 0; i < N; i++) {
		int s_hID = cityHash[sCity[i]];
		if (s_hID == 0) s_hID = cityHash[sCity[i]] = nu_hID++;
		int e_hID = cityHash[eCity[i]];
		if (e_hID == 0) e_hID = cityHash[eCity[i]] = nu_hID++;

		fwdWay[s_hID].push_back({ mCost[i],e_hID });
		revWay[e_hID].push_back({ mCost[i],s_hID });
	}
	return nu_hID - 1;
}

void add(int sCity, int eCity, int mCost) {
	int s_hID = cityHash[sCity];
	int e_hID = cityHash[eCity];
	
	fwdWay[s_hID].push_back({ mCost, e_hID });
	revWay[e_hID].push_back({ mCost, s_hID });
}


int cost(int mHub) {
	int m_hID = cityHash[mHub];
	register int ret = 0;

	memset(visit, 0, sizeof(visit)); 

	// 1. 정방향탐색 실행.
	PQ = {}; PQ.push({ 0, m_hID });

	while (!PQ.empty()) {
		myStr1 top = PQ.top(); PQ.pop();
		if (visit[top.city] == true) continue;
		visit[top.city] = true;
		minCost[top.city] = top.cost;

		for (auto& itr : fwdWay[top.city]) {
			if (visit[itr.city] == true) continue;

			PQ.push({ top.cost + itr.cost, itr.city });
		}
		
	}
	for (register int i = 1; i < nu_hID; i++) {
		ret += minCost[i];
	}

	// 2. 역방향 탐색
	memset(visit, 0, sizeof(visit));
	PQ = {}; PQ.push({ 0, m_hID });

	while (!PQ.empty()) {
		myStr1 top = PQ.top(); PQ.pop();
		if (visit[top.city] == true) continue;
		visit[top.city] = true;
		minCost[top.city] = top.cost;

		for (auto& itr : revWay[top.city]) {
			if (visit[itr.city] == true) continue;

			PQ.push({ top.cost + itr.cost, itr.city });
		}

	}
	for (register int i = 1; i < nu_hID; i++) {
		ret += minCost[i];
	}
	return ret;
}

