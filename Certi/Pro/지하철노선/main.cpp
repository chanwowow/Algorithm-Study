#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>

#define CMD_INIT 1
#define CMD_ADD 2
#define CMD_MIN_TRAVEL_TIME 3
#define CMD_MIN_TRANSFER 4

extern void init(int N, int mLastStation1[], int mLastStation2[]);
extern void add(int mLine, int mPrevStation, int mStation);
extern int minTravelTime(int mStartStation, int mEndStation);
extern int minTransfer(int mStartStation, int mEndStation);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

const int numberOfLine = 5;
static int mLastStation1[numberOfLine];
static int mLastStation2[numberOfLine];

static bool run()
{
	int numQuery;

	int N;
	int mLine, mPrevStation, mStation;
	int mStartStation, mEndStation;

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
			for (int j = 0; j < numberOfLine; j++)
				scanf("%d", &mLastStation1[j]);
			for (int j = 0; j < numberOfLine; j++)
				scanf("%d", &mLastStation2[j]);
			init(N, mLastStation1, mLastStation2);
			isCorrect = true;
			break;
		case CMD_ADD:
			scanf("%d %d %d", &mLine, &mPrevStation, &mStation);
			add(mLine, mPrevStation, mStation);
			break;
		case CMD_MIN_TRAVEL_TIME:
			scanf("%d %d", &mStartStation, &mEndStation);
			userAns = minTravelTime(mStartStation, mEndStation);
			scanf("%d", &ans);
			if (userAns != ans)
			{
				std::cout << " # Wrong ANS : " <<userAns<<std::endl;
				isCorrect = false;
			}
			break;
		case CMD_MIN_TRANSFER:
			scanf("%d %d", &mStartStation, &mEndStation);
			userAns = minTransfer(mStartStation, mEndStation);
			scanf("%d", &ans);
			if (userAns != ans)
			{
				std::cout << " # Wrong ANS : " << userAns << std::endl;
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