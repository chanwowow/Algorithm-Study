#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<string.h>
#include <iostream>

#define MAX_N 10
#define MAX_LEN 11

#define CMD_INIT 100
#define CMD_JOIN 200
#define CMD_SELECT_WORD 300
#define CMD_FIND_WINNERS 400

extern void init(int N, int M);
extern void join(int mID, char mBingo[MAX_N][MAX_N][MAX_LEN]);
extern void selectWord(int mRound, char mWord[]);
extern int findWinners();

static int N = 0, M = 0;
static char mWord[MAX_LEN];

static bool run()
{
	int query_cnt;
	scanf("%d", &query_cnt);
	bool ok = true;

	for (int q = 0; q < query_cnt; q++)
	{
		int cmd;
		scanf("%d", &cmd);

		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d%d", &N, &M);
			init(N, M);
			break;
		case CMD_JOIN:
			int mID;
			char mBingo[MAX_N][MAX_N][MAX_LEN];
			scanf("%d", &mID);
			for (int y = 0; y < N; y++)
			{
				for (int x = 0; x < N; x++)
				{
					scanf("%s", mBingo[y][x]);
				}
			}
			join(mID, mBingo);
			break;
		case CMD_SELECT_WORD:
			int mRound;
			char ans_word[MAX_LEN];
			scanf("%d", &mRound);
			selectWord(mRound, mWord);
			scanf("%s", ans_word);
			if (strcmp(mWord, ans_word) != 0)
			{
				std::cout << "========== > WORNG ANS : " << mWord << std::endl;
				ok = false;
			}
			break;
		case CMD_FIND_WINNERS:
			int ret_total, ans_total;
			ret_total = findWinners();
			scanf("%d", &ans_total);
			if (ret_total != ans_total)
			{
				std::cout << "========== > WORNG ANS : " << ret_total << std::endl;
				ok = false;
			}
			break;
		}
	}
	return ok;
}

int main()
{
	setbuf(stdout, NULL);
	// freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d%d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}