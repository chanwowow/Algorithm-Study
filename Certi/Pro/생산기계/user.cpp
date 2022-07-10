const int macN = 24005;

#include <queue>
#include <unordered_map>
#include <memory.h>
#include <set>

using namespace std;

unordered_map <int, int> hashMap; int nu_hID;

struct myStr1 {
	int yTime, ID;
};
myStr1 macDB[macN]; // hID�Է��ϸ� ����ð�(�ֽŹ���, �� ID�� �˼��ִ� DB);
struct cmp1 {
	bool operator()(myStr1 a, myStr1 b) const {
		if (a.yTime == b.yTime) return a.ID > b.ID;
		return a.yTime > b.yTime;
 	}
};
set <myStr1, cmp1> SET;

int add(int mId, int mTime);

void init(int N, int mId[], int mTime[]) {
	// �ʱ�ȭ
	hashMap = {};
	nu_hID = 1;
	SET = {};
	// �ʱⰪ
	for (int i = 0; i < N; i++) {
		add(mId[i], mTime[i]);
	}

}

int add(int mId, int mTime) {
	int hID = hashMap[mId];
	if (hID == 0) { // DB�� ���� ���̸�
		hID = hashMap[mId] = nu_hID++;

		macDB[hID] = { mTime, mId };
		SET.insert(macDB[hID]);
	}
	else {
		SET.erase(macDB[hID]);

		macDB[hID].yTime = mTime;
		SET.insert(macDB[hID]);
	}
	
	return SET.size();
}

int remove(int K) {
	int ret = 0;

	while (K > 0) {
		auto itr = SET.begin();

		hashMap.erase(itr->ID);
		SET.erase(itr);

		--K;
	}

	return SET.begin()->ID;
}

int produce(int M) {
	// binarySearch (int M) M : ������ ����� ��. mid ����ð�
	int right = M * 3000;
	int left = 1;
	int ret = right;

	register int mid = 0;
	register int yN = 0;
	

	while (right >= left) {
		yN = 0;
		mid = (right + left) >> 1;

		for (auto &itr : SET) {
			yN += mid / itr.yTime;
		}

		if (yN >= M) {
			ret =mid;
			right = mid - 1;

		}
		else {
			left = mid + 1;
		}

	}

	return ret;
}