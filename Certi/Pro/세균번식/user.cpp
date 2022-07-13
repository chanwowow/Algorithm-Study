#define MAX_N 101
#define MAX_CELL 10005

#include <queue>  
#include <list>
#include <memory.h>

using namespace std;

struct dishCell {
    int cost;
    int state;
    bool blue;
    int motherID;
}dish[MAX_N][MAX_N];

// union find��
int conn[MAX_CELL];
int newCellID;

// PQ  => �����ʿ� : �Ź� PQ�� �ް�ٴϴ��� LIST�ް�ٴϴٰ� �ʿ�� PQ�ϴ°� ����
struct str1 {
    int cost, r, c;
    int cellID;
};
struct cmp {
    bool operator()(str1 a, str1 b) {
        if (a.cost == b.cost) {
            if (a.r == b.r) {
                return a.c > b.c;
            }return a.r > b.r;
        }return a.cost < b.cost;
    }
};
priority_queue <str1, vector<str1>, cmp> PQ;
list <str1> nearby[MAX_CELL];

// ��ü ���� ������ DB
struct { int r; int c; } cellIDtoPos[MAX_CELL]; // cell��ǥ
int numBlue, numRed; // Ȱ�� R/B ���̷��� ��
int numUnion[MAX_CELL]; // motherID ������ union �� �� �˼��ִ�

void init(int N, int mDish[MAX_N][MAX_N]){
    // �ʱ�ȭ�ڵ� (!!)
    newCellID = 1;

    //�ʱⵥ����
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            dish[r][c] = { mDish[r - 1][c - 1] }; // �긻�� �� �ʱ�ȭ
        }
    }
}

int dropMedicine(int mTarget, int mRow, int mCol, int mEnergy){
    bool blue = mTarget == 1 ? true: false;

    if (dish[mRow][mCol].state == 0 || conn[dish[mRow][mCol].motherID]==-1 ) {
        // 1.cell����ִ� ���
    }
    else if (dish[mRow][mCol].blue == blue) {
        //2. ���������� ���
    }
    else {
        //3. �ٸ�����
    }

 

    // Ȱ��ȭ�� ������ ���� Ȱ��|��Ȱ��|Ȱ��|��Ȱ�� ������ �����ϴٴ� ��
    /*
    ���� ������ union�� �����ؾ� �Ѵٴ� ���� �����ؾ��Ѵ�
    =>�̶� ��Ȱ-> Ȱ�� �߽߰� ����Ȯ���� ���� list splice �ʿ�
    ���� �̶� numUnion�� �� �����ִ� �͵� ������

    -activation(r, c ..) �Լ� ���� �� ��ͻ��
    */
    return blue ? numBlue : numRed;
}

int cleanBacteria(int mRow, int mCol){
    return -1;
}

void activation(int r, int c, int energyLeft)