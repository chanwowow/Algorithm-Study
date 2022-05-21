
#include<iostream>

using namespace std;

int* cardArr[120000]; // 10만은 넘게 제작
int idx_min, idx_max;

int numDB[31];
int Joker;

void init(int mJoker, int mNumbers[5]){
    idx_min = 55000; idx_max = 55005;
    for (int i = 1; i <= 30; i++) numDB[i] = i;
    Joker = mJoker;

    for (int i = 0; i < 5; i++) {
        if (mNumbers[i] == -1) cardArr[idx_min + i] = &Joker;
        else cardArr[idx_min + i] = &numDB[mNumbers[i]];
    }
}

void putCards(int mDir, int mNumbers[5]){
    if (mDir == 0) {
        idx_min -= 5;
        for (int i = 0; i < 5; i++) {
            if (mNumbers[i] == -1) cardArr[idx_min + i] = &Joker;
            else cardArr[idx_min + i] = &numDB[mNumbers[i]];
        }
    }
    else {
        idx_max += 5;
        for (int i = 0; i < 5; i++) {
            if (mNumbers[i] == -1) cardArr[idx_max - 5 + i] = &Joker;
            else cardArr[idx_max - 5 + i] = &numDB[mNumbers[i]];
        }
    }
}

int findNumber(int mNum, int mNth, int ret[4]){
    bool Can = false;
    for (int idx = idx_min; idx <= idx_max - 4; idx++) {
        if ((*cardArr[idx] + *cardArr[idx + 1] + *cardArr[idx + 2] + *cardArr[idx + 3]) % 20 == mNum) {
            mNth--;
        }
        if (mNth == 0) {
            for (int i = 0; i < 4; i++) {
                if (cardArr[idx + i] == &Joker) ret[i] = -1;
                else ret[i] = *cardArr[idx + i];
            }
            Can = true;
            break;
        }
    }
    
    if (Can) return 1;
    else return 0;
}

void changeJoker(int mValue){
    Joker = mValue;
}