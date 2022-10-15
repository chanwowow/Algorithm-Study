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

    // ����1. �ߺ��� mID
    if (INFO_ID.find(mID) != INFO_ID.end()) return 0;

    // 3���� strig���� �ؽ�
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

    //����2. �ߺ��� DNA
    if (DNA_Map.find(h_seq) != DNA_Map.end()) return 0;

    DNA_Map[h_seq] = mID;// �����Ѵٴ� ǥ��!

    string MSEQ;
    for (int i = 0; i < mLen; i++) {
        MSEQ += mSeq[i];
    }
    INFO_ID[mID] = { h_seq, MSEQ }; // �ؽ�DNA, ����DNA

    // ����
    return 1;
}

int searchSeq(int mLen, char mBegin[]) {
    // 3���� strig���� �ؽ�
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
    // ���ڸ��� 1������Ų h_seq2
    int idx = mLen / 3 - 1;
    string h_seq2 = h_seq1;
    h_seq2[idx] = h_seq2[idx] + 1;

    
    //h_seq1���� �����ϴ� ���ڿ� ���� ����
    auto low = DNA_Map.lower_bound(h_seq1);
    if (low == DNA_Map.end()) return -1;
    auto high = DNA_Map.lower_bound(h_seq2);

    int result = distance(low, high);

    if (result > 1) return result;
    if (result == 1) return low->second;
    else return -1;

}

int eraseSeq(int mID) {
    // ����1. ���� mID
    auto infoID = INFO_ID.find(mID);
    if (infoID == INFO_ID.end()) return 0;

    DNA_Map.erase(infoID->second.h_str);
    INFO_ID.erase(mID);

    return 1;
}

int changeBase(int mID, int mPos, char mBase) {
    // ����1. ���� mID
    auto infoID = INFO_ID.find(mID);
    if (infoID == INFO_ID.end()) return 0;

    // �������� ���
    d_data Orig = infoID->second;

    string MSEQ = Orig.o_DNA;
    // ����2 ���� ��ġ
    if (mPos > MSEQ.length() - 1) return 0;
    // ����3 �̹̹ٲ�
    if (MSEQ[mPos] == mBase) return 0;
    // ����4 �ߺ��� ���⼭��
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

    // �������̽� �Ǹ� ��������
    INFO_ID[mID] = { h_seq, MSEQ };
    DNA_Map[h_seq] = mID;
    DNA_Map.erase(Orig.h_str);

    return 1;
}