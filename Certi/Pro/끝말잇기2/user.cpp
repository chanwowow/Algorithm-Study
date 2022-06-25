#define MAX_N 50000
#define MAX_M 50000
#define MAX_LEN 11

#include <string>
#include <memory.h>
#include <set>  
#include <unordered_map>

using namespace std;

int player_life[MAX_N + 1];
set <string> VOCA[26]; // a:0 ~ z :25
unordered_map <string, int> Used;

string v_arr[50000];  /// hmmm 
int v_idx;
int max_idx;

void init(int N, int M, char mWords[][MAX_LEN])
{
	Used.clear();
	memset(player_life, 0, sizeof(player_life));
	max_idx = N;

	for (int i = 0; i < 26; i++) {
		VOCA[i].clear();
	}

	// ���� �ʱⵥ���� ���� //
	for (int i = 0; i < M; i++) {
		VOCA[mWords[i][0] - 97].insert(mWords[i]);  // �ش� ���ĺ� idx�� �ش��ϴ� map�� �ܾ� ����
	}

}

int playRound(int mID, char mCh)
{
	int player_idx = mID;
	char f_c = mCh;
	v_idx = 0;

	while (!VOCA[f_c - 97].empty()) {

		v_arr[v_idx] = *(VOCA[f_c - 97].begin()); // �� �� �ܾ� array �� ����
		VOCA[f_c - 97].erase(v_arr[v_idx]); // �ش� �ܾ� ���� ����
		Used.insert({ v_arr[v_idx], 1 }); // ���õ� �ܾ��� ǥ��

		f_c = v_arr[v_idx][v_arr[v_idx].length() - 1]; // �ܾ� �� ���� ������Ʈ
		v_idx++;

		do {
			player_idx++;
			if (player_idx > max_idx) player_idx = 1; // ������ �ٽ� ���ƿ���

		} while (player_life[player_idx] == 1);

	}

	player_life[player_idx] = 1;  //D������ ǥ��

	string rev_word;
	// �ܾ�� ������ ���� �۾�
	for (int i = 0; i < v_idx; i++) {
		rev_word = "";
		for (int j = v_arr[i].length() - 1; j >= 0; j--) {
			rev_word += v_arr[i][j];
		}
		if (Used[rev_word] != 1) VOCA[rev_word[0] - 97].insert(rev_word); // ������ ���� ���̸� �ܾ� ����
	}
	return player_idx;
}