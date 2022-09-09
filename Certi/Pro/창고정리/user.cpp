#include <list>
#include <unordered_map>
#include <string>
#include <cstring>

using namespace std;

struct pdt {
    bool isRemoved;
    int sR, sC;
    int eR, eC;
    int size;
    string name, tag;
} pdtDB[301];

unordered_map <string, int> pdtHash; int nuPdtID;
unordered_map <string, int> kindHash; int nuKindID;
list <int> kindList[301];

int maxR, maxC;
void init(int R, int C) {
    nuPdtID = nuKindID = 1;
    pdtHash.clear();
    kindHash.clear();
    maxR = R;
    maxC = C;
}

struct { int sR, sC, eR, eC; } glb;
bool autoPlacing(int height, int width);

int addItem(char name[], char tag[], int height, int width, int mode, int r, int c) {
    // 배치 가능성 판단
    int sR, sC, eR, eC;
    bool can = false;
    if (mode == 0) {
        sR = r; sC = c;
        eR = r + height-1; eC = c + width-1;

        if (eR <= maxR && eC <= maxC) {
            can = true;
            for (auto& itr : pdtHash) {
                pdt chk = pdtDB[itr.second];
                if (chk.isRemoved == true) continue;
                if (sR > chk.eR || sC > chk.eC || eR < chk.sR || eC < chk.sC) continue;
                else {
                    can = false;  break; // 불가능하면 중단
                }
            }
        }
    }
    else { // 자동배치
        if (autoPlacing(height, width) == true) {
            can = true;
            sR = glb.sR; sC = glb.sC;
            eR = glb.eR;  eC = glb.eC;
        }
    }

    // 배치가 된다면
    if (can) {
        int pdtID = pdtHash[name] = nuPdtID++;
        int kindID = kindHash[tag];
        if (kindID == 0) {
            kindID = kindHash[tag] = nuKindID++;
            kindList[kindID].clear();
        }
        pdtDB[pdtID] = {};
        pdtDB[pdtID] = { false, sR,sC, eR, eC, height * width, name, tag };
        kindList[kindID].push_back(pdtID);
    }

    return can;
}

int removeItemByName(char name[]) {
    bool can = false;
    auto itr = pdtHash.find(name);
    if (itr!= pdtHash.end()) {
        if (pdtDB[itr->second].isRemoved == false) {
            can = true;
            pdtDB[itr->second].isRemoved = true;
        }
        pdtHash.erase(itr);
        return can;
    }
    return can;
}

int removeItemByTag(char tag[]) {
    int num = 0;
    auto itr = kindHash.find(tag);
    if (itr != kindHash.end()) {
        int kindID = itr->second;
        for (auto pdtID : kindList[kindID]) {
            if (pdtDB[pdtID].isRemoved == true) continue;
            pdtDB[pdtID].isRemoved = true;
            ++num;
        }
        kindList[kindID].clear();
    }
    return num;
}

int getItem(int r, int c, char name[], char tag[]) {
    bool can = false;
    for (auto &itr : pdtHash) {
        pdt chk = pdtDB[itr.second];
        if (chk.isRemoved) continue;
        if (r >= chk.sR && r <= chk.eR && c >= chk.sC && c <= chk.eC) {
            strcpy(name, chk.name.c_str());
            strcpy(tag, chk.tag.c_str());
            can = true;
            break;
        }
    }
    return can;
}

int getArea(char tag[]) {
    int ret = 0;
    auto itr = kindHash.find(tag);
    if (itr != kindHash.end()) {
        int kindID = itr->second;
        for (auto pdtID : kindList[kindID]) {
            if (pdtDB[pdtID].isRemoved == true) continue;
            ret += pdtDB[pdtID].size;
        }
    }
    return ret;
}

bool chkHere(int sR, int sC, int eR, int eC){
    bool can =true;
    if (eR <= maxR && eC <= maxC) {
        for (auto& itr : pdtHash) {
            pdt chk = pdtDB[itr.second];
            if (chk.isRemoved == true) continue;
            if (sR > chk.eR || sC > chk.eC || eR < chk.sR || eC < chk.sC) continue;
            can = false;
            break;
        }
    }return can;
}

bool autoPlacing(int height, int width) {
    bool can = true;
    int sR = 1; int sC = 1;
    int h = height - 1; int w = width - 1;
    int max_R = maxR; int max_C = maxC;
    int prevC = max_C + 1;

    while (1) {
        int eR = sR + h; int eC = sC + w;
        if (eR > max_R) {
            sR = 1;
            sC = prevC;
            prevC = max_C + 1;
            continue;
        }
        if (eC > max_C) break;
        can = true;
        for (auto& itr : pdtHash) {
            pdt chk = pdtDB[itr.second];
            if (chk.isRemoved) continue;
            if (sR > chk.eR || sC > chk.eC || eR < chk.sR || eC < chk.sC) continue;
            // 걸리는놈 있으면 그거 다음부터 시작위함
            sR = chk.eR + 1;
            prevC = min(prevC, chk.eC + 1);
            can = false;
            break;
        }
        if (can) {
            glb = { sR, sC, eR, eC };
            break;
        }
    }

    return can;
}