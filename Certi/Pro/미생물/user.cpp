/*
 1. PQ를 두개로 관리한다
  - 하나는  pq1이고 ID와 nTime : 현재시각 + 반감기
  - 하나는 pq2 이고 ID와 life  : 매번 반감기를 계산한 것. 이때 중복될 수 있으니 takeout 함수 부분에서 중복제거코드 삽입필
*/

#include <unordered_map>
#include <queue>
#include <memory.h>
#include <string>


using namespace std;

int total_eachN[100]; // 각 바이러스 종류 기록할 arr
unordered_map<string, int> mp1;
int halfT_each[100]; // 미생물 종류별 반감기를 담기 위함

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
        if (a.bLife == b.bLife) return (a.ID > b.ID); // 같은생명력일떄 보관시간 먼저인 놈이 우선순위
        return a.bLife > b.bLife;

    }
};


priority_queue<st1, vector<st1>, compTime > PQ_T;
priority_queue<st2, vector<st2>, compLife > PQ_L;

int idx_max;
int idx, ID;
void init(int N, char bNameList[][10], int mHalfTime[]) {
    // 초기화 코드, idx : Bac의 종류 수 / ID : Bac 삽입 아이디
    idx = ID = 0;
    PQ_T = {};
    PQ_L = {};
    memset(total_eachN, 0, sizeof(total_eachN));

    //1. bNameList  => 해싱 => idx로 만듦 => 반감기 table 저장
    string str1;
    idx = 0;
    for (int i = 0; i < N; i++) {

        str1 = bNameList[i];
        mp1[str1] = idx; // 이름 해싱 -> idx로 테이블 만들고
        halfT_each[idx] = mHalfTime[i]; // 반감기 table에는 idx로 저장
        idx++;


        idx_max = idx;
    }

}

void addBacteria(int tStamp, char bName[], int mLife, int mCnt) {
    //변수 ID : 들어온 박테리아의 삽입 ID
    idx = mp1[bName];

    bac_arr[ID] = { idx, tStamp, halfT_each[idx] ,mLife, mCnt, tStamp + halfT_each[idx], false }; // bac_arr[] 에 정보들 저장.
    PQ_T.push({ ID, bac_arr[ID].next_time });
    PQ_L.push({ ID, bac_arr[ID].bLife });
    total_eachN[idx] += mCnt; // 박테리아 종류별 수량 업데이트

    ID++;

}

int last_update_T;
int ID_temp, time_temp, life_temp;

void update(int tStamp) {
    int time_gap = tStamp - last_update_T;

    while (!PQ_T.empty() && PQ_T.top().next_time <= tStamp) {  // PQ_T 에서 현재시각까지 업데이트 해야될놈들만 꺼냄

        ID_temp = PQ_T.top().ID;
        time_temp = PQ_T.top().next_time;
        PQ_T.pop();

        if (bac_arr[ID_temp].deleted == true) { continue; }// 이미 재고 떨어진 놈이면 스킵

        bac_arr[ID_temp].bLife /= 2;  // 반감기 때린다.

        // 수명 다했으면 delete 하고 PQ 푸시는 스킵 // 
        if (bac_arr[ID_temp].bLife < 10) {
            bac_arr[ID_temp].deleted = true;
            total_eachN[bac_arr[ID_temp].idx] -= bac_arr[ID_temp].bCnt;
            continue;
        }

        // 수명 남았다면 PQ_L과 PQ_T 계산해서 푸시//
        PQ_T.push({ ID_temp, time_temp + bac_arr[ID_temp].half_time });
        PQ_L.push({ ID_temp, bac_arr[ID_temp].bLife });


    }

    last_update_T = tStamp;  // 마지막으로 update 실행한 시각 입력
}


int takeOut(int tStamp, int mCnt) {

    int result = 0;
    update(tStamp);

    while (!PQ_L.empty() && mCnt != 0) {
        ID_temp = PQ_L.top().ID;
        life_temp = PQ_L.top().bLife;

        /*
        1. 일단 그놈이 뒤진놈은 아닌지 delete 체크
        2. 중복에 대해서도 필요한가? // 어차피 life가 낮아지는 쪽으로만 바뀌기 때문에 안지워도 되긴함. 그러나 데이터 터짐주의
        */
        if (bac_arr[ID_temp].deleted == true) { PQ_L.pop(); continue; } //1. 이미 지워진 놈은 패스한다.

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
            PQ_L.pop(); // 다썼으므로 소진
        }

    }

    return result;
}

int checkBacteria(int tStamp, char bName[]) {

    update(tStamp);
    return total_eachN[mp1[bName]];
}