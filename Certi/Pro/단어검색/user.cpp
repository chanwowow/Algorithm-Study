#include <memory.h>
#include <string>
#include <iostream>

using namespace std;

int sameN;
int searchN;
int removeN;
/*
struct TRIE {
	int isWord;
	TRIE* child[26];

	TRIE() {
		isWord = 0;
		memset(child, 0, sizeof(child));
	}

	~TRIE() {
		for (int i = 0; i < 26; i++) {
			if (child[i] != NULL)
				delete child[i];
		}
	}

	void Insert(const char* str) {
		if (*str == NULL) {
			isWord++;
			sameN = isWord;
			return;
		}
		int curr = *str - 'a';
		if (child[curr] == NULL) child[curr] = new TRIE;
		child[curr]->Insert(++str);
	}

	void Search(const char* str) {
		if (*str == NULL) {
			if (isWord > 0) {
				searchN += isWord;
			}
			return;
		}
		if (*str == '?') {
			for (int curr = 0; curr < 26; curr++) {
				if (child[curr] == NULL) continue;
				child[curr]->Search(str+1);
			}
		}
		else {
			int curr = *str - 'a';
			if (child[curr] == NULL) return;
			child[curr]->Search(str+1);
		}
	}

	void Remove(const char* str) {
		if (*str == NULL) {
			if (isWord > 0) {
				removeN += isWord;
				isWord = 0;
			}
			return;
		}
		if (*str == '?') {
			for (int curr = 0; curr < 26; curr++) {
				if (child[curr] == NULL) continue;
				child[curr]->Remove(str+1);
			}
		}
		else {
			int curr = *str - 'a';
			if (child[curr] == NULL) return;
			child[curr]->Remove(str+1);
		}
	}

};
TRIE* Root = new TRIE();

*/
struct TRIE {
	int isWord;
	TRIE* child[26];
} TRIEPool[530000];
int PoolCnt;

TRIE* newTrie() {
	TRIE* Node = &TRIEPool[PoolCnt++];
	memset(Node, 0, sizeof(Node));
	return Node;
}
void Insert(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		t2s->isWord++;
		sameN = t2s->isWord;
		return;
	}
	int curr = *str - 'a';
	if (t2s->child[curr] == NULL)t2s->child[curr] = newTrie();
	Insert(++str, t2s->child[curr]);
}
void Search(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		if (t2s->isWord > 0) {
			searchN += t2s->isWord;
		}
		return;
	}
	if (*str == '?') {
		for (int curr = 0; curr < 26; curr++) {
			if (t2s->child[curr] == NULL) continue;
			Search(str + 1, t2s->child[curr]);
		}
	}
	else {
		int curr = *str - 'a';
		if (t2s->child[curr] == NULL) return;
		Search(str + 1, t2s->child[curr]);
	}
}

void Remove(const char* str, TRIE* t2s) {
	if (*str == NULL) {
		if (t2s->isWord > 0) {
			removeN += t2s->isWord;
			t2s->isWord = 0;
		}
		return;
	}
	if (*str == '?') {
		for (int curr = 0; curr < 26; curr++) {
			if (t2s->child[curr] == NULL) continue;
			Remove(str + 1, t2s->child[curr]);
		}
	}
	else {
		int curr = *str - 'a';
		if (t2s->child[curr] == NULL) return;
		Remove(str + 1, t2s->child[curr]);
	}
}

TRIE* Root;

void init() {

	//Root->~TRIE();  // 소멸자
	//Root = new TRIE();

	PoolCnt = 0; // 초기화
	Root = newTrie();
	
	return;
}

int add(char str[]) {
	sameN = 0;
	const char* str1 = str;
	
	//Root->Insert(str1);
	Insert(str1, Root);

	return sameN;
}

int remove(char str[]) {
	removeN = 0;
	const char* str1 = str;

	//Root->Remove(str1);
	Remove(str1, Root);

	return removeN;
}

int search(char str[]) {
	searchN = 0;
	const char* str1 = str;
	
	//Root->Search(str1);
	Search(str1, Root);

	return searchN;
}