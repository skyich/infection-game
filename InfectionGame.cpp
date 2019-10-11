// InfectionGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <ctime>

using std::cout;
using std::cin;

const int GAME_SIZE = 49;
const int PRINT_SIZE = 7;
char playground[GAME_SIZE]; // игровое поле
int allowTB[GAME_SIZE][GAME_SIZE]; // таблица разрешеннных ходов
int score[2]; // очки(G, B)
int move[2]; // ход(откуда, куда) // всегда допустим
int d;


void makeAllowTB() {
	for (int i = 0; i < GAME_SIZE; ++i)
		for(int j = 0; j < GAME_SIZE; ++j)
			if (i == j)
				allowTB[i][j] = 0;
			else {
				int x1 = i / PRINT_SIZE;
				int x2 = j / PRINT_SIZE;
				int y1 = i % PRINT_SIZE;
				int y2 = j % PRINT_SIZE;
				if (abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1)
					allowTB[i][j] = 1;
				else if (abs(x2 - x1) <= 2 && abs(y2 - y1) <= 2)
					allowTB[i][j] = 2;
				else
					allowTB[i][j] = 0;
			}
}


// печать игрового поля
void printPlayground() {
	cout << "   ";
	for (int i = 0; i < PRINT_SIZE; ++i)
		cout << i + 1 << ' ';

	cout << '\n';
	
	for (int i = 0; i < PRINT_SIZE; ++i) {
		cout << i + 1 << "  ";
		for (int j = 0; j < PRINT_SIZE; ++j) {
			char curChar;
			if (playground[i * PRINT_SIZE + j] == 0)
				curChar = 'G';
			else if (playground[i * PRINT_SIZE + j] == 1)
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
	
	for (int i = 0; i < GAME_SIZE; ++i)
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
	i -= 1;
	j -= 1;
	int t = i * PRINT_SIZE + j;
	while (playground[t] != player) {
		cout << "Input from: ";
		cin >> i >> j;
		i -= 1;
		j -= 1;
		t = i * PRINT_SIZE + j;
	}
	move[0] = t;
	cout << "Input dest: ";
	cin >> i >> j;
	i -= 1;
	j -= 1;
	t = i * PRINT_SIZE + j;
	while (playground[t] != '*') {
		cout << "Input dest: ";
		cin >> i >> j;
		i -= 1;
		j -= 1;
		t = i * PRINT_SIZE + j;
	}
	move[1] = t;
}


int value(char* pg, char player) {
	int res = 0;
	for (int i = 0; i < GAME_SIZE; ++i)
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
	for (int i = 0; i < GAME_SIZE; ++i)
		if (pg[i] == enemy && allowTB[y][i] == 1)
			pg[i] = player;
}


int minimax(char* temp_pg, char player, int depth, bool isMaximizingPlayer, int alpha, int beta) {
	if (depth == 0)
		return value(temp_pg, player);

	char save[GAME_SIZE];
	std::copy(temp_pg, temp_pg + GAME_SIZE, save);
	char enemy = player == 1 ? 0 : 1;
	//char enemy = player ^ 1;
	if (isMaximizingPlayer) {
		int bestVal = INT_MIN;
		for (int i = 0; i < GAME_SIZE; ++i) {
			if (temp_pg[i] == player) {
				for (int j = 0; j < GAME_SIZE; ++j) {
					if (i != j && temp_pg[j] == '*' && allowTB[i][j] != 0) {
						temp_move(temp_pg, player, enemy, i, j);
						int value = minimax(temp_pg, enemy, depth - 1, false, alpha, beta);
						std::copy(save, save + GAME_SIZE, temp_pg);
						if (value > bestVal) {
							bestVal = value;
							if (depth == d) {
								move[0] = i;
								move[1] = j;
							}
						}
						if (bestVal > alpha)
							alpha = bestVal;
					}
				}
			}
			if (beta <= alpha)
				break;
		}
		return bestVal;
	}
	else {
		int bestVal = INT_MAX;
		for (int i = 0; i < GAME_SIZE; ++i) {
			if (temp_pg[i] == player) {
				for (int j = 0; j < GAME_SIZE; ++j) {
					if (i != j && temp_pg[j] == '*' && allowTB[i][j] != 0) {
						temp_move(temp_pg, player, enemy, i, j);
						int value = minimax(temp_pg, enemy, depth - 1, true, alpha, beta);
						std::copy(save, save + GAME_SIZE, temp_pg);
						if (value < bestVal) {
							bestVal = value;
						}
						if (bestVal < beta)
							beta = bestVal;
					}
				}
			}
			if (beta <= alpha)
				break;
		}
		return bestVal;
	}
}


void AIMove(char player, int depth) {
	clock_t begin = clock();
	char temp[GAME_SIZE];
	std::copy(playground, playground + GAME_SIZE, temp);
	int t1 = move[0];
	int t2 = move[1];
	d = depth;
	minimax(temp, player, depth, true, INT_MIN, INT_MAX);
	if (move[0] == t1 && move[1] == t2) {
		move[0] = 0;
		move[1] = 0;
	}
	
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	
	char pl = player == 0 ? 'G' : 'B';
	std::cout << pl << " AI\ni = "<< move[0] / PRINT_SIZE + 1 <<", j = " << move[0] % PRINT_SIZE + 1 << '\n';
	std::cout << "i = "<< move[1] / PRINT_SIZE + 1 << ", j = " << move[1] % PRINT_SIZE + 1 << '\n';
	std::cout << "Time: " << elapsed_secs << '\n';
}

bool isEnd() {
	if (score[0] == 0 || score[1] == 0)
		return true;
	int res_score = score[0] + score[1];
	if (res_score >= GAME_SIZE)
		return true;
	if (move[0] == 0 && move[1] == 0)
		return true;
	return false;
}


void startGame(char player, bool firstMove) {
	for (int i = 0; i < GAME_SIZE; ++i) {
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
	playground[6] = 1;
	playground[42] = 1;
	playground[48] = 0;
	printPlayground();

	score[player1] = 2;
	score[player2] = 2;
	move[0] = 1;

	if (firstMove) {
		printCurrentPlayer(player1);
		inputMove(player1);
		//AIMove(player1, 6);
		makeMove(player1);
		printMove(player1);
	}
	
	while (!isEnd()) {
		printCurrentPlayer(player2);
		AIMove(player2, 6);
		makeMove(player2);
		printMove(player2);
		if (isEnd())
			break;
		printCurrentPlayer(player1);
		//AIMove(player1, 6);
		inputMove(player1);
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

	startGame('B', false);
}