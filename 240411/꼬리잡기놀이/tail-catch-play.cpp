#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

int N, M, K;
struct Data {
	int x;
	int y;
	int nx;
	int ny;
	int role;
};
vector<Data> Group[6];
int Point = 0;
int Map[21][21], Visit[21][21];
int Pos[21][21];
int dx[4] = { 1, -1, 0, 0 };
int dy[4] = { 0, 0, 1, -1 };


void Fast_IO() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
}

// void Print_Map() {
// 	cout << "MAP : " << endl;
// 	for (int i = 1; i <= N; i++) {
// 		for (int j = 1; j <= N; j++) {
// 			cout << Map[i][j] << '\t';
// 		} cout << endl;
// 	} cout << endl;
// }

// void Print_Pos() {
// 	cout << "Pos : " << endl;
// 	for (int i = 1; i <= N; i++) {
// 		for (int j = 1; j <= N; j++) {
// 			cout << Pos[i][j] << '\t';
// 		} cout << endl;
// 	} cout << endl;
// }


void Input() {
	Fast_IO();
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> Map[i][j];
		}
	}
}

void Spread(int x, int y, int curr_idx, int Label) {
	int curr = Map[x][y];
	//cout << "현재 칸 : " << x << ", " << y << "는 : " << Map[x][y] << " 입니다." << endl;

	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (nx < 1 || ny < 1 || nx > N || ny > N || Map[nx][ny] == 0) {
			//cout << nx << ", " << ny << " 는 레일 범위 밖입니다." << endl;
			continue;
		}
		if (Visit[nx][ny] == 1) {
			//cout << nx << ", " << ny << " 는 방문 노드입니다." << endl;
			continue;
		}
		
		//꼬리->머리->꼬리 방향의 그룹탐색

		//꼬리(3) 다음은 반드시 중간(2)이다.
		if (curr == 3 && Map[nx][ny] == 2) {
			Visit[nx][ny] = 1;
			Group[Label][curr_idx].nx = nx;
			Group[Label][curr_idx].ny = ny;
			//cout << curr_idx << ", " << Group[Label][curr_idx].nx << " , " << Group[Label][curr_idx].ny << endl;
			//현재 기준 nx, ny를 확정
			Group[Label].push_back({ nx, ny, 0, 0, Map[nx][ny] });
			Spread(nx, ny, curr_idx + 1, Label);
		}
		//중간(2) 다음은 반드시 중간(2) or 머리(1)이다.
		else if (curr == 2 && (Map[nx][ny] == 2 || Map[nx][ny] == 1)) {
			Visit[nx][ny] = 1;
			Group[Label][curr_idx].nx = nx;
			Group[Label][curr_idx].ny = ny;
			//cout << curr_idx << ", " << Group[Label][curr_idx].nx << " , " << Group[Label][curr_idx].ny << endl;
			//현재 기준 nx, ny를 확정
			Group[Label].push_back({ nx, ny, 0, 0, Map[nx][ny] });
			Spread(nx, ny, curr_idx + 1, Label);
		}
		//머리(1) 다음은 반드시 레일(4) or 꼬리(3)이다.
		else if (curr == 1 && (Map[nx][ny] == 3 || Map[nx][ny] == 4)) {
			Visit[nx][ny] = 1;
			Group[Label][curr_idx].nx = nx;
			Group[Label][curr_idx].ny = ny;
			//cout << curr_idx << ", " << Group[Label][curr_idx].nx << " , " << Group[Label][curr_idx].ny << endl;
			//현재 기준 nx, ny를 확정
			Group[Label].push_back({ nx, ny, 0, 0, Map[nx][ny] });
			Spread(nx, ny, curr_idx + 1, Label);
		}
		//레일(4) 다음은 반드시 레일(4) or 꼬리(3)이다.
		else if (curr == 4 && (Map[nx][ny] == 4 || Map[nx][ny] == 3)) {
			Visit[nx][ny] = 1;
			Group[Label][curr_idx].nx = nx;
			Group[Label][curr_idx].ny = ny;
			//cout << curr_idx << ", " << Group[Label][curr_idx].nx << " , " << Group[Label][curr_idx].ny << endl;
			//현재 기준 nx, ny를 확정
			Group[Label].push_back({ nx, ny, 0, 0, Map[nx][ny] });
			Spread(nx, ny, curr_idx + 1, Label);
		}
	}
}

void Make_Group() {
	int Label = 1;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (Map[i][j] == 3) {
				//cout << "꼬리칸 : " << i << ", " << j << "에서 그룹 생성 전파를 시작합니다." << endl;
				Visit[i][j] = 1;
				Group[Label].push_back({ i, j, 0, 0, 3 });

				Spread(i, j, 0, Label);
				Group[Label][Group[Label].size() - 1].nx = Group[Label][0].x;
				Group[Label][Group[Label].size() - 1].ny = Group[Label][0].y;
				//순환 구조 만들기
				
				//cout << Label << " 번 그룹의 크기 : " << Group[Label].size() << endl << endl;
				for (int k = 0; k < Group[Label].size(); k++) {
					Pos[Group[Label][k].x][Group[Label][k].y] = Label;
					//cout << Group[Label][k].nx << ", " << Group[Label][k].ny << endl;
				}

				Label++;
			}
		}
	}
	memset(Visit, 0, sizeof(Visit));
	//Print_Map();
	//cout << endl;
	//Print_Pos();
}

//머리 방향으로 한 칸 움직인다.
void Move_One_Step() {
	int Temp[21][21] = { 0 };
	for (int i = 1; i <= M; i++) {

		int Last_x = Group[i][0].x;
		int Last_y = Group[i][0].y;
		int Last_nx = Group[i][0].nx;
		int Last_ny = Group[i][0].ny;
		//첫 정보 저장

		for (int j = 0; j < Group[i].size() - 1; j++) {
			int nx = Group[i][j].nx;
			int ny = Group[i][j].ny;
			Temp[nx][ny] = Group[i][j].role;
			//Temp에 다음 좌표 대입

			Group[i][j].x = nx;
			Group[i][j].y = ny;
			Group[i][j].nx = Group[i][j + 1].nx;
			Group[i][j].ny = Group[i][j + 1].ny;
		}

		Temp[Last_x][Last_y] = Group[i][Group[i].size() - 1].role;
		Group[i][Group[i].size() - 1].x = Last_x;
		Group[i][Group[i].size() - 1].x = Last_y;
		Group[i][Group[i].size() - 1].nx = Last_nx;
		Group[i][Group[i].size() - 1].ny = Last_ny;
		//마지막 좌표만 처음에 처리

	}

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			Map[i][j] = Temp[i][j];
		}
	}
	//cout << "한 칸 움직임 이후" << endl;
	//Print_Map();
}

void Reverse_Path(int x, int y, int n) {
	//점수 얻기 로직 시작
	int len = 1;

	//cout << x << ", " << y << " 에서 충돌이 발생했습니다." << endl;
	if (Map[x][y] == 1) {
		//cout << "머리 사람입니다. 1점 획득" << endl;
		Point += 1;
		len = 0;
	}
	else {
		for (int i = 0; i < Group[n].size(); i++) {
			if (Group[n][i].role == 1) {
				break;
			}
			else {
				len++;
			}
		}
		//cout << len << "번째 사람입니다. " << len * len << " 점 획득" << endl;
		Point += len * len;
	}
	//점수 얻기 로직 끝

	int New_Tail_x = -1;
	int New_Tail_y = -1;

	for (int i = 0; i < Group[n].size(); i++) {
		int px = Group[n][i].x;
		int py = Group[n][i].y;
		if (Map[px][py] == 1) {
			New_Tail_x = px;
			New_Tail_y = py;
			Map[px][py] = 3;
		}
		else if (Map[px][py] == 3) Map[px][py] = 1;
		//머리, 꼬리 위치 Map 상에서 바꾸기
	}
	Group[n].clear();
	//n번그룹 정보 완전 초기화

	//cout << "꼬리칸 : " << New_Tail_x << ", " << New_Tail_y << "에서 역방향 그룹 생성 전파를 시작합니다." << endl;
	Visit[New_Tail_x][New_Tail_y] = 1;
	Group[n].push_back({ New_Tail_x, New_Tail_y, 0, 0, 3 });

	Spread(New_Tail_x, New_Tail_y, 0, n);
	Group[n][Group[n].size() - 1].nx = Group[n][0].x;
	Group[n][Group[n].size() - 1].ny = Group[n][0].y;
	memset(Visit, 0, sizeof(Visit));
	//방문배열 초기화
	//Print_Map();
}

void Left_ball(int time) {
	//cout << "왼쪽에서 공이 날아옵니다. : " << time << endl;
	bool Flag = false;
	
	for (int i = 1; i <= N; i++) {
		if (Map[time][i] >= 1 && Map[time][i] <= 3) {
			Flag = true;
			Reverse_Path(time, i, Pos[time][i]);
			break;
		}
	}

	if (Flag == true) {
		//cout << "방향이 전환됩니다." << endl;
	}
	else {
		//cout << "아무도 맞지 않았습니다." << endl;
	}
}

void Upper_ball(int time) {
	//cout << "아래에서 공이 날아옵니다. : " << time << endl;
	bool Flag = false;

	for (int i = N; i >= 1; i--) {
		if (Map[i][time] >= 1 && Map[i][time] <= 3) {
			Flag = true;
			Reverse_Path(i, time, Pos[i][time]);
			break;
		}
	}

	if (Flag == true) {
		//cout << "방향이 전환됩니다." << endl;
	}
	else {
		//cout << "아무도 맞지 않았습니다." << endl;
	}
}

void Right_ball(int time) {
	//cout << "오른쪽에서 공이 날아옵니다. : " << time << endl;
	bool Flag = false;

	for (int i = N; i >= 1; i--) {
		if (Map[time][i] >= 1 && Map[time][i] <= 3) {
			Flag = true;
			Reverse_Path(time, i, Pos[time][i]);
			break;
		}
	}

	if (Flag == true) {
		//cout << "방향이 전환됩니다." << endl;
	}
	else {
		//cout << "아무도 맞지 않았습니다." << endl;
	}
}

void High_ball(int time) {
	//cout << "위에서 공이 날아옵니다. : " << time << endl;
	bool Flag = false;

	for (int i = 1; i <= N; i++) {
		if (Map[i][time] >= 1 && Map[i][time] <= 3) {
			Flag = true;
			Reverse_Path(i, time, Pos[i][time]);
			break;
		}
	}

	if (Flag == true) {
		//cout << "방향이 전환됩니다." << endl;
	}
	else {
		//cout << "아무도 맞지 않았습니다." << endl;
	}
}

int main() {
	Input();
	Make_Group();
	int round = 0;
	for (int t = 1; t <= K; t++) {
		
		//1. 그룹이 움직입니다.
		Move_One_Step();

		//2. 공이 날아옵니다.
		round++;
		if (round == 4 * N + 1) {
			round = 1;
		}

		if (round <= N) {
			Left_ball(round);
		}
		else if (round <= 2 * N) {
			Upper_ball(round - N);
		}
		else if (round <= 3 * N) {
			Right_ball(round - 2 * N);
		}
		else if (round <= 4 * N) {
			High_ball(round - 3 * N);
		}
	}
	cout << Point;
}