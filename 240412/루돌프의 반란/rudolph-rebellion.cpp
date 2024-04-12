#include <iostream>
#include <algorithm>
using namespace std;

int N, M, P, C, D;
struct Ludol {
	int x;
	int y;
	int dir;
};
Ludol Lu;
//루돌프 관리

struct Data {
	int id;
	int x;
	int y;
	int stun;
};
Data Santa[31];
int Dead[31];
int Point[31];
int Pos[51][51];
//산타 관리

int cx[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
int cy[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
//루돌프의 8방향

int dx[4] = { -1, 0, 1, 0 };
int dy[4] = { 0, 1, 0, -1 };
//상|우|하|좌

void Input() {
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);
	cin >> N >> M >> P >> C >> D;
	cin >> Lu.x >> Lu.y;
	for (int i = 1; i <= P; i++) {
		int id, x, y;
		cin >> id >> x >> y;
		Santa[id].id = id;
		Santa[id].x = x;
		Santa[id].y = y;
		Pos[x][y] = id;
	}
}

//모든 산타가 아웃되면 TRUE
bool Game_End() {
	for (int i = 1; i <= P; i++) {
		if (Dead[i] == 0) return false;
	} return true;
}

int Make_Dist(int x1, int y1, int x2, int y2) {
	int dist_x = abs(x1 - x2);
	int dist_y = abs(y1 - y2);
	return (dist_x * dist_x) + (dist_y * dist_y);
}

void Crash_Back(int x, int y, int dir, int n, int status) {
	//status = 0 : 루돌프의 움직임으로 충돌 발생
	//status = 1 : 산타의 움직임으로 충돌 발생
	if (x < 1 || y < 1 || x > N || y > N) {
		Dead[n] = 1;
		Santa[n].stun = 0;
		Santa[n].x = 0;
		Santa[n].y = 0;
		return;
	}

	if (Pos[x][y] == 0) {
		Pos[x][y] = n;
		Santa[n].x = x;
		Santa[n].y = y;
		//Pos & Santa 동기화
		return;
	}

	if (Pos[x][y] != 0) {
		int nx = x;
		int ny = y;
		if (status == 0) {
			//루돌프의 움직임으로 충돌이 발생
			nx += cx[dir];
			ny += cy[dir];
		}
		else if (status == 1) {
			//산타의 움직임으로 충돌이 발생
			nx += dx[dir];
			ny += dy[dir];
		}
		Crash_Back(nx, ny, dir, Pos[x][y], status);

		Pos[x][y] = n;
		Santa[n].x = x;
		Santa[n].y = y;
		//Pos & Santa 동기화
	}
}

void Move_Ludol() {

	int Best_Dist = 9999, Best_X = -1, Best_Y = -1, Best_id = -1;
	for (int i = 1; i <= P; i++) {
		if (Dead[i] == 1) {
			continue;
		}
		pair<int, pair<int, int>> Best = { Best_Dist, {-Best_X, -Best_Y} };
		pair<int, pair<int, int>> Curr = { Make_Dist(Santa[i].x, Santa[i].y, Lu.x, Lu.y), {-Santa[i].x, -Santa[i].y} };
		if (Curr < Best) {
			Best_Dist = Make_Dist(Santa[i].x, Santa[i].y, Lu.x, Lu.y);
			Best_X = Santa[i].x;
			Best_Y = Santa[i].y;
			Best_id = i;
		}
	}

	int min_dist = 1e9, Gx = -1, Gy = -1, Gdir = -1;
	for (int i = 0; i < 8; i++) {
		int nx = Lu.x + cx[i];
		int ny = Lu.y + cy[i];
		int curr_dist = Make_Dist(nx, ny, Best_X, Best_Y);
		if (curr_dist < min_dist) {
			min_dist = curr_dist;
			Gx = nx;
			Gy = ny;
			Gdir = i;
		}
	}
	Lu.x = Gx;
	Lu.y = Gy;
	Lu.dir = Gdir;
	//이동 확정

	//루돌프의 이동으로 인해 충돌이 발생함
	if (Pos[Lu.x][Lu.y] != 0) {
		int Santa_Id = Pos[Lu.x][Lu.y];

		Pos[Lu.x][Lu.y] = 0;
		Point[Santa_Id] += C;
		Santa[Santa_Id].stun = 2;
		//Pos상 제거 & 점수 획득 & 기절

		int Santa_X = Lu.x + (C * cx[Lu.dir]);
		int Santa_Y = Lu.y + (C * cy[Lu.dir]);
		Crash_Back(Santa_X, Santa_Y, Lu.dir, Santa_Id, 0);
	}
}

void Move_Santa() {
	for (int i = 1; i <= P; i++) {
		if (Dead[i] == 1 || Santa[i].stun > 0) {
			continue;
		}

		int min_dist = Make_Dist(Santa[i].x, Santa[i].y, Lu.x, Lu.y);
		int Gx = -1, Gy = -1, Gdir = -1;
		for (int d = 0; d < 4; d++) {
			int nx = Santa[i].x + dx[d];
			int ny = Santa[i].y + dy[d];
			if (nx < 1 || ny < 1 || nx > N || ny > N || Pos[nx][ny] != 0) {
				continue;
			}

			int curr_dist = Make_Dist(nx, ny, Lu.x, Lu.y);
			if (curr_dist < min_dist) {
				min_dist = curr_dist;
				Gx = nx;
				Gy = ny;
				Gdir = d;
			}
		}
		//산타의 이동로직 종료

		if (!(Gx == -1 && Gy == -1)) {
			Pos[Santa[i].x][Santa[i].y] = 0;
			Santa[i].x = Gx;
			Santa[i].y = Gy;
			Pos[Gx][Gy] = i;
			//이동 확정 & Pos 동기화
		}

		//산타의 이동으로 루돌프랑 충돌이 발생
		if (Gx == Lu.x && Gy == Lu.y) {
			Gdir = (Gdir + 2) % 4;
			Pos[Gx][Gy] = 0;
			Santa[i].stun = 2;
			Point[i] += D;
			//Pos상 제거 & 점수 획득 & 기절

			int nx = Lu.x + (D * dx[Gdir]);
			int ny = Lu.y + (D * dy[Gdir]);
			Crash_Back(nx, ny, Gdir, i, 1);
		}
	}
}

int main() {
	Input();
	for (int t = 1; t <= M; t++) {

		Move_Ludol();
		//if (Game_End() == true) break;
		Move_Santa();
		if (Game_End() == true) break;

		for (int i = 1; i <= P; i++) {
			if (Santa[i].stun > 0) {
				Santa[i].stun--;
			}
			if (Dead[i] == 0) {
				Point[i]++;
			}
		}

	}
	for (int i = 1; i <= P; i++) {
		cout << Point[i] << ' ';
	}
}