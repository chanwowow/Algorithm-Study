#include <string>
#include <unordered_map>
#include <map>

using namespace std;

struct d_data {
    string h_str;
    string o_DNA;
};

unordered_map <int, d_data> INFO_ID;
unordered_map <string, char> DNA_Hash;
int hDNA_n = 1;
map <string, int> DNA_Map;


void init() {
    //hDNA_n = 1;
    INFO_ID.clear();
    //DNA_Hash.clear();
    DNA_Map.clear();
}

int addSeq(int mID, int mLen, char mSeq[]) {

    // 예외1. 중복된 mID
    if (INFO_ID.find(mID) != INFO_ID.end()) return 0;

    // 3개씩 strig으로 해시
    string h_seq;
    for (int i = 0; i < mLen / 3; i++) {
        string chunk3 = { mSeq[3 * i], mSeq[3 * i + 1],mSeq[3 * i + 2] };
        auto dnaHash = DNA_Hash.find(chunk3);
        if (dnaHash == DNA_Hash.end()) {
            h_seq += hDNA_n;
            DNA_Hash[chunk3] = hDNA_n++;
        }
        else h_seq += dnaHash->second;
    }

    //예외2. 중복된 DNA
    if (DNA_Map.find(h_seq) != DNA_Map.end()) return 0;

    DNA_Map[h_seq] = mID;// 존재한다는 표시!

    string MSEQ;
    for (int i = 0; i < mLen; i++) {
        MSEQ += mSeq[i];
    }
    INFO_ID[mID] = { h_seq, MSEQ }; // 해시DNA, 원본DNA

    // 성공
    return 1;
}

int searchSeq(int mLen, char mBegin[]) {
    // 3개씩 strig으로 해시
    string h_seq1;
    for (int i = 0; i < mLen / 3; i++) {
        string chunk3 = { mBegin[3 * i], mBegin[3 * i + 1],mBegin[3 * i + 2] };
        auto dnaHash = DNA_Hash.find(chunk3);
        if (dnaHash == DNA_Hash.end()) {
            h_seq1 += hDNA_n;
            DNA_Hash[chunk3] = hDNA_n++;
        }
        else h_seq1 += dnaHash->second;
    }
    // 끝자리만 1증가시킨 h_seq2
    int idx = mLen / 3 - 1;
    string h_seq2 = h_seq1;
    h_seq2[idx] = h_seq2[idx] + 1;

    
    //h_seq1으로 시작하는 문자열 갯수 구함
    auto low = DNA_Map.lower_bound(h_seq1);
    if (low == DNA_Map.end()) return -1;
    auto high = DNA_Map.lower_bound(h_seq2);

    int result = distance(low, high);

    if (result > 1) return result;
    if (result == 1) return low->second;
    else return -1;

}

int eraseSeq(int mID) {
    // 예외1. 없는 mID
    auto infoID = INFO_ID.find(mID);
    if (infoID == INFO_ID.end()) return 0;

    DNA_Map.erase(infoID->second.h_str);
    INFO_ID.erase(mID);

    return 1;
}

int changeBase(int mID, int mPos, char mBase) {
    // 예외1. 없는 mID
    auto infoID = INFO_ID.find(mID);
    if (infoID == INFO_ID.end()) return 0;

    // 기존정보 백업
    d_data Orig = infoID->second;

    string MSEQ = Orig.o_DNA;
    // 예외2 없는 위치
    if (mPos > MSEQ.length() - 1) return 0;
    // 예외3 이미바뀜
    if (MSEQ[mPos] == mBase) return 0;
    // 예외4 중복된 염기서열
    MSEQ[mPos] = mBase;
    string h_seq;
    for (int i = 0; i < MSEQ.length() / 3; i++) {
        string chunk3 = { MSEQ[3 * i], MSEQ[3 * i + 1],MSEQ[3 * i + 2] };
        auto dnaHash = DNA_Hash.find(chunk3);
        if (DNA_Hash.find(chunk3) == DNA_Hash.end()) {
            h_seq += hDNA_n;
            DNA_Hash[chunk3] = hDNA_n++;
        }
        else h_seq += dnaHash->second;
    }

    if (DNA_Map.find(h_seq) != DNA_Map.end()) return 0;

    // 가능케이스 되면 정보업뎃
    INFO_ID[mID] = { h_seq, MSEQ };
    DNA_Map[h_seq] = mID;
    DNA_Map.erase(Orig.h_str);

    return 1;
}