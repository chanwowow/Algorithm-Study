const int Max_Hotel = 1001;
const int Max_Room =101;
const int Max_Region =11;
const int Max_Bed =11;
const int Max_Rtype= 5;
const int Max_Vtype =5;


#include <unordered_map>
#include <queue>
#include <vector>	
#include <memory.h>
//#include <iostream>

using namespace std;
// 호텔 방별 정보///
int Hotel_ID_Price[Max_Hotel][Max_Room]; // 호텔 ID , 방 idx로 찾아간다
struct hotel {
	int Hotel_ID;
	int Room_idx;
	int cal_idx;
};
unordered_map<int, hotel> Id_Hash;
int Room_N[Max_Hotel]; // 룸넘버 idx 할당하기 위함

// 호텔 조건별로 ID 분류/
struct filt {
	int Room_ID;
	int Price;
	int BedN;
} temp;
struct pq_up {
	bool operator ()(filt a, filt b) {
		if (a.Price == b.Price) return a.Room_ID > b.Room_ID;
		return a.Price > b.Price;
	}
};
priority_queue <filt, vector<filt>, pq_up> findResult[Max_Region][Max_Rtype][Max_Vtype]; // first: RoomID, second:Bed수
vector<filt> for_Repush;// 재푸시용

vector<pair<int,int>> Calander[100000];
int cal_idx_g;

//ㅁㅁㅁㅁㅁㅁㅁ
void init(int N, int mRoomCnt[]) {
	// 초기화 코드/.
	for (int i = 0; i <cal_idx_g; i++) Calander[i].clear();
	cal_idx_g = 0;
	memset(Hotel_ID_Price, 0, sizeof(Hotel_ID_Price));
	memset(Room_N, 0, sizeof(Room_N));
	Id_Hash.clear();
	for (int i = 1; i < Max_Region; i++) {
		for (int k = 1; k < Max_Rtype; k++) {
			for (int t = 1; t < Max_Vtype; t++) {
				findResult[i][k][t] = {};
			}
		}
	}
}
											//0지역 1침대수 2룸타입 3조망타입 4가격
void addRoom(int mHotelID, int mRoomID, int mRoomInfo[]){
	
	int Room_idx = Room_N[mHotelID - 1]++;

	Id_Hash[mRoomID] = { mHotelID, Room_idx , cal_idx_g++ };

	Hotel_ID_Price[mHotelID - 1][Room_idx] = mRoomInfo[4];
	findResult[mRoomInfo[0]][mRoomInfo[2]][mRoomInfo[3]].push({ mRoomID ,mRoomInfo[4],mRoomInfo[1] });
	
}
/*
* // 얘로 하면 터진다 c바
int findRoom(int mFilter[]) {
	bool can;
	int Hotel_ID, Room_idx, cal_idx;
	int Result_ID = 0;
	
	if (findResult[mFilter[2]][mFilter[4]][mFilter[5]].empty()) return -1;

	for (auto& itr : findResult[mFilter[2]][mFilter[4]][mFilter[5]]) {
		if (itr.second != mFilter[3]) continue;

		can = true;
		cal_idx = Id_Hash[itr.first].cal_idx;

		// itr로 해싱
		for (auto& day : Calander[cal_idx]) {
			if (day.first >= mFilter[1] || day.second <= mFilter[0]) continue;
			else {
				can = false; break; //만약 겹치는 날 이미 있으면 불가
			}
				
		}
		// 조건해당하면 PQ 집어넣는 부분//
		if (can == true) {
			Hotel_ID = Id_Hash[itr.first].Hotel_ID;
			Room_idx = Id_Hash[itr.first].Room_idx;
			PQ.push({ Hotel_ID_Price[Hotel_ID - 1][Room_idx],itr.first });
		}
	}

	if (PQ.empty()) return -1;
	//  pq TOP 날짜에 예약
	cal_idx = Id_Hash[PQ.top().Room_ID].cal_idx;
	Calander[cal_idx].push_back({ mFilter[0] ,mFilter[1] });
	
	
	return Result_ID;
}
*/


int findRoom(int mFilter[]) {
	bool vacant;
	int Hotel_ID, Room_idx, cal_idx, new_Price;
	int Result_ID = 0;

	for_Repush.clear();

	if (findResult[mFilter[2]][mFilter[4]][mFilter[5]].empty()) return -1;

	while (!findResult[mFilter[2]][mFilter[4]][mFilter[5]].empty()) {
		temp = findResult[mFilter[2]][mFilter[4]][mFilter[5]].top();
	//	cout << " %%%%TEMP Room ID:  " << temp.Room_ID << " Price :" << temp.Price << endl;

		findResult[mFilter[2]][mFilter[4]][mFilter[5]].pop();
		// 1. BED 조건이 안맞는 경우
		if (temp.BedN != mFilter[3]) {
			for_Repush.push_back(temp);
			continue;
		}
		
		Hotel_ID = Id_Hash[temp.Room_ID].Hotel_ID;
		Room_idx = Id_Hash[temp.Room_ID].Room_idx;
		cal_idx= Id_Hash[temp.Room_ID].cal_idx;
		new_Price = Hotel_ID_Price[Hotel_ID - 1][Room_idx];
		// 2. 가격이 최신 정보가 아닌 경우
		if (temp.Price != new_Price) {
			//가격이 최신 정보가 아니어도 뺀다. 대신 얘는 바로 PQ 업데이트
			findResult[mFilter[2]][mFilter[4]][mFilter[5]].push({ temp.Room_ID, new_Price, temp.BedN });
			continue;
		}
		
		// 3. 해당 날짜에 그방 미리 예약이 된 경우
		vacant = true;
		for (auto& day : Calander[cal_idx]) {
			if (day.first >= mFilter[1] || day.second <= mFilter[0]) continue;
			else {
				vacant = false;
				break; //만약 하나라도 겹치는 날 이미 있으면 불가
			} 
		}
		if (vacant == false) {
			for_Repush.push_back(temp);
			continue;
		}

		//4. 모든 조건에 부합 한 경우, 예약 후, 재 푸시,  for문 종료
		Calander[cal_idx].push_back({ mFilter[0] ,mFilter[1] });
		findResult[mFilter[2]][mFilter[4]][mFilter[5]].push(temp);
		Result_ID = temp.Room_ID;
		break;

	}

	for (auto& itr : for_Repush) {
		findResult[mFilter[2]][mFilter[4]][mFilter[5]].push(itr);
	}

	if (Result_ID == 0) return -1;
	return Result_ID;
}

int riseCosts(int mHotelID) {
	int result=0;
	for (int i = 0; i < Room_N[mHotelID -1]; i++) {
		Hotel_ID_Price[mHotelID - 1][i] = Hotel_ID_Price[mHotelID - 1][i] * 1.1;
		result += Hotel_ID_Price[mHotelID - 1][i];
		
	}
	return result;
}