// Pacman Game in C language 
#include <conio.h> // 콘솔 창에서 입력 및 출력을 다루는 함수와 상수들을 제공
#include <stdio.h> // 표준 입출력
#include <stdlib.h> // 표준 라이브러리 함수

// All the elements to be used 
// Declared here 
#define WIDTH 40 // 게임 보드 넓이
#define HEIGHT 20 // 게임 보드 높이
#define PACMAN 'C' // 팩맨
#define WALL '#' // 벽
#define FOOD '.' // 음식
#define EMPTY ' ' // 빈 공간
#define DEMON 'X' // 악마

// Global Variables are 
// Declared here 
int res = 0; // 게임 결과
int score = 0; //게임 점수
int pacman_x, pacman_y; // 팩맨 현재 위치
char board[HEIGHT][WIDTH]; // 게임 보드를 나타내는 2차원 배열
int food = 0; // 음식의 총 개수
int curr = 0; // 현재 상태
void initialize() // 게임 보드 초기화 역할 함
{ 
	// Putting Walls as boundary in the Game 
	// 게임 보드를 높이, 너비로 정의된 크기로 설정하고, 중첩 for루프를 사용 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			if (i == 0 || j == WIDTH - 1 || j == 0 
				|| i == HEIGHT - 1) { 
				board[i][j] = WALL; 
			} // 게임보드의 가장자리에 벽 배치
			else
				board[i][j] = EMPTY; 
		} // 그 외 나머지 공간은 빈 공간으로 설정
	} 

	// Putting Walls inside the Game 
	int count = 50; // 50으로 초기화
	while (count != 0) { // 50개의 벽을 랜덤하게 게임 보드 내부에 배치 
		int i = (rand() % (HEIGHT + 1)); // 0부터 height + 1 난수 생성
		int j = (rand() % (WIDTH + 1)); // 0부터 width + 1 난수 생

		if (board[i][j] != WALL && board[i][j] != PACMAN) { // 선택한 위치 의 셀이 벽이 아니고 팩맨이 닌 경우 
			board[i][j] = WALL; // 해당 위치에 벽 배
			count--; // count 변수 감
		} 
	} // while 루프 반복하면서 50개의 벽 랜덤하게 배치 

	int val = 5; // while 루프를 5번 반복하기 위해 변수를 5로 초기화
	while (val--) { 
		int row = (rand() % (HEIGHT + 1)); 
		for (int j = 3; j < WIDTH - 3; j++) { 
			if (board[row][j] != WALL 
				&& board[row][j] != PACMAN) { 
				board[row][j] = WALL; 
			} 
		} 
	} 

	// Putting Demons in the Game 
	count = 10; 
	while (count != 0) { 
		int i = (rand() % (HEIGHT + 1)); 
		int j = (rand() % (WIDTH + 1)); 

		if (board[i][j] != WALL && board[i][j] != PACMAN) { 
			board[i][j] = DEMON; 
			count--; 
		} 
	} 

	// Cursor at Center 
	pacman_x = WIDTH / 2; 
	pacman_y = HEIGHT / 2; 
	board[pacman_y][pacman_x] = PACMAN; 

	// Points Placed 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			if (i % 2 == 0 && j % 2 == 0 
				&& board[i][j] != WALL 
				&& board[i][j] != DEMON 
				&& board[i][j] != PACMAN) { 

				board[i][j] = FOOD; 
				food++; 
			} 
		} 
	} 
} 

void draw() 
{ 
	// Clear screen 
	system("cls"); 

	// Drawing All the elements in the screen 
	for (int i = 0; i < HEIGHT; i++) { 
		for (int j = 0; j < WIDTH; j++) { 
			printf("%c", board[i][j]); 
		} 
		printf("\n"); 
	} 
	printf("Score: %d\n", score); 
} 

// Function enables to move the Cursor 
void move(int move_x, int move_y) 
{ 
	int x = pacman_x + move_x; 
	int y = pacman_y + move_y; 

	if (board[y][x] != WALL) { 
		if (board[y][x] == FOOD) { 
			score++; 
			food--; 
			curr++; 
			if (food == 0) { 
				res = 2; 
				return; 
			} 
		} 
		else if (board[y][x] == DEMON) { 
			res = 1; 
		} 

		board[pacman_y][pacman_x] = EMPTY; 
		pacman_x = x; 
		pacman_y = y; 
		board[pacman_y][pacman_x] = PACMAN; 
	} 
} 

// Main Function 
int main() 
{ 
	initialize(); 
	char ch; 
	food -= 35; 
	int totalFood = food; 
	// Instructions to Play 
	printf(" Use buttons for w(up), a(left) , d(right) and "
		"s(down)\nAlso, Press q for quit\n"); 

	printf("Enter Y to continue: \n"); 

	ch = getch(); 
	if (ch != 'Y' && ch != 'y') { 
		printf("Exit Game! "); 
		return 1; 
	} 

	while (1) { 
		draw(); 
		printf("Total Food count: %d\n", totalFood); 
		printf("Total Food eaten: %d\n", curr); 
		if (res == 1) { 
			// Clear screen 
			system("cls"); 
			printf("Game Over! Dead by Demon\n Your Score: "
				"%d\n", 
				score); 
			return 1; 
		} 

		if (res == 2) { 
			// Clear screen 
			system("cls"); 
			printf("You Win! \n Your Score: %d\n", score); 
			return 1; 
		} 

		// Taking the Input from the user 
		ch = getch(); 

		// Moving According to the 
		// input character 
		switch (ch) { 
		case 'w': 
			move(0, -1); 
			break; 
		case 's': 
			move(0, 1); 
			break; 
		case 'a': 
			move(-1, 0); 
			break; 
		case 'd': 
			move(1, 0); 
			break; 
		case 'q': 
			printf("Game Over! Your Score: %d\n", score); 
			return 0; 
		} 
	} 

	return 0; 
}
