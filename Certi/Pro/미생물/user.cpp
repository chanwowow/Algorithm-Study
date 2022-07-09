/*
 1. PQ�� �ΰ��� �����Ѵ�
  - �ϳ���  pq1�̰� ID�� nTime : ����ð� + �ݰ���
  - �ϳ��� pq2 �̰� ID�� life  : �Ź� �ݰ��⸦ ����� ��. �̶� �ߺ��� �� ������ takeout �Լ� �κп��� �ߺ������ڵ� ������
*/

#include <unordered_map>
#include <queue>
#include <memory.h>
#include <string>


using namespace std;

int total_eachN[100]; // �� ���̷��� ���� ����� arr
unordered_map<string, int> mp1;
int halfT_each[100]; // �̻��� ������ �ݰ��⸦ ��� ����

struct backteria_info {
    int idx;
    int in_time;
    int half_time;

    int bLife;
    int bCnt;
    int next_time;
    bool deleted;

} bac_arr[15000], mem;

struct st1 {
    int ID, next_time;
};
struct compTime {
    bool operator()(st1 a, st1 b) {
        //if (a.next_time == b.next_time) return (a.ID > b.ID);
        return a.next_time > b.next_time;

    }
};

struct st2 {
    int ID, bLife;
};
struct compLife {
    bool operator()(st2 a, st2 b) {
        if (a.bLife == b.bLife) return (a.ID > b.ID); // ����������ϋ� �����ð� ������ ���� �켱����
        return a.bLife > b.bLife;

    }
};


priority_queue<st1, vector<st1>, compTime > PQ_T;
priority_queue<st2, vector<st2>, compLife > PQ_L;

int idx_max;
int idx, ID;
void init(int N, char bNameList[][10], int mHalfTime[]) {
    // �ʱ�ȭ �ڵ�, idx : Bac�� ���� �� / ID : Bac ���� ���̵�
    idx = ID = 0;
    PQ_T = {};
    PQ_L = {};
    memset(total_eachN, 0, sizeof(total_eachN));

    //1. bNameList  => �ؽ� => idx�� ���� => �ݰ��� table ����
    string str1;
    idx = 0;
    for (int i = 0; i < N; i++) {

        str1 = bNameList[i];
        mp1[str1] = idx; // �̸� �ؽ� -> idx�� ���̺� �����
        halfT_each[idx] = mHalfTime[i]; // �ݰ��� table���� idx�� ����
        idx++;


        idx_max = idx;
    }

}

void addBacteria(int tStamp, char bName[], int mLife, int mCnt) {
    //���� ID : ���� ���׸����� ���� ID
    idx = mp1[bName];

    bac_arr[ID] = { idx, tStamp, halfT_each[idx] ,mLife, mCnt, tStamp + halfT_each[idx], false }; // bac_arr[] �� ������ ����.
    PQ_T.push({ ID, bac_arr[ID].next_time });
    PQ_L.push({ ID, bac_arr[ID].bLife });
    total_eachN[idx] += mCnt; // ���׸��� ������ ���� ������Ʈ

    ID++;

}

int last_update_T;
int ID_temp, time_temp, life_temp;

void update(int tStamp) {
    int time_gap = tStamp - last_update_T;

    while (!PQ_T.empty() && PQ_T.top().next_time <= tStamp) {  // PQ_T ���� ����ð����� ������Ʈ �ؾߵɳ�鸸 ����

        ID_temp = PQ_T.top().ID;
        time_temp = PQ_T.top().next_time;
        PQ_T.pop();

        if (bac_arr[ID_temp].deleted == true) { continue; }// �̹� ��� ������ ���̸� ��ŵ

        bac_arr[ID_temp].bLife /= 2;  // �ݰ��� ������.

        // ���� �������� delete �ϰ� PQ Ǫ�ô� ��ŵ // 
        if (bac_arr[ID_temp].bLife < 10) {
            bac_arr[ID_temp].deleted = true;
            total_eachN[bac_arr[ID_temp].idx] -= bac_arr[ID_temp].bCnt;
            continue;
        }

        // ���� ���Ҵٸ� PQ_L�� PQ_T ����ؼ� Ǫ��//
        PQ_T.push({ ID_temp, time_temp + bac_arr[ID_temp].half_time });
        PQ_L.push({ ID_temp, bac_arr[ID_temp].bLife });


    }

    last_update_T = tStamp;  // ���������� update ������ �ð� �Է�
}


int takeOut(int tStamp, int mCnt) {

    int result = 0;
    update(tStamp);

    while (!PQ_L.empty() && mCnt != 0) {
        ID_temp = PQ_L.top().ID;
        life_temp = PQ_L.top().bLife;

        /*
        1. �ϴ� �׳��� �������� �ƴ��� delete üũ
        2. �ߺ��� ���ؼ��� �ʿ��Ѱ�? // ������ life�� �������� �����θ� �ٲ�� ������ �������� �Ǳ���. �׷��� ������ ��������
        */
        if (bac_arr[ID_temp].deleted == true) { PQ_L.pop(); continue; } //1. �̹� ������ ���� �н��Ѵ�.

        if (bac_arr[ID_temp].bCnt > mCnt) {
            bac_arr[ID_temp].bCnt -= mCnt;
            total_eachN[bac_arr[ID_temp].idx] -= mCnt;
            result += mCnt * life_temp;
            mCnt = 0;
        }
        else {
            mCnt -= bac_arr[ID_temp].bCnt;
            total_eachN[bac_arr[ID_temp].idx] -= bac_arr[ID_temp].bCnt;
            bac_arr[ID_temp].deleted = true;
            result += bac_arr[ID_temp].bCnt * life_temp;
            PQ_L.pop(); // �ٽ����Ƿ� ����
        }

    }

    return result;
}

int checkBacteria(int tStamp, char bName[]) {

    update(tStamp);
    return total_eachN[mp1[bName]];
}