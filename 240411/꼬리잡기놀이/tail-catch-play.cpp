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

	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (nx < 1 || ny < 1 || nx > N || ny > N || Map[nx][ny] == 0 || Visit[nx][ny] == 1) continue;

		//꼬리(3) 다음은 반드시 중간(2)이다.
		if (curr == 3 && Map[nx][ny] != 2) continue;
        Visit[nx][ny] = 1;
		Group[Label][curr_idx].nx = nx;
		Group[Label][curr_idx].ny = ny;
		Group[Label].push_back({ nx, ny, 0, 0, Map[nx][ny] });
		Spread(nx, ny, curr_idx + 1, Label);
	}
}

void Make_Group() {
	int Label = 1;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			if (Map[i][j] == 3) {

				Visit[i][j] = 1;
				Group[Label].push_back({ i, j, 0, 0, 3 });

				Spread(i, j, 0, Label);
				Group[Label][Group[Label].size() - 1].nx = Group[Label][0].x;
				Group[Label][Group[Label].size() - 1].ny = Group[Label][0].y;
				//순환 구조 만들기

				for (int k = 0; k < Group[Label].size(); k++) {
					Pos[Group[Label][k].x][Group[Label][k].y] = Label;
				}

				Label++;
			}
		}
	}
	memset(Visit, 0, sizeof(Visit));
}

//머리 방향으로 한 칸 움직인다.
void Move_One_Step() {
	int Temp[21][21] = { 0 };
	for (int i = 1; i <= M; i++) {

		int Last_x = Group[i][0].x;
		int Last_y = Group[i][0].y;
		int Last_nx = Group[i][0].nx;
		int Last_ny = Group[i][0].ny;

		for (int j = 0; j < Group[i].size() - 1; j++) {
			int nx = Group[i][j].nx;
			int ny = Group[i][j].ny;
			Temp[nx][ny] = Group[i][j].role;
			//Temp 이동 정보 저장

			Group[i][j].x = nx;
			Group[i][j].y = ny;
			Group[i][j].nx = Group[i][j + 1].nx;
			Group[i][j].ny = Group[i][j + 1].ny;
		}

		int size = Group[i].size();
		Temp[Group[i][size - 1].nx][Group[i][size - 1].ny] = Group[i][size - 1].role;
		Group[i][size - 1].x = Last_x;
		Group[i][size - 1].y = Last_y;
		Group[i][size - 1].nx = Last_nx;
		Group[i][size - 1].ny = Last_ny;
	}


	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			Map[i][j] = Temp[i][j];
		}
	}

}

void Reverse_Path(int x, int y, int n) {
	//점수 얻기 로직 시작
	if (Map[x][y] == 1) {
		Point += 1;
	}
	else {
		int head = -1;
		int target = -1;
		for (int i = 0; i < Group[n].size(); i++) {
			if (Group[n][i].role == 1) {
				head = i;
			}
			else if (Group[n][i].x == x && Group[n][i].y == y) {
				target = i;
			}
		}
		int p = abs(target - head) + 1;
		Point += p * p;
	}
	//점수 얻기 로직 끝

	int Tail_x = Group[n][0].x;
	int Tail_y = Group[n][0].y;

	for (int i = 0; i < Group[n].size(); i++) {
		int px = Group[n][i].x;
		int py = Group[n][i].y;
		if (Map[px][py] == 1) {
			Map[px][py] = 3;
			Map[Tail_x][Tail_y] = 1;
			Tail_x = px;
			Tail_y = py;
			break;
		}
	}
	Group[n].clear();
	//n번그룹 정보 완전 초기화

	Visit[Tail_x][Tail_y] = 1;
	Group[n].push_back({ Tail_x, Tail_y, 0, 0, 3 });

	Spread(Tail_x, Tail_y, 0, n);
	Group[n][Group[n].size() - 1].nx = Group[n][0].x;
	Group[n][Group[n].size() - 1].ny = Group[n][0].y;
	memset(Visit, 0, sizeof(Visit));
	//방문배열 초기화
}

void Left_ball(int time) {
	for (int i = 1; i <= N; i++) {
		if (Map[time][i] >= 1 && Map[time][i] <= 3) {
			Reverse_Path(time, i, Pos[time][i]);
			break;
		}
	}
}

void Upper_ball(int time) {
	for (int i = N; i >= 1; i--) {
		if (Map[i][time] >= 1 && Map[i][time] <= 3) {
			Reverse_Path(i, time, Pos[i][time]);
			break;
		}
	}
}

void Right_ball(int time) {
	for (int i = N; i >= 1; i--) {
		if (Map[time][i] >= 1 && Map[time][i] <= 3) {
			Reverse_Path(time, i, Pos[time][i]);
			break;
		}
	}
}

void High_ball(int time) {
	for (int i = 1; i <= N; i++) {
		if (Map[i][time] >= 1 && Map[i][time] <= 3) {
			Reverse_Path(i, time, Pos[i][time]);
			break;
		}
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
			Right_ball(N - (round - 2 * N) + 1);
		}
		else if (round <= 4 * N) {
			High_ball(N - (round - 3 * N) + 1);
		}
	}
	cout << Point;
}