#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <iostream>

#define CMD_INIT 100
#define CMD_ADD 200
#define CMD_MOVE 300
#define CMD_ERASE 400
#define CMD_GET 500

extern void init();
extern void destroy();
extern int addCommand(char mCommand[]);
extern int moveCursor(int mPos);
extern void eraseCommand();
extern int getValue(char mVariable);
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#define MAX_L 200
static int Score;

static void cmd_init()
{
	init();
}

static void cmd_add()
{
	char command[MAX_L];
	scanf("%s", command);
	int result = addCommand(command);
	int check;
	scanf("%d", &check);
	if (result != check) {
		std::cout << "==== WRONG ANS : " << result << std::endl;

		Score = 0;
	}
}

static void cmd_move()
{
	int pos;
	scanf("%d", &pos);
	int result = moveCursor(pos);
	int check;
	scanf("%d", &check);
	if (result != check) {
		std::cout << "==== WRONG ANS : " << result << std::endl;

		Score = 0;
	}
}

static void cmd_erase()
{
	eraseCommand();
}

static void cmd_get()
{
	char variable[2];
	scanf("%s", variable);
	int result = getValue(variable[0]);
	int check;
	scanf("%d", &check);
	if (result != check) {
		std::cout << "==== WRONG ANS : " << result << std::endl;

		Score = 0;
	}
}

static int run()
{
	int N;
	scanf("%d", &N);
	Score = 1;
	for (int i = 0; i < N; ++i)
	{
		int cmd;
		scanf("%d", &cmd);
		switch (cmd)
		{
		case CMD_INIT: cmd_init(); break;
		case CMD_ADD: cmd_add(); break;
		case CMD_MOVE: cmd_move(); break;
		case CMD_ERASE: cmd_erase(); break;
		case CMD_GET: cmd_get(); break;
		default: break;
		}
	}
	destroy();
	return Score;
}

int main()
{
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	int T, MARK;
	scanf("%d %d", &T, &MARK);

	for (int tc = 1; tc <= T; tc++)
	{
		if (run() == 1) printf("#%d %d\n", tc, MARK);
		else printf("#%d %d\n", tc, 0);
	}
	return 0;
}