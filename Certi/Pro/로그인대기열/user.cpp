#include <map>
#include <string>
#include <memory.h>


using namespace std;

map<string, int> MP; // ID -> ( ��⿭ idx )

string w_List[50001];
int head, tail;

void init(){
	//�ʱ�ȭ �ڵ�
	MP.clear();
	for (int i = 1; i < tail; i++) {
		w_List[i] = "";
	}
	head = tail = 1;
}

void loginID(char mID[10]){
	int idx = MP[mID];
	if (idx != 0) w_List[idx] = ""; // �̹� ������̾��ٸ� ������⿭ ����
	MP[mID] = tail; // 1. ID����
	w_List[tail] = mID; // 2. ��⿭���� ����

	tail++;
}

int closeIDs(char mStr[10]){
	int result = 0;
	string str2, ID;
	int str_size = strlen(mStr);

	// 1. �Է��� ABC�� str2=ABD ����� ����
	for (int i = 0; i < str_size - 1; i++) 	str2 += mStr[i];
	str2 += mStr[str_size - 1] + 1;
	//cout << "## Str 1 , str2 : " << mStr << "  " << str2 << endl;
	// 
	// 2. ABC�� ABD ���� str �� �̴� ����
	int chk = MP[mStr];
	MP[str2];

	auto itr1 = MP.find(mStr);
	auto itr2 = MP.find(str2);
	if (chk == 0) itr1++; // �ش��ʼ��� �ϼ��� ���ڰ� �������� �ǳʶ�.
	// 3. �ش��� ��⿭���� ����

	while (itr1 != itr2) {
		ID = itr1->first;
		//cout << endl << " Erased : " << ID << endl << endl;
		
		w_List[itr1->second] = ""; // ��⿭ ����
		itr1++;  // �� ���� �̰� ��������.. delete�ϸ� itr���󰡳�
		MP.erase(ID); // ���̺����� ���� [�ƿ� ������ ����?idx�� 0 ����� 
		
		result++; // ����� ���
		
	}
	
	if (chk == 0) MP.erase(mStr);
	MP.erase(str2); 
	//cout << "## Chosung Erase N : "<<result << endl;
	return result;
}

void connectCnt(int mCnt){
	string ID;
	for (head; head < tail; head++) {
		if (w_List[head] == "")continue; // ��������� ��ŵ
		ID = w_List[head];
		MP.erase(ID);
		w_List[head] = "";

		if (--mCnt == 0) break;
	}
}

int waitOrder(char mID[10]){
	int deleted = 0;
	int q_idx = MP[mID];

	// ��⿭�� ���³��̸�
	if (q_idx == 0) {
		MP.erase(mID);
		//cout << " X exist, result 0" << endl;
		return  0;
	}
	// ��⿭�� �ִ³��̸� head�ͻ��̿� ��� ����� ����
	for (int i = head; i < q_idx; i++) {
		if (w_List[i] == "") deleted++;
	}
	
	int result = q_idx - head - deleted +1 ;
	//cout <<endl<<" ### DaegiYeol n th : " << result << "HEAD :"<<head << endl;
	return result;
}


