#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>

#define MAX_N 131072
#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

extern void modifyTournament(int N, int mOrder[]);

static int org[MAX_N];
static int winners[MAX_N / 2];
static int losers[MAX_N / 2];
static int copied[MAX_N];

static int N;
static bool ok;

void swapOrder(int mIndexA, int mIndexB)
{
	if (!ok || mIndexA < 0 || mIndexA >= N || mIndexB < 0 || mIndexB >= N)
	{
		ok = false;
		return;
	}

	int tmp = org[mIndexA];
	org[mIndexA] = org[mIndexB];
	org[mIndexB] = tmp;
}

static bool isRightGroup(int s, int e) {
	if (e - s <= 1)
		return true;

	int mid = (e + s) / 2;

	for (int i = 0; i < mid - s; i++)
	{
		winners[i] = MIN(org[s + 2 * i], org[s + 2 * i + 1]);
		losers[i] = MAX(org[s + 2 * i], org[s + 2 * i + 1]);

		if (winners[i] > mid || losers[i] <= mid)
			return false;
	}

	for (int i = 0; i < mid - s; i++)
	{
		org[s + i] = winners[i];
		org[mid + i] = losers[i];
	}

	return isRightGroup(s, mid) && isRightGroup(mid, e);
}

static bool run()
{
	ok = true;

	scanf("%d", &N);
	for (int i = 0; i < N; i++)
	{
		scanf("%d", &org[i]);
		copied[i] = org[i];
	}
	modifyTournament(N, copied);

	return ok && isRightGroup(0, N);
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