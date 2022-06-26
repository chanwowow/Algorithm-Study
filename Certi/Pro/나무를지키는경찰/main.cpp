#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>

#define CMD_INIT 1
#define CMD_OCCUR 2
#define CMD_CANCEL 3
#define CMD_POSITION 4

extern void init(int N, int mParent[]);
extern void occur(int mTimeStamp, int mID, int mTown, int mPriority);
extern void cancel(int mTimeStamp, int mID);
extern int position(int mTimeStamp);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int mParent[350];

static bool run()
{
	int numQuery;

	int N, mTimeStamp, mID, mTown, mPriority;

	int userAns, ans;

	bool isCorrect = false;

	scanf("%d", &numQuery);

	for (int i = 0; i < numQuery; ++i)
	{
		int cmd;
		scanf("%d", &cmd);

		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &N);
			for (int j = 0; j < N; j++)
				scanf("%d", &mParent[j]);
			init(N, mParent);
			isCorrect = true;
			break;
		case CMD_OCCUR:
			scanf("%d %d %d %d", &mTimeStamp, &mID, &mTown, &mPriority);
			occur(mTimeStamp, mID, mTown, mPriority);
			break;
		case CMD_CANCEL:
			scanf("%d %d", &mTimeStamp, &mID);
			cancel(mTimeStamp, mID);
			break;
		case CMD_POSITION:
			scanf("%d", &mTimeStamp);
			userAns = position(mTimeStamp);
			scanf("%d", &ans);
			if (userAns != ans)
			{
				std::cout << " ==========> Wrong ANS : " << userAns << std::endl;
				isCorrect = false;
			}
			break;
		default:
			isCorrect = false;
			break;
		}
	}
	return isCorrect;
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