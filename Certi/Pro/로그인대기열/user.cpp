#include <map>
#include <string>
#include <memory.h>


using namespace std;

map<string, int> MP; // ID -> ( 대기열 idx )

string w_List[50001];
int head, tail;

void init(){
	//초기화 코드
	MP.clear();
	for (int i = 1; i < tail; i++) {
		w_List[i] = "";
	}
	head = tail = 1;
}

void loginID(char mID[10]){
	int idx = MP[mID];
	if (idx != 0) w_List[idx] = ""; // 이미 대기중이었다면 먼저대기열 삭제
	MP[mID] = tail; // 1. ID삽입
	w_List[tail] = mID; // 2. 대기열에도 삽입

	tail++;
}

int closeIDs(char mStr[10]){
	int result = 0;
	string str2, ID;
	int str_size = strlen(mStr);

	// 1. 입력이 ABC면 str2=ABD 만드는 과정
	for (int i = 0; i < str_size - 1; i++) 	str2 += mStr[i];
	str2 += mStr[str_size - 1] + 1;
	//cout << "## Str 1 , str2 : " << mStr << "  " << str2 << endl;
	// 
	// 2. ABC와 ABD 사이 str 다 뽑는 과정
	int chk = MP[mStr];
	MP[str2];

	auto itr1 = MP.find(mStr);
	auto itr2 = MP.find(str2);
	if (chk == 0) itr1++; // 해당초성이 완성인 글자가 없을때만 건너뜀.
	// 3. 해당놈들 대기열에서 삭제

	while (itr1 != itr2) {
		ID = itr1->first;
		//cout << endl << " Erased : " << ID << endl << endl;
		
		w_List[itr1->second] = ""; // 대기열 삭제
		itr1++;  // 와 ㅅㅂ 이거 시점주의.. delete하면 itr날라가네
		MP.erase(ID); // 테이블에서도 삭제 [아예 지우지 말까?idx만 0 만들게 
		
		result++; // 지운갯수 계산
		
	}
	
	if (chk == 0) MP.erase(mStr);
	MP.erase(str2); 
	//cout << "## Chosung Erase N : "<<result << endl;
	return result;
}

void connectCnt(int mCnt){
	string ID;
	for (head; head < tail; head++) {
		if (w_List[head] == "")continue; // 비어있으면 스킵
		ID = w_List[head];
		MP.erase(ID);
		w_List[head] = "";

		if (--mCnt == 0) break;
	}
}

int waitOrder(char mID[10]){
	int deleted = 0;
	int q_idx = MP[mID];

	// 대기열에 없는놈이면
	if (q_idx == 0) {
		MP.erase(mID);
		//cout << " X exist, result 0" << endl;
		return  0;
	}
	// 대기열에 있는놈이면 head와사이에 빈놈 몇개인지 센다
	for (int i = head; i < q_idx; i++) {
		if (w_List[i] == "") deleted++;
	}
	
	int result = q_idx - head - deleted +1 ;
	//cout <<endl<<" ### DaegiYeol n th : " << result << "HEAD :"<<head << endl;
	return result;
}


