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

// union find용
int conn[MAX_CELL];
int newCellID;

// PQ  => 수정필요 : 매번 PQ를 달고다니느니 LIST달고다니다가 필요시 PQ하는게 낫다
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

// 전체 공용 데이터 DB
struct { int r; int c; } cellIDtoPos[MAX_CELL]; // cell좌표
int numBlue, numRed; // 활성 R/B 바이러스 수
int numUnion[MAX_CELL]; // motherID 넣으면 union 총 수 알수있는

void init(int N, int mDish[MAX_N][MAX_N]){
    // 초기화코드 (!!)
    newCellID = 1;

    //초기데이터
    for (int r = 1; r <= N; r++) {
        for (int c = 1; c <= N; c++) {
            dish[r][c] = { mDish[r - 1][c - 1] }; // 얘말곤 다 초기화
        }
    }
}

int dropMedicine(int mTarget, int mRow, int mCol, int mEnergy){
    bool blue = mTarget == 1 ? true: false;

    if (dish[mRow][mCol].state == 0 || conn[dish[mRow][mCol].motherID]==-1 ) {
        // 1.cell비어있는 경우
    }
    else if (dish[mRow][mCol].blue == blue) {
        //2. 같은종류인 경우
    }
    else {
        //3. 다른종류
    }

 

    // 활성화시 주의할 점은 활성|비활성|활성|비활성 구조가 가능하다는 점
    /*
    따라서 끝까지 union을 진행해야 한다는 점을 참고해야한다
    =>이때 비활-> 활성 발견시 인접확장을 위해 list splice 필요
    또한 이때 numUnion을 잘 합쳐주는 것도 잊지마

    -activation(r, c ..) 함수 제작 후 재귀사용
    */
    return blue ? numBlue : numRed;
}

int cleanBacteria(int mRow, int mCol){
    return -1;
}

void activation(int r, int c, int energyLeft)