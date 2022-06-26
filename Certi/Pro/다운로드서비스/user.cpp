struct Result {
	int finish;
	int param;
};
#define INF 12345678
#define MaxCase 10005

#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <iostream>

using namespace std;
int gTime;
struct {
	bool PC;

	int motherID;
	unordered_set<int> activeChildID;
	int tpCapa;
	
	int dataLeft;
	int estTime;
	int lastCheckTime;
} dev[MaxCase]; int poolCnt;
int newHub() {
	dev[poolCnt] = {};
	return poolCnt++;
}
int newPC() {
	dev[poolCnt] = {true };
	return poolCnt++;
}

struct str1 {
	int estTime;
	int hID;
};
struct cmp {
	bool operator()(str1 a, str1 b) {
		return a.estTime > b.estTime;
	}
};
priority_queue<str1, vector<str1>, cmp> PQ;

unordered_map<int, int> hashMap;
void dnRealloc(int thisID, int newCapa);
void upDelete(int thisID);
int upAdd(int thisID);
void updating(int mTime);

void init(int mCapa){
	poolCnt = 0;
	// root 서버의 할당
	hashMap[0] = newHub();
	dev[0].motherID = -180; ///루트의 표시
	dev[0].tpCapa = mCapa;
}

void addHub(int mTime, int mParentID, int mID){
	updating(mTime);

	int hID = hashMap[mID] = newHub();
	dev[hID].motherID= hashMap[mParentID];
}

int removeHub(int mTime, int mID){
	updating(mTime);
	int hID = hashMap[mID];

	if (!dev[hID].activeChildID.empty()) return 0;
	else return 1;
	// 굳이 삭제할 필요는 없을듯 어차피 연결 없을거라
}

void requestDL(int mTime, int mParentID, int mpcID, int mSize){
	updating(mTime);
	int hID = hashMap[mpcID] = newPC();
	cout << "### PC ID " << mpcID << " : " << hID <<endl;
	dev[hID].motherID= hashMap[mParentID];
	dev[hID].dataLeft = mSize;
	dev[hID].lastCheckTime = mTime;

	upAdd(hID); /// 상향 추가
	//cout << " CAPA GOT    :" << dev[hID].tpCapa << endl;
}

Result checkPC(int mTime, int mpcID){
	updating(mTime);
	int hID = hashMap[mpcID];

	Result res;
	if (dev[hID].dataLeft == 0) {
		res.finish = 1;
		res.param = dev[hID].estTime;
	}
	else {
		res.finish = 0;
		res.param = dev[hID].dataLeft;
	}
	return res;
}


void dnRealloc(int thisID, int newCapa) {

	// PC인 경우에는
	if (dev[thisID].PC == true) {
		int dataPrev = dev[thisID].dataLeft;
		int timePrev = dev[thisID].lastCheckTime;
		dev[thisID].dataLeft =dataPrev -  dev[thisID].tpCapa * (gTime - timePrev);
		dev[thisID].lastCheckTime = gTime;

		if (dev[thisID].dataLeft <= 0) {
			dev[thisID].dataLeft = 0; return;
		}
		dev[thisID].tpCapa = newCapa; //새 capa
		if (newCapa == 0) dev[thisID].estTime = INF;
		else dev[thisID].estTime = gTime + ceil(double(dev[thisID].dataLeft) / newCapa); // 새시간
		cout << " REALLOC hID :" << thisID << "|EST Time:" << dev[thisID].estTime << " DATA LEFT :" << dev[thisID].dataLeft << endl;
		PQ.push({ dev[thisID].estTime , thisID });
		return;
	}
	// 허브인 경우에는 자식들 재귀
	int capaEach = dev[thisID].tpCapa / dev[thisID].activeChildID.size();
	for (auto itr : dev[thisID].activeChildID) {
		dev[itr].tpCapa = capaEach;
		cout << "itr:" << itr << " ";
		dnRealloc(itr, capaEach);
	}
}
void upDelete(int thisID) {
	int motherID = dev[thisID].motherID;
	// 부모로 가서 활성ID삭제
	dev[motherID].activeChildID.erase(thisID);
	// 마지막 놈이었다면 그 부모에게도 삭제
	if (dev[motherID].activeChildID.empty()) {
		if (motherID != 0) // root가 아닌 경우에만
			upDelete(dev[thisID].motherID);
	}
	dnRealloc(motherID, dev[motherID].tpCapa);
}


int upAdd(int thisID) {
	int motherID = dev[thisID].motherID;
	dev[motherID].activeChildID.insert(thisID);
	if (dev[motherID].activeChildID.size() == 1) {
		if (motherID != 0) {
			dev[motherID].tpCapa = upAdd(motherID);
		}
	}
	int capaEach=dev[motherID].tpCapa / dev[motherID].activeChildID.size();
	dnRealloc(motherID, capaEach);
	return capaEach;
	// 부모에서 나에게 줄수있는 capa 받아온다.
}


void updating(int mTime) {
	gTime = mTime;
	str1 top;

	while (!PQ.empty()) {
		top = PQ.top();
		//cout << " PQ VALUE" << top.estTime << endl;
		// 유효성체크 -> 과거것인지 체크 -> 진행
		if (dev[top.hID].estTime == top.estTime) {
			PQ.pop(); continue;
		}
		if (top.estTime > mTime) break;
		PQ.pop();

		cout << "## hID : " << top.hID << " finished at "<<top.estTime<<endl ;

		dev[top.hID].dataLeft = 0; // 완료처리
		upDelete(top.hID);
	
	}

}
