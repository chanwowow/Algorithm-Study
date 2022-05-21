
const int maxN = 5005;
const int maxGN = 505;

#include <unordered_map>

using namespace std;

struct {
	int memberN;
	int msgN;
	int msgIdx;
	char MSG[10];
} groupMsg[maxGN];

unordered_map <int, int> groupOf[maxN]; // groupOf[pID] (gID) = offset (포함되기직전 그룹 최신메세지 갯수)
//int msgRcvN[maxN];

int gMsgIdx;

void init(int N){
	gMsgIdx = 1;
	for (register int i = 1; i <= N; i++) {
		groupOf[i] = {};
	}
}

void createGroup(int groupID, int mNum, int uIDs[]){
	groupMsg[groupID] = { mNum, 0, 0, 0 };

	for (register int i = 0; i < mNum; i++) {
		groupOf[uIDs[i]][groupID] = 0;
		//msgRcvN[i] = 0;
	}
}

int sendMessage(int uID, int groupID, char mText[]){
	groupOf[uID][groupID]++; // 본인은 안받으니까

	groupMsg[groupID].msgN++;
	groupMsg[groupID].msgIdx = gMsgIdx++; // 최종인덱스 저장
	for (register int i = 0; i < 10; i++) {
		groupMsg[groupID].MSG[i] = mText[i];
		if (mText[i] == '\0') break;
	}

	return groupMsg[groupID].memberN - 1;
}

void setOnOff(int uID, int groupID){

	auto itr = groupOf[uID].find(groupID);
	// 1. 원래 off 였다면 on
	if (itr == groupOf[uID].end()) {
		groupOf[uID][groupID] = groupMsg[groupID].msgN;
		groupMsg[groupID].memberN++;
	}
	// 2. 원래 On 였다면 off
	else {
		groupOf[uID].erase(itr);
		groupMsg[groupID].memberN--;
	}
}

int checkMsg(int uID, char mMsg[]){
	register int msgRcvN = 0;
	register int msgIdx = 0;
	int gID = 0;

	for (auto &itr : groupOf[uID]) {
		register int msgStk = groupMsg[itr.first].msgN - itr.second;
		if (msgStk == 0) continue;
		msgRcvN += msgStk;
		if (groupMsg[itr.first].msgIdx > msgIdx) {
			msgIdx = groupMsg[itr.first].msgIdx;
			gID = itr.first;
		}
	}

	for (int i = 0; i < 10; i++) {
		mMsg[i] = groupMsg[gID].MSG[i];
		if (groupMsg[gID].MSG[i] == '\0') break;
	}

	return msgRcvN;
}


/*
  [ 그룹 메세지 ]

< 문제설명 및 핵심 로직>
- 여러명의 그룹 만들기
- 그룹 멤버들 (수신동의) 에게 메시지 변하기
- 설정변경 ( 그룹메세지 수신동의 / 반대 )
- 가장 최근의 메세지 확인 or 전체삭제

< 제약조건 >
- 멤버 N 500명
- 그룹 최대 500개

- API max 50000번
- checkMsg는 5천번

< 자료구조>


1. 그냥 그룹별로 메세지는 하나만 저장한다. arr로 저장하고 idx로 참조하는 식으로 하자
2. 각각 그룹별로 멤버들의 메세지설정값을 가지고 있으며
-> chk[ID] = ID놈이 마지막으로 체크한 나의 메세지 idx 저장.


방법 1.
각각 놈들에게는 메세지의 ID를 집어넣고, 메세지 원본은 그룹본체에서 떙겨오기

방법 2.
on/ off 바뀌는 놈만 체크해놓는 방식
- 그룹 본체에 애들에 대한 설정값을 저장해놓는다
- => .setting [ID] = {1/0,  마지막chk ID };

++ 하이브리드로
msgEach [ID] = { 총받은msg수, 마지막받은msg ID };

# 관건 : 가장 최근에 받은 메세지를 반환해야 한다.




1.그룹 메세지 통합관리 (각 그룹별 최신 메세지만 관리)
struct groupMsg [gID] = { on멤버수 ,누적메세지갯수,/ 메세지idx mIdx,  최신메세지};e

2. 소속 그룹은 각자 본인이 저장
u_map <int, str> groupInvolved [pID] ( 소속그룹) = offset (포함될 당시의 최신메세지 갯수)



# 개선필요 부분 ###

- 본인은 본인에게 메세지를 보내지 않기 때문에,
- 가장 최근의 메세지 한개씩만 그룹별로 저장해놓으면 그 최신문자를 보낸 사람의 최신수신을 조회했을때 오류난다.
- 단순히 if (msgStk == 0)  로는 체크가 안됨. msgN의 갭이 0이아닌 상수차이가 나는경우에는!

 ㅁ 이 해결은 어떠냐? 그룹str안에 마지막 전송자의 ID를 저장해놓는거? : 허점있다 그 그룹만 메세지를 주구장창보낸경우



*/