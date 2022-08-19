#define MAX_STOCK_LEN 10
#define MAX_N 100000

#include <string>
#include <memory.h>
#include <map>
#include <unordered_map>

using namespace std;

map <string, pair<int, int>> stk_name;
unordered_map <string, int> stk_list;

int org_prc[MAX_N]; // 시초가
int del_prc[MAX_N]; // 변동량
int idx2_ID[MAX_N];// ID 역추적


void init(int N, char mStocks[][MAX_STOCK_LEN + 1], int mPrices[])
{
    stk_name.clear();
    stk_list.clear();
    memset(del_prc, 0, sizeof(del_prc)); // arr는 얘만 해도 될듯

    for (int i = 0; i < N; i++) {
        stk_name.insert({ mStocks[i] ,{i, mPrices[i]} }); // 1. 정렬 (이름, (ID, 시초가))
    }

    int idx = 0;
    for (auto itr = stk_name.begin(); itr != stk_name.end(); itr++) {
        stk_list.insert({ itr->first, idx }); // 2. 해시맵에 이름과 idx 매핑. 
        idx2_ID[idx] = itr->second.first; //ID 및 시초가 옮겨 저장
        org_prc[idx] = itr->second.second;
        idx++;
    }

}

int idx1, idx2;
void changePrices(char mFromStock[], char mToStock[], int mPriceDiff)
{
    idx1 = stk_list[mFromStock];
    idx2 = stk_list[mToStock];

    for (int i = idx1; i <= idx2; i++) {
        del_prc[i] += mPriceDiff;
    }

}

int getPrice(char mStock[])
{
    idx1 = stk_list[mStock];

    return org_prc[idx1] + del_prc[idx1];
}

int prc_max, ID_min;
int getMostIncreasedStock(char mFromStock[], char mToStock[])
{
    idx1 = stk_list[mFromStock];
    idx2 = stk_list[mToStock];

    ID_min = idx2_ID[idx1];
    prc_max = del_prc[idx1];

    for (int i = idx1; i <= idx2; i++) {
        if (del_prc[i] > prc_max) {
            prc_max = del_prc[i];
            ID_min = idx2_ID[i];

        }
        else if (del_prc[i] == prc_max) {
            if (ID_min > idx2_ID[i]) ID_min = idx2_ID[i];
        }
    }

    return ID_min;
}