#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>
#include <string.h>

#define ADD_ITEM                100
#define REMOVE_ITEM_BY_NAME     200
#define REMOVE_ITEM_BY_TAG      300
#define GET_ITEM                400
#define GET_AREA                500

extern void init(int R, int C);
extern int addItem(char name[], char tag[], int height, int width,
    int mode, int r, int c);
extern int removeItemByName(char name[]);
extern int removeItemByTag(char tag[]);
extern int getItem(int r, int c, char name[], char tag[]);
extern int getArea(char tag[]);
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int run(int Result) {
    int N, R, C;

    scanf("%d %d %d", &N, &R, &C);

    init(R, C);

    for (int i = 0; i < N; ++i) {
        int cmd = 0;
        char name[11];
        char tag[11];
        char userName[11];
        char userTag[11];
        int height, width, mode, r, c;
        int ans, res;

        scanf("%d", &cmd);

        switch (cmd) {
        case ADD_ITEM:
            scanf("%s %s", name, tag);
            scanf("%d %d %d %d %d", &height, &width, &mode, &r, &c);

            res = addItem(name, tag, height, width, mode, r, c);

            scanf("%d", &ans);
            if (res != ans) { Result = 0; 
            std::cout << "===========> WRONG ANS :" << res << std::endl;
            }
            break;

        case REMOVE_ITEM_BY_NAME:
            scanf("%s", name);

            res = removeItemByName(name);

            scanf("%d", &ans);
            if (res != ans) {
                Result = 0;
                std::cout << "===========> WRONG ANS :" << res << std::endl;
            }
            break;

        case REMOVE_ITEM_BY_TAG:
            scanf("%s", tag);

            res = removeItemByTag(tag);

            scanf("%d", &ans);
            if (res != ans) {
                Result = 0;
                std::cout << "===========> WRONG ANS :" << res << std::endl;
            }
            break;

        case GET_ITEM:
            scanf("%d %d", &r, &c);

            res = getItem(r, c, userName, userTag);

            scanf("%d", &ans);
            if (res != ans) Result = 0;
            if (ans == 1) {
                scanf("%s %s", name, tag);
                if (strcmp(name, userName) != 0) { Result = 0; std::cout << " WRONG name!! : " << userName << std::endl; }
                if (strcmp(tag, userTag) != 0) { Result = 0; std::cout << " WRONG tag!! : " << userTag<< std::endl; }
            }
            break;

        case GET_AREA:
            scanf("%s", tag);

            res = getArea(tag);

            scanf("%d", &ans);
            if (res != ans) {
                Result = 0;
                std::cout << "===========> WRONG ANS :" << res << std::endl;
            }
            break;

        default:
            Result = 0;
            break;
        }
    }

    return Result;
}


int main() {
    setbuf(stdout, NULL);

    //freopen("sample_input.txt", "r", stdin);

    int T, Result;
    scanf("%d %d", &T, &Result);

    for (int tc = 1; tc <= T; tc++) {
        printf("#%d %d\n", tc, run(Result));
    }

    return 0;
}
