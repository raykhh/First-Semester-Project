#include <windows.h>
#include <conio.h>
#include <ctime>
#include <iostream>
#include <mmsystem.h>
#include<string>
#include <cstdlib>
#pragma comment(lib, "winmm.lib")

using namespace std;


const int MAP_HEIGHT = 40;
const int MAP_WIDTH = 30;
const int maxBullets = 50;
const int EnemyBullets = 10;
bool level = false;
int movestarThreshold = 20;
int asteroidRate = 10;
int enemyBulletThreshold = 20;
int crateThresh = 30;
void setColor(int color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void cratesgenerate(char arr[][MAP_WIDTH]) {
	srand(time(NULL));
	int i, j;
	i = rand() % 3 + 2;
	j = rand() % 28 + 2;
	arr[i][j] = '$';
}
void movcrates(char arr[][MAP_WIDTH]) {
	// Move crates down by 1 row
	for (int x = MAP_HEIGHT - 2; x >= 2; x--) {
		for (int y = 2; y < MAP_WIDTH - 1; y++) {
			if (arr[x][y] == '$') {
				// Clear the current position of the crate
				arr[x][y] = ' ';

				// Move the crate one row down and overwrite whatever is in the new position
				if (x + 1 < MAP_HEIGHT - 1) {
					arr[x + 1][y] = '$';
				}
			}
		}
	}
}
void enemyBulletMovement(int enemyBullets[EnemyBullets][3], char arr[][MAP_WIDTH], int& lives, int a, int b, int c, int d) {
	for (int i = 0; i < EnemyBullets; i++) {
		if (enemyBullets[i][2] == 1) { // Active bullet
			int row = enemyBullets[i][0] + 1;
			int col = enemyBullets[i][1];

			// Clear the old position
			arr[enemyBullets[i][0]][enemyBullets[i][1]] = ' ';

			if (row < 39) {
				if (row == a && col == b || row == a && col == c || row == a && col == d) { // Collision with player
					lives--;
					enemyBullets[i][2] = 0; // Deactivate bullet
				}
				else if (arr[row][col] == '|') {
					enemyBullets[i][2] = 0;
					arr[row][col] = 0;
				}
				else {
					enemyBullets[i][0] = row; // Move bullet down
					arr[row][col] = '|';
				}
			}
			else {
				enemyBullets[i][2] = 0; // Deactivate if out of bounds
			}
		}
	}
}


void enemyship(int& x, int& y, bool& enemymov) {

	if (y < MAP_WIDTH - 2 && enemymov) {
		y++;
	}
	else {
		enemymov = false;
		if (y > 2 && enemymov == false) {
			y--;
		}
		else
			enemymov = true;
	}
}
void asteriods(char arr[][MAP_WIDTH]) {
	srand(time(NULL));
	int i, j;
	i = rand() % 20 + 2;
	j = rand() % 28 + 2;
	arr[i][j] = '#';
}

void movasteroids(char arr[][MAP_WIDTH]) {
	for (int x = 38; x >= 2; x--) {
		for (int y = 2; y < 28; y++) {
			if (arr[x][y] == '#') {
				arr[x][y] = ' ';
				if (x + 1 < 38) {
					arr[x + 1][y] = '#';
				}
			}
		}
	}
}

void stars(char arr[][MAP_WIDTH]) {
	int i, j;
	i = rand() % 20 + 2;
	j = rand() % (MAP_WIDTH - 2) + 2;
	arr[i][j] = '*';
}

void movastars(char arr[][MAP_WIDTH], int& scorestar, int a, int b, int c, int d) {
	for (int row = MAP_HEIGHT - 1; row >= 2; row--) {
		for (int col = 2; col < MAP_WIDTH - 2; col++) {
			if (arr[row][col] == '*') {
				if (row + 1 == a && col == b || row + 1 == a && col == c || row + 1 == a && col == d) {
					scorestar++;
					arr[row][col] = ' ';
					return;
				}
				arr[row][col] = ' ';

				if (row + 1 < 38) {
					arr[row + 1][col] = '*';
				}
			}
		}
	}
}

void bulletsmov(int bullets[maxBullets][3], char arr[][MAP_WIDTH], int& enemyhealth, bool& enemydestroy, int& enemyshipcount) {
	for (int i = 0; i < maxBullets; i++) {
		if (bullets[i][2] == 1) {
			int row = bullets[i][0] - 1;
			int col = bullets[i][1];
			//if (arr[bullets[i][0]][bullets[i][1]] == '|') {
			arr[bullets[i][0]][bullets[i][1]] = ' ';
			//}
			if (row > 0) {
				if (arr[row][col] == '#') {
					bullets[i][2] = 0;
					arr[row][col] = ' ';
				}
				else if (arr[row][col] == '*') {
					bullets[i][0]--;
					arr[bullets[i][0]][bullets[i][1]] = '|';
					arr[row + 2][col] = '*';
				}
				else  if (arr[row][col] == '$') {
					bullets[i][0]--;
					arr[bullets[i][0]][bullets[i][1]] = '|';
					arr[row + 2][col] = '$';
				}
				else if (arr[row][col] == 'E') {
					bullets[i][2] = 0; // Deactivate bullet
					enemyhealth--; // Decrease enemy health
					if (enemyhealth <= 0) { // Destroy enemy if health is zero
						arr[row][col] = ' ';
						enemydestroy = true;
						enemyshipcount++;
						enemyhealth = 3;
					}
				}
				else {
					bullets[i][0]--;
					arr[bullets[i][0]][bullets[i][1]] = '|';
				}
			}
			else {
				bullets[i][2] = 0;
			}
		}
	}
}


void movement(int& a, int& b, int& c, int& d, char ch) {
	if ((ch == 'w' || ch == 'W' || ch == 72) && a > 28) {
		a--;
	}
	else if ((ch == 'J' || ch == 'j') && a > 28) {
		a = a - 2;
	}
	else if ((ch == 'a' || ch == 'A' || ch == 75) && c > 1) {
		c--;
		d--;
		b--;
	}
	else if ((ch == 'd' || ch == 'D' || ch == 77) && d < 28) {
		c++;
		d++;
		b++;
	}
	else if ((ch == 's' || ch == 'S' || ch == 80) && a < 38) {
		a++;
	}
}

void map(char arr[][MAP_WIDTH], int& scorestar, int& lives, int& enemyshipcount, int& distance) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0 });
	setColor(4);
	cout << "Distance : " << distance << "m ";
	cout << "EnemyShip Destroy : " << enemyshipcount << ' ';
	cout << "Score : " << scorestar;
	cout << " Lives : " << lives << endl;
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (arr[i][j] == '^') setColor(10); // Bright Green for player ship body
			else if (arr[i][j] == '=') setColor(11); // Bright Blue for player ship wings
			else if (arr[i][j] == '|') setColor(14); // Bright Yellow for bullets
			else if (arr[i][j] == '$') setColor(11); // Cyan for crates
			else if (arr[i][j] == '*') setColor(15); // Bright White for stars
			else if (arr[i][j] == '#') setColor(8); // Dark Gray for asteroids
			else if (arr[i][j] == 'E') setColor(12); // Bright Red for enemy ships
			else setColor(1); // Dark Blue for borders
			cout << arr[i][j] << ' ';
		}
		cout << endl;
	}
	if (distance >= 3000 || enemyshipcount >= 5 && !level) {
		system("cls");
		cout << "Level 1 completed \n";
		cout << "Get Ready For Level 2\n";
		Sleep(2000);
		level = true;
		distance = 0;
		lives = 3;
		enemyshipcount = 0;
		movestarThreshold = 20;
		asteroidRate = 5;
		enemyBulletThreshold = 5;
		crateThresh = 20;

	}
	if (level) {
		if (distance >= 5000 || enemyshipcount >= 10) {
			cout << "You Win Game Over \n";
			system("pause");
			exit(0);
		}
	}
	else if (lives <= 0) {
		system("cls");
		cout << "\t\t\t\tGame Over! " << endl;
		cout << "\t\t\t\tMoye Moye" << endl;
		system("pause");
		exit(0);
	}
}
void Collision(int& a, int& b, char arr[][MAP_WIDTH], int& lives, int c, int d) {
	if (arr[a][b] == '#') {
		lives--;
		arr[a][b] = ' ';
	}
	else if (arr[a][c] == '#') {
		lives--;
		arr[a][c] = ' ';
	}
	else if (arr[a][d] == '#') {
		lives--;
		arr[a][d] = ' ';
	}
	else if (arr[a][b] == '$') {
		lives++;
		arr[a][b] = ' ';
	}
	else if (arr[a][c] == '$') {
		lives++;
		arr[a][c] = ' ';
	}
	else if (arr[a][d] == '$') {
		lives++;
		arr[a][d] = ' ';
	}

}
void newgame() {
	char arr[MAP_HEIGHT][MAP_WIDTH];
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (i == 0 || i == MAP_HEIGHT - 1 || j == 0 || j == MAP_WIDTH - 1) {
				arr[i][j] = '#';
			}

			else {
				arr[i][j] = ' ';
			}
		}
	}
	int distance = 0;
	time_t lastupdate = time(0);
	int a = 28, b = 15, c = 14, d = 16;
	int x = 10, y = 15, scorestar = 0, lives = 3;
	bool enemydestroy = true;
	int enemyhealth = 3, enemyshipcount = 0;
	bool enemymov = true, bulletmov = false;
	int moveAsteroidCounter = 0;
	int moveAsteroidThreshold = 5;
	char ch;
	int bullets[maxBullets][3];
	int movestarCounter = 0;

	int asteroidspawn = 0;

	int enemyBullets[EnemyBullets][3];
	int enemyBulletCounter = 0;

	int cratemov = 0;

	for (int i = 0; i < EnemyBullets; i++) {
		enemyBullets[i][2] = 0;
	}
	for (int i = 0; i < maxBullets; i++) {
		bullets[i][2] = 0;
	}

	while (1) {
		if (_kbhit()) {
			ch = _getch();
			arr[a][b] = ' ';
			arr[a][c] = ' ';
			arr[a][d] = ' ';

			movement(a, b, c, d, ch);

			arr[a][b] = '^';
			arr[a][c] = '=';
			arr[a][d] = '=';
			if (ch == ' ') {
				for (int i = 0; i < maxBullets; i++) {
					if (bullets[i][2] == 0) {
						bullets[i][0] = a - 1;
						bullets[i][1] = b;
						bullets[i][2] = 1;
						arr[bullets[i][0]][bullets[i][1]] = '|';
						bulletmov = true;
						break;
					}
				}
			}
			else if (ch == 'B' || ch == 'b') {
				for (int i = 0; i < maxBullets; i++) {
					if (bullets[i][2] == 0) {
						bullets[i][0] = a - 1;
						bullets[i][1] = b;
						bullets[i][2] = 1;
						arr[bullets[i][0]][bullets[i][1]] = '|';
						break;
					}
				}
			}
		}
		time_t currentime = time(0);
		if (currentime - lastupdate >= 1) {
			distance += 16;
			lastupdate = currentime;
		}
		if (cratemov >= crateThresh) {
			cratesgenerate(arr);
			cratemov = 0;
		}
		else {
			cratemov++;
		}
		movcrates(arr);
		if (enemyBulletCounter >= enemyBulletThreshold) {
			for (int i = 0; i < EnemyBullets; i++) {
				if (enemyBullets[i][2] == 0) {
					enemyBullets[i][0] = x + 1;
					enemyBullets[i][1] = y;
					enemyBullets[i][2] = 1;
					arr[x + 1][y] = '|';
					break;
				}
			}
			arr[x][y] = ' ';
			if (enemydestroy) {
				enemyship(x, y, enemymov);
				arr[x][y] = 'E';
			}
			enemyBulletCounter = 0;
		}
		else {
			enemyBulletCounter++;
			if (enemydestroy) {
				enemyBulletMovement(enemyBullets, arr, lives, a, b, c, d);
			}
		}



		static int bulletMoveCounter = 0;
		const int bulletMoveThreshold = 1;
		if (bulletmov) {
			if (bulletMoveCounter >= bulletMoveThreshold) {
				bulletsmov(bullets, arr, enemyhealth, enemydestroy, enemyshipcount);
				bulletMoveCounter = 0;
			}
			else {
				bulletMoveCounter++;
				bulletsmov(bullets, arr, enemyhealth, enemydestroy, enemyshipcount);
			}
		}
		/* bulletsmov(bullets, arr);*/
		if (movestarCounter >= movestarThreshold) {
			stars(arr);
			movestarCounter = 0;
		}
		else {
			movestarCounter++;
		}
		if (asteroidspawn >= asteroidRate) {
			asteriods(arr);
			asteroidspawn = 0;
		}
		else {
			asteroidspawn++;
		}

		if (moveAsteroidCounter >= moveAsteroidThreshold) {
			movastars(arr, scorestar, a, b, c, d);
			movasteroids(arr);
			moveAsteroidCounter = 0;
		}
		else {
			moveAsteroidCounter++;
		}
		Collision(a, b, arr, lives, c, d);
		map(arr, scorestar, lives, enemyshipcount, distance);
	}

}

int main() {
	cout << "\t\t\t\t+++++++++++++++" << " ALIEN WARE GAME " << "+++++++++++++++\n";
	cout << "\t\t\t\t1.Start Game If You Have Stamina To Survive\n";
	int choice;
	cin >> choice;
	switch (choice) {
	case 1:
		system("cls");
		newgame();
	}


	return 0;
}