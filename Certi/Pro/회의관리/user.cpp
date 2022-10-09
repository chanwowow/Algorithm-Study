#define MAXM (10)
#define MAXL (11)

#include <map>
#include <unordered_map>
#include <vector>
#include <list>

#include <iostream>
using namespace std;

unordered_map <string, int> mID_Hash;
unordered_map<string, int> pID_Hash;
int newh_mID, newh_pID;

struct myStr1 { int EndTime; int h_mID; };
map<int, myStr1> pSchedule[20000];

struct {
	string MeetingName;
	int h_mID;
	int StartTime; int EndTime;
	vector <int> ppl;
} meetingDB[10000];

bool meetInsert(int StartTime, int EndTime, int h_pID) {
	bool can = false;

	auto low = pSchedule[h_pID].lower_bound(EndTime);
	if (low == pSchedule[h_pID].end() || low->first != EndTime) {// 뒤 조건
		// 앞 조건
		if (low == pSchedule[h_pID].begin()) can = true;
		else {
			low--;
			if (low->second.EndTime < StartTime) can = true;
		}
	}
	return can;
}

void init(){ 
	for (int i = 0; i < newh_mID; i++) {
		meetingDB[i] = {};
	}
	for (int i = 0; i < newh_pID; i++) {
		pSchedule[i] = {};
	}
	mID_Hash.clear();
	pID_Hash.clear();
	newh_mID = newh_pID = 1;
}

int addMeeting(char mMeeting[MAXL], int M, char mMemberList[MAXM][MAXL], int mStartTime, int mEndTime){
	int h_mID = mID_Hash[mMeeting];
	if (h_mID == 0) h_mID = mID_Hash[mMeeting] = newh_mID++;
	meetingDB[h_mID] = { mMeeting, h_mID ,mStartTime, mEndTime};

	// 참가자 스케줄파악
	for (int i = 0; i < M; i++) {
		int h_pID = pID_Hash[mMemberList[i]];
		if (h_pID == 0)  h_pID = pID_Hash[mMemberList[i]] = newh_pID++;

		bool can = meetInsert(mStartTime, mEndTime, h_pID);
		// 스케줄 가능한 사람만 넣는다
		if (can) {
			meetingDB[h_mID].ppl.push_back(h_pID);
			pSchedule[h_pID][mStartTime] = {mEndTime, h_mID	};
		}
	}
	return meetingDB[h_mID].ppl.size();
}

int cancelMeeting(char mMeeting[MAXL]){
	int h_mID = mID_Hash[mMeeting];
	if (h_mID == 0 || meetingDB[h_mID].ppl.size() == 0) return 0;
	// 있는 회의라면 정보삭제
	int StartTime = meetingDB[h_mID].StartTime;
	for (auto h_pID : meetingDB[h_mID].ppl) {
		pSchedule[h_pID].erase(StartTime);
	}
	meetingDB[h_mID].ppl.clear();

	return 1;
}

int changeMeetingMember(char mMeeting[MAXL], char mMember[MAXL]){
	int caseN = 2; //(0: 삭제, 1: 삽입 2: 삽입불가)
	//1. 없는 회의인지, 없는 사람인지 체크
	int h_mID = mID_Hash[mMeeting];
	if (h_mID == 0 || meetingDB[h_mID].ppl.size() == 0) return -1;
	int StartTime = meetingDB[h_mID].StartTime; int EndTime = meetingDB[h_mID].EndTime;

	int h_pID = pID_Hash[mMember];
	if (h_pID == 0) h_pID = pID_Hash[mMember] = newh_pID++;
	//2. 있는 회의라면 돌면서인간 찾으면 삭제
	for (auto itr = meetingDB[h_mID].ppl.begin(); itr != meetingDB[h_mID].ppl.end(); itr++) {
		if (*itr == h_pID) {
			caseN = 0;
			itr = meetingDB[h_mID].ppl.erase(itr);
			pSchedule[h_pID].erase(StartTime);
			break;
		}
	}
	if (caseN == 2) {
		bool can = meetInsert(StartTime, EndTime, h_pID);
		// 스케줄 가능시 넣음
		if (can) {
			caseN = 1;
			meetingDB[h_mID].ppl.push_back(h_pID);
			pSchedule[h_pID][StartTime] = { EndTime, h_mID };
		}
	}
	return caseN;
}

int changeMeeting(char mMeeting[MAXL], int mStartTime, int mEndTime){
	int h_mID = mID_Hash[mMeeting];
	if (h_mID == 0 || meetingDB[h_mID].ppl.size() == 0) return 0;
	for (auto itr = meetingDB[h_mID].ppl.begin(); itr != meetingDB[h_mID].ppl.end();) {
		
		pSchedule[*itr].erase(meetingDB[h_mID].StartTime);
		bool can = meetInsert(mStartTime, mEndTime, *itr);

		if (!can) {
			itr = meetingDB[h_mID].ppl.erase(itr);
			continue;
		}
		pSchedule[*itr][mStartTime] = {mEndTime, h_mID};
		itr++;
	}
	meetingDB[h_mID].StartTime = mStartTime;
	meetingDB[h_mID].EndTime = mEndTime;

	return meetingDB[h_mID].ppl.size();
}

void checkNextMeeting(char mMember[], int mTime, char mResult[]){
	int h_pID = pID_Hash[mMember];

	auto upper = pSchedule[h_pID].upper_bound(mTime);
	if (upper != pSchedule[h_pID].end()) {
		int h_mID = upper->second.h_mID;
		int i = 0;
		for (i; meetingDB[h_mID].MeetingName[i]!='\0'; i++) mResult[i] = meetingDB[h_mID].MeetingName[i];
		mResult[i] = '\0';
	} 
	else mResult[0] = '\0';
}

