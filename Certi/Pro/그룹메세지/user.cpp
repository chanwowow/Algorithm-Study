
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

unordered_map <int, int> groupOf[maxN]; // groupOf[pID] (gID) = offset (���ԵǱ����� �׷� �ֽŸ޼��� ����)
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
	groupOf[uID][groupID]++; // ������ �ȹ����ϱ�

	groupMsg[groupID].msgN++;
	groupMsg[groupID].msgIdx = gMsgIdx++; // �����ε��� ����
	for (register int i = 0; i < 10; i++) {
		groupMsg[groupID].MSG[i] = mText[i];
		if (mText[i] == '\0') break;
	}

	return groupMsg[groupID].memberN - 1;
}

void setOnOff(int uID, int groupID){

	auto itr = groupOf[uID].find(groupID);
	// 1. ���� off ���ٸ� on
	if (itr == groupOf[uID].end()) {
		groupOf[uID][groupID] = groupMsg[groupID].msgN;
		groupMsg[groupID].memberN++;
	}
	// 2. ���� On ���ٸ� off
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
  [ �׷� �޼��� ]

< �������� �� �ٽ� ����>
- �������� �׷� �����
- �׷� ����� (���ŵ���) ���� �޽��� ���ϱ�
- �������� ( �׷�޼��� ���ŵ��� / �ݴ� )
- ���� �ֱ��� �޼��� Ȯ�� or ��ü����

< �������� >
- ��� N 500��
- �׷� �ִ� 500��

- API max 50000��
- checkMsg�� 5õ��

< �ڷᱸ��>


1. �׳� �׷캰�� �޼����� �ϳ��� �����Ѵ�. arr�� �����ϰ� idx�� �����ϴ� ������ ����
2. ���� �׷캰�� ������� �޼����������� ������ ������
-> chk[ID] = ID���� ���������� üũ�� ���� �޼��� idx ����.


��� 1.
���� ��鿡�Դ� �޼����� ID�� ����ְ�, �޼��� ������ �׷캻ü���� ���ܿ���

��� 2.
on/ off �ٲ�� �� üũ�س��� ���
- �׷� ��ü�� �ֵ鿡 ���� �������� �����س��´�
- => .setting [ID] = {1/0,  ������chk ID };

++ ���̺긮���
msgEach [ID] = { �ѹ���msg��, ����������msg ID };

# ���� : ���� �ֱٿ� ���� �޼����� ��ȯ�ؾ� �Ѵ�.




1.�׷� �޼��� ���հ��� (�� �׷캰 �ֽ� �޼����� ����)
struct groupMsg [gID] = { on����� ,�����޼�������,/ �޼���idx mIdx,  �ֽŸ޼���};e

2. �Ҽ� �׷��� ���� ������ ����
u_map <int, str> groupInvolved [pID] ( �Ҽӱ׷�) = offset (���Ե� ����� �ֽŸ޼��� ����)



# �����ʿ� �κ� ###

- ������ ���ο��� �޼����� ������ �ʱ� ������,
- ���� �ֱ��� �޼��� �Ѱ����� �׷캰�� �����س����� �� �ֽŹ��ڸ� ���� ����� �ֽż����� ��ȸ������ ��������.
- �ܼ��� if (msgStk == 0)  �δ� üũ�� �ȵ�. msgN�� ���� 0�̾ƴ� ������̰� ���°�쿡��!

 �� �� �ذ��� ���? �׷�str�ȿ� ������ �������� ID�� �����س��°�? : �����ִ� �� �׷츸 �޼����� �ֱ���â�������



*/