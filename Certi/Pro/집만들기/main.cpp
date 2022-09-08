#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>

extern void init(int N);
extern int put(int mRow, int mCol);

#define CMD_INIT 100
#define CMD_PUT 200

static bool run()
{
    int query_num;
    scanf("%d", &query_num);

    int ret, ans;
    bool ok = false;

    for (int q = 0; q < query_num; q++)
    {
        int query;
        scanf("%d", &query);

        if (query == CMD_INIT)
        {
            int N;
            scanf("%d", &N);
            init(N);
            ok = true;
        }
        else if (query == CMD_PUT)
        {
            int mRow, mCol;
            scanf("%d %d", &mRow, &mCol);
            ret = put(mRow, mCol);
            scanf("%d", &ans);
            if (ans != ret)
            {
                std::cout << "==========> WRONG ANS :" << ret << " corrt : " << ans << std::endl;
                ok = false;
            }
        }
    }
    return ok;
}
int main()
{
    setbuf(stdout, NULL);
    // freopen("sample_input.txt", "r", stdin);
    int T, MARK;
    scanf("%d %d", &T, &MARK);

    for (int tc = 1; tc <= T; tc++)
    {
        int score = run() ? MARK : 0;
        printf("#%d %d\n", tc, score);
    }
    return 0;
}
