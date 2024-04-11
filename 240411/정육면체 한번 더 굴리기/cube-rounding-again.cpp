#include<iostream>
#include<algorithm>
#include<cstring>
#include<vector>
using namespace std;

int N, M, Map[21][21], Visit[21][21];
int Dice[7] = { 0, 1, 2, 3, 4, 5, 6 };
int dx[4] = { 0, 1, 0, -1 };
int dy[4] = { 1, 0, -1, 0 };
//동|남|서|북

int Gx = 1, Gy = 1, Gdir = 0;
//주사위의 현재 좌표
int Curr_Point, Total_Point;

//방향 반사
int Reverse_Dir(int n) {
	return (n + 2) % 4;
}

//시계방향 90도
int Clock_Dir(int n) {
	return (n + 1) % 4;
}

//반시계방향 90도
int Unclock_Dir(int n) {
	if (n == 0) return 3;
	else return n - 1;
}

void Input() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> Map[i][j];
		}
	}
}


void Dfs(int x, int y, int n) {
	for (int i = 0; i < 4; i++) {
		int nx = x + dx[i];
		int ny = y + dy[i];
		if (nx < 1 || ny < 1 || nx > N || ny > N) continue;
		if (Visit[nx][ny] == 1) continue;
		if (Map[nx][ny] != n) continue;
		Curr_Point += n;
		Visit[nx][ny] = 1;
		//cout << "좌표 : " << nx << ", " << ny << endl;
		Dfs(nx, ny, n);
	}
}


void Rolling_Dice() {

	int d1 = Dice[1], d2 = Dice[2], d3 = Dice[3];
	int d4 = Dice[4], d5 = Dice[5], d6 = Dice[6];

	int nx = Gx + dx[Gdir];
	int ny = Gy + dy[Gdir];

	if (nx < 1 || ny < 1 || nx > N || ny > N) {
		Gdir = Reverse_Dir(Gdir);
		nx = Gx + dx[Gdir];
		ny = Gy + dy[Gdir];
		//cout << "방향 전환" << endl;
	}
	// cout << "이동 좌표 : " << nx << ", " << ny << endl;

	Gx = nx;
	Gy = ny;
	//이동 확정
	
	Visit[Gx][Gy] = 1;
	Curr_Point += Map[Gx][Gy];
	//cout << "좌표 : " << Gx << ", " << Gy << endl;
	Dfs(Gx, Gy, Map[Gx][Gy]);
	//영역 & 점수 구하기

	// cout << "총점 :" << Curr_Point << endl;
	Total_Point += Curr_Point;
	Curr_Point = 0;
	memset(Visit, 0, sizeof(Visit));
	//탐색 초기화


	if (Gdir == 0) {
		Dice[1] = d4;
		Dice[4] = d6;
		Dice[6] = d3;
		Dice[3] = d1;
	}
	if (Gdir == 1) {
		Dice[1] = d5;
		Dice[5] = d6;
		Dice[6] = d2;
		Dice[2] = d1;
	}
	if (Gdir == 2) {
		Dice[1] = d3;
		Dice[3] = d6;
		Dice[6] = d4;
		Dice[4] = d1;
	}
	if (Gdir == 3) {
		Dice[1] = d2;
		Dice[2] = d6;
		Dice[6] = d5;
		Dice[5] = d1;
	}

	// cout << "주사위 아랫면 : " << Dice[6] << endl;
	// cout << "지도 면 : " << Map[Gx][Gy] << endl;

	//지도 숫자보다 주사위 아래가 더 큰 경우
	if (Dice[6] > Map[Gx][Gy]) {
		// cout << "시계 방향 90도 전환" << endl;
		Gdir = Clock_Dir(Gdir);
	}
	//지도 숫자보다 주사위 아래가 더 작은 경우
	else if (Dice[6] < Map[Gx][Gy]) {
		// cout << "반시계 방향 90도 전환" << endl;
		Gdir = Unclock_Dir(Gdir);
	}


}

int main() {
	Input();
	for (int i = 1; i <= M; i++) {
		Rolling_Dice();
	} cout << Total_Point;
}