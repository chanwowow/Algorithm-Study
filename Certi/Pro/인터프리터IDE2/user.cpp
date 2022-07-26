#include <string>
#include <stack>;
#include <unordered_map>
#include <iostream>
/// ���� string �߰����� ���� ����� ����� ������?///
using namespace std;

unordered_map<char, int> charTable; bool did;
stack <int> var[26]; /// A_Z �ֽŰ� ����
string word;
int cur; // n��° ��;
int max_cur; // Ŀ�� ����
int gidx, gidx_max;

void lineWork(int workKind);
int calc();

void init() {

	if (!did) {
		for (int i = 0; i < 26; i++) {
			charTable['A' + i] = 100 + i;
		}
		for (int i = 0; i < 10; i++) {
			charTable['0' + i] = i;
		}
	}
	for (int i = 0; i < 26; i++) {
		var[i] = {}; var[i].push(0);
	}
	word.clear();
	for (int i = 0; i < 200; i++) word.push_back('?');
	cur = 1;
	max_cur = 1;
}

void destroy() {

}

int addCommand(char mCommand[]) {
	int idx = cur * 200;
	string Input = mCommand;
	int needN = 200 - Input.size();
	for (int i = 0; i < needN; i++) Input.push_back('?');

	word.insert(idx, Input);
	++max_cur;

	return cur;
}

int moveCursor(int mPos) {
	if (mPos > 0) {
		while (mPos > 0) {
			if (cur == max_cur)break;
			gidx = cur * 200; gidx_max = gidx + 199;
			lineWork(0); //0 ����
			cur++;
			mPos--;
		}
	}
	else if (mPos < 0) {
		while (mPos <= 0) {
			if (cur == max_cur) { cur--; mPos++;  continue; }
			if (cur == 0) { break; }
			gidx = cur * 200;
			lineWork(1); //1 ����
			cur--;
			mPos++;
		}
		cur++;
	}
	return cur;
}

void eraseCommand() {
	if (cur == max_cur)return; // ���� (�������� ) �� skip

	int idx = cur * 200;
	auto itr = word.begin();

	word.erase(itr + idx, itr + (idx + 200));
	--max_cur;
}

int getValue(char mVariable) {
	int varIdx = mVariable - 'A';
	return var[varIdx].top();
}


void lineWork(int workKind) {

	//int idx = gidx;
	int varList[26] = {};

	int imax = 0;
	for (imax; imax < 26;) {
		if (word[gidx] == '=') {
			gidx++;  break;
		}
		if (word[gidx] == ',') {
			gidx++; continue;
		}
		varList[imax] = word[gidx++] - 'A';
		imax++;
	}

	if (workKind == 0) {
		// �־��� �������� ���� ���Ѵ�.
		for (int i = 0; i < imax; i++) {
			var[varList[i]].push(calc());
		}
	}
	else {// �־��� �������� ���� �ǵ�����.
		for (int i = 0; i < imax; i++) {
			var[varList[i]].pop();
		}
	}
	//gidx = idx;
}

int calc() {
	int result = 0; int job; int back;

	while (gidx < gidx_max) {
		char chk = word[gidx++];

		if (chk == '(') result = calc();
		else if (chk == ')' || chk == '?' || chk == ',') break;

		else if (chk == '+') result = result + calc();
		else if (chk == '-') result = result - calc();
		else if (chk == '*') result = result * calc();
		else if (chk == '/')result = result / calc();
		else {
			int num = charTable[chk];
			if (num >= 100) { // ������ ��쿡�� �ֽŰ� �ҷ���
				num = var[num - 100].top();
			}
			result = num;
		}

		if (result < 0) result = 0;
		else if (result >= 10000) result %= 10000;
	}
	return result;
}