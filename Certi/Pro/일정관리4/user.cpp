#define MAXL		(11)

#include <map>
#include <unordered_map>
#include <memory.h>
#include <iostream>

using namespace std;

struct RESULT
{
	char mTitle[MAXL];
	int mStartDay;
	int mEndDay;
};
struct str1 {
	int dayType; // 1 시작일, 2 종료일, 3 시작+종료
	int hID;
};
struct str2 {
	string sdlName;
	int stDate;
	int edDate;
	bool deleted;
} sdlDB[30005];

map <int, str1> calander;
unordered_map<string, int> sdlHash;
int newhID;


int gN;
void init(int N){
	gN = N;
	calander.clear();
	sdlHash.clear();
	newhID = 1;

	calander[0] = { 2 }; // 종료일로 지정ㅇ
	calander[N + 1] = { 18 }; // 아무거나로 지정

}

void InsertSchedule(string mTitle, int mStartDay, int mEndDay) {
	int hID = sdlHash[mTitle] = newhID++;
	sdlDB[hID] = { mTitle,mStartDay, mEndDay, false };

	if (mStartDay == mEndDay) {
		calander[mStartDay] = { 3, hID };
	}
	else {
		calander[mStartDay] = { 1,hID };
		calander[mEndDay] = { 2, hID };
	}
}

int addSchedule(char mTitle[], int mStartDay, int mEndDay, int mForced){
	bool can = false;
	string Name = mTitle;
	if (mForced == 0) { // 일반모드
		if (calander.find(mStartDay) == calander.end() && calander.find(mEndDay) == calander.end()) {
			auto upper1 = calander.upper_bound(mStartDay); upper1--;
			if (upper1->second.dayType != 1) { // 시작일이 아니고
				auto upper2 = calander.upper_bound(mEndDay); upper2--;
				if (upper1 == upper2) {
					can = true;
					// 삽입할 수 있다.
					InsertSchedule(mTitle, mStartDay, mEndDay);
				}
			}
		}

	}
	else {
		can = true;

		auto upper = calander.upper_bound(mStartDay); upper--;
		while (upper->first <= mEndDay) {
			int hID = upper->second.hID;

			if (upper->second.dayType == 1) {
				advance(upper, 2);

				sdlDB[hID].deleted = true;
				calander.erase(sdlDB[hID].stDate);
				calander.erase(sdlDB[hID].edDate);
			}
			else {
				if (upper->first >= mStartDay) {
					advance(upper, 1);
					sdlDB[hID].deleted = true;
					calander.erase(sdlDB[hID].stDate);
				}
				else advance(upper, 1);
			}

		}
		InsertSchedule(mTitle, mStartDay, mEndDay);
	}
	return can;
}

RESULT getSchedule(int mDay){
	RESULT ret;

	auto upper = calander.upper_bound(mDay); upper--;
	if (upper->second.dayType != 1 && upper->first!=mDay) {
		ret.mTitle[0] = '\0';
	}
	else {
		int hID = upper->second.hID;

		string name = sdlDB[hID].sdlName;
		for (int i = 0; i < name.length(); i++) {
			ret.mTitle[i] = name[i];
		}
		ret.mTitle[name.length()] = '\0';
		
		ret.mStartDay = sdlDB[hID].stDate;
		ret.mEndDay = sdlDB[hID].edDate;
	}

	return ret;
}

int deleteSchedule(char mTitle[]){
	bool can = false;
	int hID = sdlHash[mTitle];
	if (hID == 0) can = false;
	else if (sdlDB[hID].deleted == true) can = false;
	else {
		can = true;
		sdlDB[hID].deleted = true;
		calander.erase(sdlDB[hID].stDate);
		calander.erase(sdlDB[hID].edDate);
	}
	return can;

}

int findEmptySchedule(){
	int ans = 0;

	auto itr = calander.begin();
	while (itr->first < gN) {

		int prev = itr++->first;
		int val = -(prev - (itr->first)) - 1;
		ans = max(ans, val );

		if (itr->second.dayType == 1) itr++;
	}

	return ans;
}

