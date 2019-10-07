// InfectionGame.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <map>
#include <cstdlib>

using std::cout;
using std::cin;
using std::map;

char playground[36]; // игровое поле
int adjacencyTB[36][36]; //таблица смежности
int allowTB[36][36][2]; // таблица разрешеннных ходов
int depth; // максимальное количество оставшихся ходов
map<char, int> score; // очки 

struct move {
	int parentPos;
	int newPos;
};

void makeAdjacencyTB() {
	for (int i = 0; i < 36; ++i)
		for(int j = 0; j < 36; ++j)
			if (i == j)
				adjacencyTB[i][j] = 0;
			else {
				int x1 = i / 6;
				int x2 = j / 6;
				int y1 = i % 6;
				int y2 = j % 6;
				if (abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1)
					adjacencyTB[i][j] = 1;
				else
					adjacencyTB[i][j] = 0;
				
				//adjacencyTB[i][j] = abs(i - j) / 6 <= 1 && abs(i - j) % 6 <= 1 ? 1 : 0;
			}
}

void makeAllowTB() {

}

// печать игрового поля
void printPlayground() {
	cout << "   ";
	for (int i = 0; i < 6; ++i)
		cout << i << ' ';

	cout << '\n';
	
	for (int i = 0; i < 6; ++i) {
		cout << i << "  ";
		for (int j = 0; j < 6; ++j)
			cout << playground[i * 6 + j] << ' ';
		cout << '\n';
	}
	cout << '\n';
}

void printMove(char player) {
	cout << "########" << '\n';
	if (player == 'G')
		cout << "GREEN Move" << '\n';
	else
		cout << "BLUE Move" << '\n';
	printPlayground();
}

// сделать ход из parentPos в newPos
void makeMove(char player, int parentPos, int newPos) {
	char enemy = player == 'G' ? 'B' : 'G';
	if (adjacencyTB[parentPos][newPos] != 1) {
		playground[parentPos] = '*';
		depth += 1;
		if (score[player] != 0)
			score[player] -= 1;
	}
	
	playground[newPos] = player;
	depth -= 1;
	score[player] += 1;

	for (int i = 0; i < 36; ++i)
		if (playground[i] != '*' && adjacencyTB[newPos][i] == 1) {
			if (playground[i] == enemy)
				score[enemy] -= 1;
			playground[i] = player;
			depth -= 1;
			score[player] += 1;
		}
}

move inputMove() {
	move result;
	int i, j;
	cin >> i >> j;
	result.parentPos = i * 6 + j;
	cin >> i >> j;
	result.newPos = i * 6 + j;
	return result;
}

move randomMove(char player) {
	int parent = rand() % 36;
	while (playground[parent] != player)
		parent = rand() % 36;
	int newPos = rand() % 36;
	while(playground[newPos] != '*')
		newPos = rand() % 36;

	return move{parent, newPos};
}

void startGame(char humanPlayer, bool firstMove) {
	for (int i = 0; i < 36; ++i) {
		playground[i] = '*';
	}

	char AI = humanPlayer == 'B' ? 'G' : 'B';
	
	// начальная расстановка
	playground[0] = 'G';
	playground[5] = 'B';
	playground[30] = 'B';
	playground[35] = 'G';
	printPlayground();

	score = map<char, int>();
	score['G'] = 2;
	score['B'] = 2;
	depth = 32;

	move currentMove;
	if (firstMove) {
		currentMove = randomMove(humanPlayer);
		makeMove(humanPlayer, currentMove.parentPos, currentMove.newPos);
		printMove(humanPlayer);
	}
	
	while (depth != 0 && score[AI] != 0) {
		currentMove = randomMove(AI);
		makeMove(AI, currentMove.parentPos, currentMove.newPos);
		printMove(AI);
		if (depth == 0 || score[humanPlayer] == 0)
			break;
		currentMove = randomMove(humanPlayer);
		makeMove(humanPlayer, currentMove.parentPos, currentMove.newPos);
		printMove(humanPlayer);
	}

	if (score[humanPlayer] > score[AI])
		cout << "Человек одолел машину со счетом " << score[humanPlayer] << " : " << score[AI] << '\n';
	else
		cout << "Человечество проиграло... Счет " << score[humanPlayer] << " : " << score[AI] << '\n';
}

int main()
{
	makeAdjacencyTB();
	
	/*
	for (int i = 0; i < 36; ++i) {
		for (int j = 0; j < 36; ++j)
			cout << adjacencyTB[i][j] << ' ';
		cout << '\n';
	}*/

	startGame('G', true);
}