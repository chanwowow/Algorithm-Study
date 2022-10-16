#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<string.h>

#define MAX_SIZE (100005)

extern void openDocument(int, char[], int);
extern void moveCursor(int, int, int);
extern void addNewLine();
extern void backSpace(int);
extern void getRow(int, char[]);

static unsigned int seed = 5;
static int pseudo_rand() {
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7fff;
}

static char mContents[MAX_SIZE];
static int new_line;
static char user_ans_str[1000];
static char ans_str[1000];

static void make_str(char str[], int len)
{
	for (int i = 0; i < len; i++)
	{
		str[i] = (char)(pseudo_rand() % 26) + 'a';
	}
	str[len] = 0;
}

static int run(int _score)
{
	int n, m;
	scanf("%d%d%d%d", &n, &seed, &m, &new_line);

	int ret = _score;

	make_str(mContents, n);

	for (int i = 0; i < new_line; i++)
	{
		int idx = (pseudo_rand() * pseudo_rand()) % (n - 1);

		mContents[idx] = '\n';
	}

	openDocument(n, mContents, m);

	int tot_query = 0;
	int tot_page = n / (m * m) + new_line / m + 1;

	scanf("%d", &tot_query);

	for (int query = 0; query < tot_query; query++)
	{
		int mPage = pseudo_rand() % tot_page + 1;
		int correct_ans;
		int row = pseudo_rand() % m + 1;
		int col = pseudo_rand() % m + 1;

		moveCursor(mPage, row, col);

		int comm = pseudo_rand() % 2;
		int len;
		switch (comm)
		{
		case 0:
			addNewLine();

			break;
		case 1:
			len = pseudo_rand() % 10 + 1;
			backSpace(len);

			break;

		default:
			break;
		}

		int get_row_cnt = pseudo_rand() % 5 + 1;

		for (int i = 0; i < get_row_cnt; i++)
		{
			row = pseudo_rand() % m + 1;

			getRow(row, user_ans_str);


			int ans_l = 0;
			scanf("%d", &ans_l);

			if (ans_l)
			{
				scanf("%s", ans_str);
				if (ans_str[ans_l - 1] == '$')
					ans_str[ans_l - 1] = '\n';

			}
			else
			{
				ans_str[0] = 0;
			}

			int l = strlen(user_ans_str);
			if (l != ans_l || strcmp(user_ans_str, ans_str))
			{
				ret = 0;
			}
		}
	}

	return ret;
}

int main()
{
	setbuf(stdout, NULL);
	//	freopen("sample_input.txt", "r", stdin);

	int tc, score;
	scanf("%d%d", &tc, &score);

	for (int t = 1; t <= tc; t++)
	{
		printf("#%d %d\n", t, run(score));
	}

	return 0;
}
