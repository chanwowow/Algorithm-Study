#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>
#include<string.h>

#include <iostream>

#define MAX_N (10000)
#define MAX_STR_LEN (13)
#define TEAM_RATIO (20)
#define BOOK_RATIO (80)
#define ELAPSE_RATIO (100)

extern void init(int N, char mName[][MAX_STR_LEN]);
extern void createTeam(char mClubName[], int M, char mName[][MAX_STR_LEN]);
extern int bookMeeting(char mTeamName[], int mMinutes);
extern void timeElapse(int mDays);

static unsigned int seed = 5;
static int pseudo_rand() {
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7fff;
}

static void make_string(char str[])
{
	int len = pseudo_rand() % 7 + 6;

	for (int i = 0; i < len; i++)
	{
		str[i] = pseudo_rand() % 26 + 'a';
	}
	str[len] = '\0';
}

static char person_name[MAX_N][MAX_STR_LEN];
static char team_name[MAX_N][MAX_STR_LEN];
static int team_size;
static int team_arr[MAX_N * 2];
static char team_member_name[MAX_N][MAX_STR_LEN];

static void init()
{
	for (int i = 0; i < MAX_N * 2; i++)
	{
		team_arr[i] = -1;
	}
}

static int run(int score)
{
	int n;
	int m;
	int query_cnt;

	scanf("%d%d%d%d", &seed, &n, &m, &query_cnt);

	team_size = 0;


	for (int i = 0; i < n; i++)
	{
		make_string(person_name[i]);
	}
	init(n, person_name);

	for (int query = 0; query < query_cnt; query++)
	{
		int ratio = pseudo_rand() % 100;
		if (ratio < TEAM_RATIO || query < n / 2)
		{
			make_string(team_name[team_size]);
			int team_member_cnt = pseudo_rand() % m + 2;

			for (int k = 0; k < team_member_cnt; k++)
			{
				do
				{
					int idx = pseudo_rand() % n;
					if (team_arr[idx] == team_size)
					{
						continue;
					}

					team_arr[idx] = team_size;
					strcpy(team_member_name[k], person_name[idx]);
					break;

				} while (1);
			}

			createTeam(team_name[team_size], team_member_cnt, team_member_name);

			team_size++;
		}
		if (ratio < BOOK_RATIO)
		{
			int mMins = pseudo_rand() % 81 + 40;
			int idx = pseudo_rand() % team_size;
			int user_ans = bookMeeting(team_name[idx], mMins);
			int correct_ans;
			scanf("%d", &correct_ans);

			if (correct_ans != user_ans) {
				std::cout << " =========> Wrong ANS :" << user_ans << std::endl;
				score = 0;
			}
				
		}
		else
		{
			int mDays = pseudo_rand() % 7 + 1;
			timeElapse(mDays);
		}
	}


	return score;
}
int main()
{
	setbuf(stdout, NULL);
	//	freopen("sample_input.txt", "r", stdin);
	int tc, score;
	scanf("%d%d", &tc, &score);

	for (int t = 1; t <= tc; t++)
	{
		init();
		int tc_score = run(score);

		printf("#%d %d\n", t, tc_score);
	}

	return 0;
}