#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CMD_INIT 100
#define CMD_DESTROY 200
#define CMD_PUT 300

#define MAX_N 1000

extern void init(int N, int M, int mU[][4], int mR[][4], int mB[][4], int mL[][4]);
extern void destroy();
extern int put(int mPiece[4]);

static int PieceU[MAX_N + 2][4];
static int PieceR[MAX_N + 2][4];
static int PieceD[MAX_N + 2][4];
static int PieceL[MAX_N + 2][4];


static int run()
{
	int isOK = 0;
	int N;
	int cmd, result = 0, check;

	int mN, mM;
	int mPiece[4];

	scanf("%d", &N);

	for (int c = 0; c < N; ++c)
	{
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT:
			scanf("%d %d", &mN, &mM);
			for (int i = 0; i < mN; i++) scanf("%d %d %d %d", &PieceU[i][0], &PieceU[i][1], &PieceU[i][2], &PieceU[i][3]);
			for (int i = 0; i < mN; i++) scanf("%d %d %d %d", &PieceR[i][0], &PieceR[i][1], &PieceR[i][2], &PieceR[i][3]);
			for (int i = 0; i < mN; i++) scanf("%d %d %d %d", &PieceD[i][0], &PieceD[i][1], &PieceD[i][2], &PieceD[i][3]);
			for (int i = 0; i < mN; i++) scanf("%d %d %d %d", &PieceL[i][0], &PieceL[i][1], &PieceL[i][2], &PieceL[i][3]);

			init(mN, mM, PieceU, PieceR, PieceD, PieceL);
			isOK = 1;
			break;

		case CMD_PUT:
			scanf("%d %d %d %d", &mPiece[0], &mPiece[1], &mPiece[2], &mPiece[3]);
			result = put(mPiece);
			scanf("%d", &check);
			if (result != check)
				isOK = 0;
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