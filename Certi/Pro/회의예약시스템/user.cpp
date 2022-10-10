#define MaxMem 10003
#define MaxTeam 3003
#define MAX_STR_LEN (13)

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <memory.h>
#include <iostream>


/*
### 다 풀었는데 오답 TC 못찾겠다
그것만 디버깅 ㅎ면 된다

*/
using namespace std;

struct meetStr {
	int Day, Start, Dur;
};
struct cmp {
	bool operator()(meetStr a, meetStr b)const {
		if (a.Day == b.Day) {
			if (a.Start == b.Start) {
				return a.Dur < b.Dur;
			}return a.Start < b.Start;
		}return a.Day < b.Day;
	}
};

// 이름 해시맵
unordered_map<string, int> teamNameHash;  int newTeamID;
unordered_map<string, int> memNameHash; int newMemID;
// 팀 DB
struct {
	int memList[31];
	meetStr meet;
} teamDB[MaxTeam];

//개인소속팀 DB
unordered_set <int> teamInvolved[MaxMem];

// 문풀용 temp 자료
// 팀원들의 스케줄
set<meetStr, cmp> teamSchedule;
// 팀원들의 소속팀 정리
set<int> teamListTemp;

int gDay;

void init(int N, char mName[][MAX_STR_LEN]){
	newTeamID = newMemID = 0;
	teamNameHash.clear();
	memNameHash.clear();
	gDay = 8;

	for (int i = 0; i < N; i++) {
		int memID = memNameHash[mName[i]] = newMemID++;
		teamInvolved[memID].clear();
	}
}

void createTeam(char mTeamName[], int M, char mName[][MAX_STR_LEN]){
	int teamID = teamNameHash[mTeamName] = newTeamID++;
	teamDB[teamID] = {}; // 한번에 초기화와 속도비교

	teamDB[teamID].memList[0] = M;
	for (int i = 0; i < M; i++) {
		int memID = memNameHash[mName[i]];
		teamDB[teamID].memList[i+1] = memID;
		teamInvolved[memID].insert(teamID);
	}
}

int bookMeeting(char mTeamName[], int mMinutes){
	//cout << " Today :  " << gDay<<endl;
	int thisWeek = gDay / 7;
	int today = gDay % 7; // 회의시작가능성 날짜
	if (today == 6) { ++thisWeek; today = 1; }
	else if (today == 0) today = 1;

	int teamID = teamNameHash[mTeamName];
	teamListTemp.clear();
	teamSchedule.clear();

	// 기예약 취소 및 팀 추합
	meetStr backUp = teamDB[teamID].meet;
	teamDB[teamID].meet = {};
	for (int i = 1; i <= teamDB[teamID].memList[0]; i++) {
		for (auto itr : teamInvolved[teamDB[teamID].memList[i]]) {
			teamListTemp.insert(itr);
		}
	}
	// 전체 팀 스케줄 정리
	for (auto itr : teamListTemp) {
		if (teamDB[itr].meet.Day == 0) continue;
		if (teamDB[itr].meet.Day < today)
			teamSchedule.insert({ teamDB[itr].meet.Day+7, teamDB[itr].meet.Start, teamDB[itr].meet.Dur });
		else
			teamSchedule.insert(teamDB[itr].meet);
	}

	struct {
		int w, d, hr, min;
	} result = {};

	// 회의시간 정하는 부분
	// 시작과 끝부분 지정
	teamSchedule.insert({ today, 540, 0 }); 
	if(today ==1)  teamSchedule.insert({ 5, 1080, 0 });
	else teamSchedule.insert({ today + 6, 1080, 0 });

	auto itr = teamSchedule.find({today, 540, 0});

	if (gDay == 34) {
		int sjs = 1;
	}

	for (int i = 1; i < teamSchedule.size(); i++) {
		meetStr a = *itr++;
		meetStr b = *itr;

		if (a.Day != b.Day) {
			if (mMinutes <= 1080 - (a.Start + a.Dur - 1)) {
				result = { thisWeek + a.Day / 7, a.Day % 7, a.Start + a.Dur,0 };
				break;
			}

			int tmr = a.Day+1;
			if (a.Day == 5) tmr = 8;

			teamSchedule.insert({ tmr, 540, 0 });
			itr = teamSchedule.find({tmr, 540 ,0 });
			--i;
		}
		else {
			if (mMinutes <= (b.Start - 1) - (a.Start + a.Dur - 1)) {
				result = { thisWeek + a.Day / 7, a.Day % 7, a.Start + a.Dur,0 };
				break;
			}
		}
	}
	if (result.hr == 0) { teamDB[teamID].meet = backUp;  return 0; }

	int hr = result.hr;
	result.hr = hr / 60;
	result.min = hr % 60;
	teamDB[teamID].meet = {result.d, hr, mMinutes};
	int ret = 100000 * result.w + 10000 * result.d + 100 * result.hr + result.min;
	cout << " NEXT Time Can:" << (hr+mMinutes)/60*100 + (hr + mMinutes) % 60 <<endl;

	return ret;
}

void timeElapse(int mDays){
	gDay += mDays;
}