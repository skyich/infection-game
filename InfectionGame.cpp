// InfectionGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>

using std::cout;
using std::cin;

char playground[36]; // игровое поле
int allowTB[36][36]; // таблица разрешеннных ходов
int depth = 2; // максимальное количество оставшихся ходов
int score[2]; // очки(G, B)
int move[2]; // ход(откуда, куда) // всегда допустим


void makeAllowTB() {
	for (int i = 0; i < 36; ++i)
		for(int j = 0; j < 36; ++j)
			if (i == j)
				allowTB[i][j] = 0;
			else {
				int x1 = i / 6;
				int x2 = j / 6;
				int y1 = i % 6;
				int y2 = j % 6;
				if (abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1)
					allowTB[i][j] = 1;
				else if (abs(x2 - x1) <= 2 && abs(y2 - y1) <= 2)
					allowTB[i][j] = 2;
				else
					allowTB[i][j] = 0;
				
				//adjacencyTB[i][j] = abs(i - j) / 6 <= 1 && abs(i - j) % 6 <= 1 ? 1 : 0;
			}
}


// печать игрового поля
void printPlayground() {
	cout << "   ";
	for (int i = 0; i < 6; ++i)
		cout << i << ' ';

	cout << '\n';
	
	for (int i = 0; i < 6; ++i) {
		cout << i << "  ";
		for (int j = 0; j < 6; ++j) {
			char curChar;
			if (playground[i * 6 + j] == 0)
				curChar = 'G';
			else if (playground[i * 6 + j] == 1)
				curChar = 'B';
			else
				curChar = '*';
				cout << curChar << ' ';
		}
		cout << '\n';
	}
	cout << '\n';
}

void printMove(char player) {
	printPlayground();
}

void printCurrentPlayer(char player) {
	char pl = player == 0 ? 'G' : 'B';
	cout << pl << " move:" << '\n';
}


// сделать ход из parentPos в newPos
void makeMove(char player) {
	char enemy = player == 1 ? 0 : 1;
	if (allowTB[move[0]][move[1]] == 1) {
		playground[move[1]] = player;
		score[player] += 1;
	}
	else {
		playground[move[0]] = '*';
		playground[move[1]] = player;
	}
	
	for (int i = 0; i < 36; ++i)
		if (playground[i] == enemy && allowTB[move[1]][i] == 1) {
			playground[i] = player;
			score[player] += 1;
			score[enemy] -= 1;
		}
}

void inputMove(char player) {
	int i, j;
	cout << "Input from: ";
	cin >> i >> j;
	int t1 = i * 6 + j;
	while (playground[t1] != player) {
		cout << "Input from: ";
		cin >> i >> j;
		t1 = i * 6 + j;
	}
	
	move[0] = i * 6 + j;
	cin >> i >> j;
	move[1] = i * 6 + j;
}


int value(char* pg, char player) {
	int res = 0;
	for (int i = 0; i < 36; ++i)
		if (pg[i] == player)
			++res;
	return res;
}

void temp_move(char* pg, char player, char enemy, int x, int y) {
	if (allowTB[x][y] == 1) {
		pg[y] = player;
	}
	else {
		pg[x] = '*';
		pg[y] = player;
	}
	for (int i = 0; i < 36; ++i)
		if (pg[i] == enemy && allowTB[y][i] == 1)
			pg[i] = player;
}


int minimax(char* temp_pg, char player, int depth, bool isMaximizingPlayer, int alpha, int beta) {
	if (depth == 0)
		return value(temp_pg, player);

	char save[36];
	std::copy(temp_pg, temp_pg + 36, save);
	char enemy = player == 1 ? 0 : 1;
	if (isMaximizingPlayer) {
		int bestVal = INT_MIN;
		for (int i = 0; i < 36; ++i) {
			if (temp_pg[i] == player) {
				for (int j = 0; j < 36; ++j) {
					if (i != j && temp_pg[j] == '*' && allowTB[i][j] != 0) {
						temp_move(temp_pg, player, enemy, i, j);
						int value = minimax(temp_pg, enemy, depth - 1, false, alpha, beta);
						std::copy(save, save + 36, temp_pg);
						if (value > bestVal) {
							bestVal = value;
							if (depth == 6) {
								move[0] = i;
								move[1] = j;
							}
						}
						if (bestVal > alpha)
							alpha = bestVal;
						if (beta <= alpha)
							break;
						
					}
				}
			}
		}
		return bestVal;
	}
	else {
		int bestVal = INT_MAX;
		for (int i = 0; i < 36; ++i) {
			if (temp_pg[i] == player) {
				for (int j = 0; j < 36; ++j) {
					if (i != j && temp_pg[j] == '*' && allowTB[i][j] != 0) {
						temp_move(temp_pg, player, enemy, i, j);
						int value = minimax(temp_pg, enemy, depth - 1, true, alpha, beta);
						std::copy(save, save + 36, temp_pg);
						if (value < bestVal) {
							bestVal = value;
						}
						if (bestVal < beta)
							beta = bestVal;
						if (beta <= alpha)
							break;
						
					}
				}
			}
		}
		return bestVal;
	}
}


void AIMove(char player) {
	clock_t begin = clock();
	char temp[36];
	std::copy(playground, playground + 36, temp);
	int t1 = move[0];
	int t2 = move[1];
	minimax(temp, player, 6, true, INT_MIN, INT_MAX);
	if (move[0] == t1 && move[1] == t2) {
		move[0] = 0;
		move[1] = 0;
	}
	
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	
	char pl = player == 0 ? 'G' : 'B';
	std::cout << pl << " AI\ni = "<< move[0] / 6 <<", j = " << move[0] % 6 << '\n';
	std::cout << "i = "<< move[1] / 6 << ", j = " << move[1] % 6 << '\n';
	std::cout << "Time: " << elapsed_secs << '\n';
}

bool isEnd() {
	if (score[0] == 0 || score[1] == 0)
		return true;
	int res_score = score[0] + score[1];
	if (res_score >= 36)
		return true;
	if (move[0] == 0 && move[1] == 0)
		return true;
	return false;
}


void startGame(char player, bool firstMove) {
	for (int i = 0; i < 36; ++i) {
		playground[i] = '*';
	}

	char player1, player2;
	if (player == 'G') {
		player1 = 0;
		player2 = 1;
	}
	else {
		player1 = 1;
		player2 = 0;
	}
	
	// начальная расстановка
	playground[0] = 0;
	playground[5] = 1;
	playground[30] = 1;
	playground[35] = 0;
	printPlayground();

	score[player1] = 2;
	score[player2] = 2;

	if (firstMove) {
		printCurrentPlayer(player1);
		inputMove();
		makeMove(player1);
		printMove(player1);
	}
	
	while (!isEnd()) {
		printCurrentPlayer(player2);
		AIMove(player2);
		makeMove(player2);
		printMove(player2);
		if (isEnd())
			break;
		printCurrentPlayer(player1);
		inputMove();
		makeMove(player1);
		printMove(player1);
	}

	char player2_ch = player == 'G' ? 'B' : 'G';
	if (score[player1] > score[player2])
		cout << player << " win. Score " << score[player1] << " : " << score[player2] << '\n';
	else
		cout << player2_ch << " win. Score " << score[player2] << " : " << score[player1] << '\n';
}

int main()
{
	makeAllowTB();
	
	/*
	for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < 36; ++j)
			cout << allowTB[i][j] << ' ';
		cout << '\n';
	}*/

	startGame('G', true);
}