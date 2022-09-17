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
// ȣ�� �溰 ����///
int Hotel_ID_Price[Max_Hotel][Max_Room]; // ȣ�� ID , �� idx�� ã�ư���
struct hotel {
	int Hotel_ID;
	int Room_idx;
	int cal_idx;
};
unordered_map<int, hotel> Id_Hash;
int Room_N[Max_Hotel]; // ��ѹ� idx �Ҵ��ϱ� ����

// ȣ�� ���Ǻ��� ID �з�/
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
priority_queue <filt, vector<filt>, pq_up> findResult[Max_Region][Max_Rtype][Max_Vtype]; // first: RoomID, second:Bed��
vector<filt> for_Repush;// ��Ǫ�ÿ�

vector<pair<int,int>> Calander[100000];
int cal_idx_g;

//��������������
void init(int N, int mRoomCnt[]) {
	// �ʱ�ȭ �ڵ�/.
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
											//0���� 1ħ��� 2��Ÿ�� 3����Ÿ�� 4����
void addRoom(int mHotelID, int mRoomID, int mRoomInfo[]){
	
	int Room_idx = Room_N[mHotelID - 1]++;

	Id_Hash[mRoomID] = { mHotelID, Room_idx , cal_idx_g++ };

	Hotel_ID_Price[mHotelID - 1][Room_idx] = mRoomInfo[4];
	findResult[mRoomInfo[0]][mRoomInfo[2]][mRoomInfo[3]].push({ mRoomID ,mRoomInfo[4],mRoomInfo[1] });
	
}
/*
* // ��� �ϸ� ������ c��
int findRoom(int mFilter[]) {
	bool can;
	int Hotel_ID, Room_idx, cal_idx;
	int Result_ID = 0;
	
	if (findResult[mFilter[2]][mFilter[4]][mFilter[5]].empty()) return -1;

	for (auto& itr : findResult[mFilter[2]][mFilter[4]][mFilter[5]]) {
		if (itr.second != mFilter[3]) continue;

		can = true;
		cal_idx = Id_Hash[itr.first].cal_idx;

		// itr�� �ؽ�
		for (auto& day : Calander[cal_idx]) {
			if (day.first >= mFilter[1] || day.second <= mFilter[0]) continue;
			else {
				can = false; break; //���� ��ġ�� �� �̹� ������ �Ұ�
			}
				
		}
		// �����ش��ϸ� PQ ����ִ� �κ�//
		if (can == true) {
			Hotel_ID = Id_Hash[itr.first].Hotel_ID;
			Room_idx = Id_Hash[itr.first].Room_idx;
			PQ.push({ Hotel_ID_Price[Hotel_ID - 1][Room_idx],itr.first });
		}
	}

	if (PQ.empty()) return -1;
	//  pq TOP ��¥�� ����
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
		// 1. BED ������ �ȸ´� ���
		if (temp.BedN != mFilter[3]) {
			for_Repush.push_back(temp);
			continue;
		}
		
		Hotel_ID = Id_Hash[temp.Room_ID].Hotel_ID;
		Room_idx = Id_Hash[temp.Room_ID].Room_idx;
		cal_idx= Id_Hash[temp.Room_ID].cal_idx;
		new_Price = Hotel_ID_Price[Hotel_ID - 1][Room_idx];
		// 2. ������ �ֽ� ������ �ƴ� ���
		if (temp.Price != new_Price) {
			//������ �ֽ� ������ �ƴϾ ����. ��� ��� �ٷ� PQ ������Ʈ
			findResult[mFilter[2]][mFilter[4]][mFilter[5]].push({ temp.Room_ID, new_Price, temp.BedN });
			continue;
		}
		
		// 3. �ش� ��¥�� �׹� �̸� ������ �� ���
		vacant = true;
		for (auto& day : Calander[cal_idx]) {
			if (day.first >= mFilter[1] || day.second <= mFilter[0]) continue;
			else {
				vacant = false;
				break; //���� �ϳ��� ��ġ�� �� �̹� ������ �Ұ�
			} 
		}
		if (vacant == false) {
			for_Repush.push_back(temp);
			continue;
		}

		//4. ��� ���ǿ� ���� �� ���, ���� ��, �� Ǫ��,  for�� ����
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