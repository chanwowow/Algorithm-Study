#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>

#define CMD_INIT 1
#define CMD_ADD_RESTAURANT 2
#define CMD_ADD_VALUE 3
#define CMD_BEST_VALUE 4
#define CMD_REGIONAL_VALUE 5

extern void init(int N, int M, int mRoads[][2]);
extern void addRestaurant(int mCityID, char mName[]);
extern void addValue(char mName[], int mScore);
extern int bestValue(char mStr[]);
extern int regionalValue(int mCityID, int mDist);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int mRoads[50][2];
static char mName[6];
static char mStr[6];

static bool run()
{
	int numQuery;

	int N, M, mCityID, mScore, mDist;

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
			scanf("%d %d", &N, &M);
			for (int j = 0; j < M; j++)
				scanf("%d %d", &mRoads[j][0], &mRoads[j][1]);
			init(N, M, mRoads);
			isCorrect = true;
			break;
		case CMD_ADD_RESTAURANT:
			scanf("%d", &mCityID);
			scanf("%s", mName);
			addRestaurant(mCityID, mName);
			break;
		case CMD_ADD_VALUE:
			scanf("%s", mName);
			scanf("%d", &mScore);
			addValue(mName, mScore);
			break;
		case CMD_BEST_VALUE:
			scanf("%s", mStr);
			userAns = bestValue(mStr);
			scanf("%d", &ans);
			if (userAns != ans)
			{
				isCorrect = false;
				std::cout << " ==========> WRONG ANS :" << userAns << std::endl;
			}
			break;
		case CMD_REGIONAL_VALUE:
			scanf("%d", &mCityID);
			scanf("%d", &mDist);
			userAns = regionalValue(mCityID, mDist);
			scanf("%d", &ans);
			if (userAns != ans)
			{
				isCorrect = false;
				std::cout << " ==========> WRONG ANS :" << userAns << std::endl;
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
	//freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		int score = run() ? MARK : 0;
		printf("#%d %d\n", tc, score);
	}

	return 0;
}