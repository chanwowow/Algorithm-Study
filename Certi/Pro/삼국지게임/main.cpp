#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>

#define CMD_INIT 100
#define CMD_DESTROY 200
#define CMD_ALLY 300
#define CMD_ATTACK 400
#define CMD_RECRUIT 500

/////////////////////////////////////////////////////////////////////////
extern void init(int N, int mSoldier[25][25], char mGeneral[25][25][11]);
extern void destroy();
extern int ally(char mMonarchA[11], char mMonarchB[11]);
extern int attack(char mMonarchA[11], char mMonarchB[11], char mGeneral[11]);
extern int recruit(char mMonarch[11], int mNum, int mOption);
/////////////////////////////////////////////////////////////////////////

#define MAX_N 25
#define MAX_L 10

static int Sol[MAX_N][MAX_N];
static char General[MAX_N][MAX_N][MAX_L + 1];

static int run()
{
	int isOK = 0;

	int N;
	int cmd;
	int result;
	int check;

	int mN;
	char mMonarchA[11];
	char mMonarchB[11];
	char mGeneral[11];
	int mOption;
	int num;

	scanf("%d", &N);

	for (int c = 0; c < N; ++c)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d", &mN);
			for (int j = 0; j < mN; j++)
				for (int i = 0; i < mN; i++)
					scanf("%d", &Sol[j][i]);

			for (int j = 0; j < mN; j++)
				for (int i = 0; i < mN; i++)
					scanf("%s", General[j][i]);

			init(mN, Sol, General);
			isOK = 1;
			break;

		case CMD_ALLY:
			scanf("%s %s", mMonarchA, mMonarchB);
			result = ally(mMonarchA, mMonarchB);
			scanf("%d", &check);
			if (result != check) {
				std::cout << "==========>  #Wrong ANS : " << result << std::endl;
				isOK = 0;
			}
				
			break;

		case CMD_ATTACK:
			scanf("%s %s %s", mMonarchA, mMonarchB, mGeneral);
			result = attack(mMonarchA, mMonarchB, mGeneral);
			scanf("%d", &check);
			if (result != check) {
				std::cout << "==========>  #Wrong ANS : " << result << std::endl;
				isOK = 0;
			}
			break;

		case CMD_RECRUIT:
			scanf("%s %d %d", mMonarchA, &num, &mOption);
			result = recruit(mMonarchA, num, mOption);
			scanf("%d", &check);
			if (result != check) {
				std::cout << "==========>  #Wrong ANS : " << result << std::endl;
				isOK = 0;
			}
			break;

		default:
			isOK = 0;
			break;
		}
	}

	destroy();

	return isOK;
}

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		if (run()) printf("#%d %d\n", tc, MARK);
		else printf("#%d %d\n", tc, 0);
	}
	return 0;
}