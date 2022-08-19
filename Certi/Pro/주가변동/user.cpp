#define MAX_STOCK_LEN 10
#define MAX_N 100000

#include <string>
#include <memory.h>
#include <map>
#include <unordered_map>

using namespace std;

map <string, pair<int, int>> stk_name;
unordered_map <string, int> stk_list;

int org_prc[MAX_N]; // ���ʰ�
int del_prc[MAX_N]; // ������
int idx2_ID[MAX_N];// ID ������


void init(int N, char mStocks[][MAX_STOCK_LEN + 1], int mPrices[])
{
    stk_name.clear();
    stk_list.clear();
    memset(del_prc, 0, sizeof(del_prc)); // arr�� �길 �ص� �ɵ�

    for (int i = 0; i < N; i++) {
        stk_name.insert({ mStocks[i] ,{i, mPrices[i]} }); // 1. ���� (�̸�, (ID, ���ʰ�))
    }

    int idx = 0;
    for (auto itr = stk_name.begin(); itr != stk_name.end(); itr++) {
        stk_list.insert({ itr->first, idx }); // 2. �ؽøʿ� �̸��� idx ����. 
        idx2_ID[idx] = itr->second.first; //ID �� ���ʰ� �Ű� ����
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