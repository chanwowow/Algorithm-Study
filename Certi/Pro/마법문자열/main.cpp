#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>

#define MAX_LEN 10

void init(int N);
int generateString(int mTimestamp, int mLifespan, int mLen, char mStr[]);
int checkString(int mTimestamp, int mLen);

#define CMD_INIT 100
#define CMD_GEN 200
#define CMD_CHECK 300

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
        else if (query == CMD_GEN)
        {
            int mTimestamp;
            int mLifespan, mLen;
            char mStr[MAX_LEN + 1];
            scanf("%d %d %d %s", &mTimestamp, &mLifespan, &mLen, mStr);
            ret = generateString(mTimestamp, mLifespan, mLen, mStr);
            scanf("%d", &ans);
            if (ans != ret)
            {
                std::cout << "      # MY Wrong ANS : " << ret << std::endl << std::endl;
                ok = false;
            }
        }
        else if (query == CMD_CHECK)
        {
            int mTimestamp, mLen;
            int ans;
            scanf("%d %d", &mTimestamp, &mLen);
            ret = checkString(mTimestamp, mLen);
            scanf("%d", &ans);
            if (ans != ret) {
                std::cout << "      # MY Wrong ANS : " << ret << std::endl << std::endl;
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
