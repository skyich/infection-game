// InfectionGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>

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
	cout << "GREEN Score " << score[0] <<'\n';
	cout << "BLUE Score " << score[1] << '\n';
	if (player == 0)
		cout << "GREEN Move" << '\n';
	else
		cout << "BLUE Move" << '\n';
	printPlayground();
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

void inputMove() {
	int i, j;
	cin >> i >> j;
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

void temp_move(char* pg, char player, int x, int y) {
	char enemy = player == 1 ? 0 : 1;
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
						temp_move(temp_pg, player, i, j);
						int value = minimax(temp_pg, enemy, depth - 1, false, alpha, beta);
						if (value > bestVal) {
							bestVal = value;
							move[0] = i;
							move[1] = j;
						}
						if (alpha > bestVal)
							alpha = bestVal;
						if (beta <= alpha)
							break;
						std::copy(save, save + 36, temp_pg);
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
						temp_move(temp_pg, player, i, j);
						int value = minimax(temp_pg, enemy, depth - 1, true, alpha, beta);
						if (value < bestVal) {
							bestVal = value;
						}
						if (alpha < bestVal)
							alpha = bestVal;
						if (beta <= alpha)
							break;
						std::copy(save, save + 36, temp_pg);
					}
				}
			}
		}
		return bestVal;
	}
}


void AIMove(char player) {
	char temp[36];
	std::copy(playground, playground + 36, temp);
	minimax(temp, player, 2, true, INT_MIN, INT_MAX);
	std::cout << "I   "<< move[0] <<"  J  " << move[1] << '\n';
}


void startGame(char humanPlayer, bool firstMove) {
	for (int i = 0; i < 36; ++i) {
		playground[i] = '*';
	}

	char human, AI;
	if (humanPlayer == 'G') {
		human = 0;
		AI = 1;
	}
	else {
		human = 1;
		AI = 0;
	}
	
	// начальная расстановка
	playground[0] = 0;
	playground[5] = 1;
	playground[30] = 1;
	playground[35] = 0;
	printPlayground();

	score[human] = 2;
	score[AI] = 2;
	depth = 32;

	if (firstMove) { // первый ход человек
		AIMove(human);
		makeMove(human);
		printMove(human);
	}
	
	while (depth != 0 && score[AI] != 0) {
		AIMove(AI);
		makeMove(AI);
		printMove(AI);
		if (depth == 0 || score[human] == 0)
			break;
		AIMove(human);
		makeMove(human);
		printMove(human);
	}

	if (score[human] > score[AI])
		cout << "Human win " << score[human] << " : " << score[AI] << '\n';
	else
		cout << "AI Win...  " << score[human] << " : " << score[AI] << '\n';
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