#include <unordered_map>
#include <queue>
#include <math.h>

using namespace std;

struct {
    int state; //0 x, 1 queue, 2 parkin
    int t_p_time;
    int t_q_time;
    int p_in_time;
    int q_in_time;
} Car[70010]; // 차량별정보
unordered_map <int, int> C_ID_hash;

struct str1 {
    int car_id;
    int priority;
    int q_in_time;
};
struct wait_priority {
    bool operator()(str1 a, str1 b) {
        if (a.priority == b.priority) return (a.q_in_time > b.q_in_time); // 같은 조건에선 먼저온놈이 우선
        return a.priority > b.priority;
    }
};

priority_queue <str1, vector<str1>, wait_priority> PQ; // 대기차랑 PQ

double B_T, U_T;
int B_F, U_F, Capa;
int h_id, idx; // hash된 차량 idx
int parking_n; // 주차중인 차량 댓수
int q_exept; //대기 중 출차된 댓수 (PQ예외 처리용)

int calc_fee(double time) {
    if (B_T >= time)
        return B_F;
    else
        return (B_F + ceil((time - B_T) / U_T) * U_F);
}

void init(int mBaseTime, int mBaseFee, int mUnitTime, int mUnitFee, int mCapacity) {
    // 초기화 부분
    parking_n = 0;
    q_exept = 0;
    C_ID_hash.clear();
    PQ = {};
    for (int i = 1; i < h_id; i++) {
        Car[i] = {};
    }
    h_id = 1;

    B_T = mBaseTime; B_F = mBaseFee; U_T = mUnitTime; U_F = mUnitFee; Capa = mCapacity;
}

int arrive(int mTime, int mCar) {
    // 처음오는 차량인지 확인
    if (C_ID_hash[mCar] == 0) idx = C_ID_hash[mCar] = h_id++;
    else idx = C_ID_hash[mCar];

    if (Capa > parking_n) { // 주차장 빈자리 있으면 주차
        Car[idx].state = 2;//주차
        Car[idx].p_in_time = mTime;
        parking_n++;
    }
    else {
        Car[idx].state = 1;//대기
        Car[idx].q_in_time = mTime;

        //대기열 추가 부분// id / 계산식-((-입차시간+과거대기누적)-(과거 주차누적)) / q_in_time
        PQ.push({ idx, mTime - Car[idx].t_q_time + Car[idx].t_p_time, mTime });
    }
    return PQ.size() - q_exept; // PQ대기열 - 대기중 제외된 수
}

int leave(int mTime, int mCar) {
    int result;
    idx = C_ID_hash[mCar];
    // [1. 주차중인 차량이면 ]
    if (Car[idx].state == 2) {
        // 1). 요금계산
        result = calc_fee(mTime - Car[idx].p_in_time); // 요금계산
        Car[idx].state = 0;
        Car[idx].t_p_time += mTime - Car[idx].p_in_time;
        parking_n--;

        // 2). 대기열에서 차량 투입
        while (!PQ.empty()) {
            idx = PQ.top().car_id;

            //1. PQ정보가 대기중인 차량이고, 맞는 정보이면/ 주차한다
            if (Car[idx].state == 1 && (Car[idx].q_in_time == PQ.top().q_in_time)) {
                PQ.pop();
                Car[idx].state = 2;
                Car[idx].p_in_time = mTime;
                Car[idx].t_q_time += (mTime - Car[idx].q_in_time);
                parking_n++;
                break;
            }
            else {
                PQ.pop();
                q_exept--;
                continue;
            }

        }
        return result;
    }
    //[2. 대기중인 차량이면]
    else {
        Car[idx].state = 0;
        Car[idx].t_q_time += (mTime - Car[idx].q_in_time); // 대기시간 누적
        q_exept++;
        return -1;
    }

}