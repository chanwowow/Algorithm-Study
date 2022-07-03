#include<map>
#include<set>

using namespace std;

map<int, int> usingSpace; // first�� �����ּ�, second�� size
struct eInfo {
	int eSize; int eAddr;
};
struct cmp { // �������, �ּҼ��� ��������
	bool operator()(eInfo a, eInfo b) const {
		if (a.eSize == b.eSize) return a.eAddr < b.eAddr;
		return a.eSize < b.eSize;
	}
};
set <eInfo, cmp> freeSpace;
int gN;

void init(int N) {
	//�ʱ�ȭ
	usingSpace.clear();
	freeSpace.clear();

	gN = N;
	freeSpace.insert({ N, 0 });
}


int allocate(int mSize) {

	auto low = freeSpace.lower_bound({ mSize, 0 });
	if (low == freeSpace.end()) return -1;

	int eSize = low->eSize;
	int eAddr = low->eAddr;
	freeSpace.erase({ eSize, eAddr });

	// 1. �Ҵ��ϰ� ���� ���� ������ �ٽó־���
	int leftSize = eSize- mSize;
	if (leftSize > 0) {
		int leftsAddr = eAddr + mSize;
		freeSpace.insert({ leftSize, leftsAddr });
	}
	// 2. �Ҵ������ ���� ��������
	usingSpace[eAddr] = mSize;

	return eAddr;
}

int release(int mAddr) {
	auto mapItr = usingSpace.find(mAddr); // ����������ġ
	if (mapItr == usingSpace.end()) return -1;
	int fSize = mapItr->second;
	int frontAddr; int frontSize;
	bool eraseEn = false;

	// 1. �հ��� �����ϱ�
	if (mapItr == usingSpace.begin()) {
		if (mAddr == 0) {
			eraseEn = false;
			frontAddr = mAddr;
			frontSize = 0;
		}
		else {
			eraseEn = true;
			frontAddr = 0;
			frontSize = mAddr;
		}
	}
	else {
		mapItr--;
		if (mAddr == mapItr->first + mapItr->second) {
			eraseEn = false;
			frontAddr = mAddr;
			frontSize = 0;
		}
		else {
			eraseEn = true;
			frontAddr = mapItr->first + mapItr->second;
			frontSize = mAddr - frontAddr;
		}
	}
	if (eraseEn) freeSpace.erase({ frontSize, frontAddr });


	// 2/ �ް��� ����
	int backAddr = mAddr + fSize;
	int backSize;
	auto low = usingSpace.lower_bound(backAddr);
	if (backAddr == gN || (low!=usingSpace.end()&& backAddr==low->first)) {
		// ��ûũ�����̶� ���ų�, ������� �����̶��
		backSize = 0;
	}
	else {
		if (low == usingSpace.end()) backSize = gN - backAddr;
		else backSize = low->first - backAddr;

		freeSpace.erase({ backSize, backAddr });
	}
	freeSpace.insert({ frontSize + fSize + backSize, frontAddr });

	usingSpace.erase(mAddr);// ���� �Ҵ� ��Ͽ����� ����.
	return fSize;
}


