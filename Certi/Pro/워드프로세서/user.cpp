const int maxTrie = 15000;

#include <cstring>
#include <string>
#include <set>


using namespace std;

set <string> wordList;

struct TRIE {
	bool isWord;
	char thisWord[11];
	TRIE* child[26];
} triePool[maxTrie]; int poolCnt;
TRIE* Root;

TRIE* newTRIE() {
	TRIE* ptr = &triePool[poolCnt++];
	*ptr = {};
	return ptr;
}

string fullWord;
string gWord;

void Insert(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		t2s->isWord = true;
		if (t2s->thisWord[0] == 0) {
			strcpy(t2s->thisWord, gWord.c_str());
		}return;
	}
	int curr = *str - 'a';
	if (t2s->child[curr] == NULL) t2s->child[curr] = newTRIE();
	Insert((str + 1), t2s->child[curr]);
}

void Remove(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		t2s->isWord = false;
		return;
	}
	int curr = *str - 'a';
	Remove((str + 1), t2s->child[curr]);
}

bool Find1(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		if (t2s->isWord == true) {
			return true;
		}return false;
	}
	int curr = *str - 'a';
	if (t2s->child[curr] == NULL)return false;
	return Find1((str + 1), t2s->child[curr]);
}

bool Find(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		if (t2s->isWord == true) {
			wordList.insert(t2s->thisWord);
			return true;
		}return false;
	}
	else if (*str == '#') {  // 조커 나오면
		for (int i = 0; i < 26; i++) {
			if ((t2s->child[i] != NULL)) {
				if (Find((str + 1), t2s->child[i])) return true;
			}
		}return false;
	}
	else {
		int curr = *str - 'a';
		if (t2s->child[curr] == NULL)return false;
		return Find((str + 1), t2s->child[curr]);
	}
}

void init(int N, char mString[]) {
	// 초기화
	poolCnt = 0;
	Root = newTRIE();
	//초기값
	fullWord = mString;
}

void addWord(char mWord[]) {
	gWord = mWord;
	Insert(mWord, Root);
}

void removeWord(char mWord[]) {
	Remove(mWord, Root);
}

int correct(int mStart, int mEnd) {
	int ret = 0;
	char chk[11] = {};
	for (register int i = mStart; i <= mEnd; i++) {
		int idx1 = i;
		int j = 0;
		while (i <= mEnd && fullWord[i] != '_') {
			chk[j++] = fullWord[i++];
		}chk[j] = '\0';

		// 이번에 체크할 단어 : chk
		if (Find1(chk, Root)) { continue; } // 이미 존재단어면 skip

		// 단어 없다면 사전 순으로 조커 집어넣는다.
		char charTemp;
		int k = 0;
		for (k; k < j; k++) {
			if (k != 0) chk[k - 1] = charTemp; // 이전것 원복
			charTemp = chk[k]; // 단어 새로 백업
			chk[k] = '#'; // 조커새로삽입
			Find(chk, Root);
		}
		if (!wordList.empty()) {
			int l = 0;
			char cpy[11];
			auto itr = wordList.begin();

			strcpy(cpy, itr->c_str());
			while (cpy[l] != NULL) {
				fullWord[idx1++] = cpy[l++];
			}
			wordList.clear();
			++ret;
		}
	}
	return ret;
}

void destroy(char mResult[]) {
	strcpy(mResult, fullWord.c_str());
}