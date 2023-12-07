#include <conio.h> // 콘솔 입출력
#include <stdio.h> // 표준 입출력 함수
#include <stdlib.h> // 유틸리티 함수
#include <windows.h> // Windows API 함수

#define HEIGHT 30 // 높이
#define WIDTH 50 // 너비
#define PACMAN 'C' // 팩맨
#define WALL '#' // 벽
#define FOOD '.' // 음식
#define EMPTY ' ' // 빈 공간
#define DEMON 'X' // 악마
 
struct Difficulty {
    int lives; // 목숨
    int demonMoveEnabled; // 악마 움직임
    int wallCount; // 추가 벽
    int demonadd; // 추가 악마
    int randomfood; // 추가 음식
    int visibilityRadius; // 가시 반경
};

int res = 0; // 결과
int score = 0; // 점수
int pacman_x, pacman_y; // 팩맨 위치
char board[HEIGHT][WIDTH]; // 게임 보드
int food = 0; // 음식
int curr = 0; // 현재 점수
int lives; // 목숨
int isEaten = 0; // 먹은 음식 개수

// 게임 보드 초기화
void initialize(struct Difficulty difficulty) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                board[i][j] = WALL; // 가장자리 벽 생성
            } else if (i > 1 && i < HEIGHT - 2 && j > 1 && j < WIDTH - 2 && rand() % 100 < 20) {
                board[i][j] = WALL; // 내부 영역에서 각 칸에 대해 20%의 확률로 벽 생성
            } else {
                board[i][j] = EMPTY; // 나머지 공간은 빈 공간
            }
        }
    	for (int k = 0; k < difficulty.demonadd; k++) { 
        int count = 1;
        while (count != 0) {
            int i = rand() % HEIGHT; // 악마 무작위로 추가
            int j = rand() % WIDTH;

            if (board[i][j] != WALL && board[i][j] != PACMAN && board[i][j] != DEMON) {
                board[i][j] = DEMON; // 벽, 팩맨, 악마가 아닌 곳에 배치
                count--;
            }
        }
    }

   	 int count = 20 + difficulty.wallCount;
	 while (count != 0) {
		int i = (rand() % (HEIGHT - 2)) + 1; // 벽 무작위로 추가
		int j = (rand() % (WIDTH - 2)) + 1;
    
        if (board[i][j] != WALL && board[i][j] != PACMAN) {
            if (board[i-1][j] != WALL && board[i+1][j] != WALL && board[i][j-1] != WALL && board[i][j+1] != WALL) {
                board[i][j] = WALL; // 해당 위치의 상하좌가 벽이 아닐 때 벽 추가
                count--;
            }
        }
    }
    
    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;
    }

void placeFood(struct Difficulty difficulty, int *totalFood) { // 음식 무작위로 배치
    int foodCount = 1 + difficulty.randomfood; 
    *totalFood = foodCount; // totalFood가 가리키는 메모리 주소에 foodCount 값을 저장

    while (foodCount != 0) {
        int i = (rand() % (HEIGHT - 2)) + 1; 
        int j = (rand() % (WIDTH - 2)) + 1;   

        if (board[i][j] != WALL && board[i][j] != DEMON && board[i][j] != PACMAN && board[i][j] != FOOD) {
            board[i][j] = FOOD; // 벽, 악마, 팩맨, 음식이 없는 곳에 배치
            foodCount--;
        }
    }
}

// 악마 움직임 로직
void moveDemon() {
    if (lives <= 0) { // 팩맨의 목숨이 0 이하일때
        return; // 악마가 움직이지 않음
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == DEMON) { // 현재 위치에 악마가 있을 때
                if (rand() % 2 == 0 && isEaten == 0) { // 무작위로 생성된 난수가 짝수이고 isEaten이 0일 때
                    int move_x, move_y;
                    do { // 무작위로 악마를 움직일 새로운 좌표 생성
                        move_x = (rand() % 3 - 1) * 2;
                        move_y = (rand() % 3 - 1) * 2;
                    } while // 벽, 팩맨, 음식, 악마가 없는 곳
		    (j + move_x < 0 || j + move_x >= WIDTH || i + move_y < 0 || i + move_y >= HEIGHT ||
		     board[i + move_y][j + move_x] == WALL || board[i + move_y][j + move_x] == PACMAN ||
		     board[i + move_y][j + move_x] == FOOD || board[i + move_y][j + move_x] == DEMON);
		    // 악마 이동
                    int new_x = j + move_x;
                    int new_y = i + move_y;

                    board[i][j] = EMPTY;
                    board[new_y][new_x] = DEMON;
                }
            }
        }
    }
}

// 게임 화면을 콘솔에 출력하는 함수
void draw(struct Difficulty difficulty, int totalFood) {
    system("cls");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
	    // 악마 움직임이 1이고 가시 반경 내에 있을 때
            if (difficulty.demonMoveEnabled == 1 && difficulty.visibilityRadius > 0) {
                int distance = abs(pacman_x - j) + abs(pacman_y - i); // 시야 제한
		    
                if (distance <= difficulty.visibilityRadius) {
                    if (board[i][j] == PACMAN) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                        printf("%c", board[i][j]); // 팩맨이 노란색
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    } else if (board[i][j] == DEMON) { // 악마가 빨간색
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        printf("%c", board[i][j]); // 벽이 파란색
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    } 
			else if (board[i][j] == WALL){
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
			printf("%c", board[i][j]);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			}
			else {
                        printf("%c", board[i][j]);
                    }
                } else {
                    printf(" ");
                }
            } else {
                if (board[i][j] == PACMAN) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
                    printf("%c", board[i][j]);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                } else if (board[i][j] == DEMON) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    printf("%c", board[i][j]);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                } 
		else if (board[i][j] == WALL){
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		    printf("%c", board[i][j]);
		    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				}
				else {
                    printf("%c", board[i][j]);
                }
            }
        }
        printf("\n");
    }

    printf("Score: %d\n", score); // 점수
    printf("Lives: %d\n", lives); // 목숨 개수
    printf("Total Food: %d\n", totalFood); // 총 음식 개수
}

// 팩맨을 움직이는 함수
void move(int move_x, int move_y) { // 새로운 좌표 계산
    int x = pacman_x + move_x;
    int y = pacman_y + move_y;

    if (board[y][x] != WALL) { // 새로운 좌표에 있는 요소가 벽이 아니라면
        char originalContent = board[y][x]; // 계속 진행

        if (originalContent == FOOD) { // 새로운 요소가 음식이라면
            score++; // 점수 증가
            food--; // 음식 감소
            curr++; // 현재 점수 증가
            if (food == 0) { // 음식을 모두 먹었다면
                res = 2; // 승리
                return;
            }
        } else if (originalContent == DEMON) { // 악마를 만났다면
            isEaten = 1;
            lives--; // 목숨 감소
            if (lives == 0) { // 목숨이 0 이라면
                res = 1; // 패배
                return;
            }
        }
        board[pacman_y][pacman_x] = EMPTY; // 현재 팩맨의 위치를 빈 공간으로 변경
        pacman_x = x;
        pacman_y = y;
        board[pacman_y][pacman_x] = PACMAN; // 새로운 좌표로 팩맨 이동
    }
}

// 게임을 시작하는 함수
int gameStart(struct Difficulty difficulty) {
    int totalFood;

    initialize(difficulty); // 게임 초기화
    placeFood(difficulty, totalFood); // 음식 배치, 전체 음식 개수
    char ch;

    while (1) {
        if (difficulty.demonMoveEnabled == 1) { // 악마 움직임이 1인 경우
            moveDemon(); // 악마 움직임 함수 호출
        }
        draw(difficulty, totalFood); // 게임 상태 화면에 그리기

        printf("Total Food eaten: %d\n", curr);

        ch = getch(); // 사용자의 키 입력 받기

	// 받은 키에 따라 move함수 호출하여 팩맨 움직임
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

        if (res == 1) { // 승리했을 때
            printf("Game Over! Dead by Demon\n Your Score: %d\n", score);
            exit(0);
        } else if (curr == totalFood) { //패배했을 때
            printf("You Win! \n Your Score: %d\n", score);
            exit(0);
        }
    }
}

int main() {
   
    int select; // 난이도 선택 변수

    while (1) {
    system("cls");
    printf("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n");
    printf("┃        ▤▤ 팩맨 게임 ▤▤        ┃\n");
    printf("┃      ▤▤  난이도 선택  ▤▤      ┃\n");
    printf("┃                                  ┃\n");
    printf("┃          1. 쉬      움           ┃\n");
    printf("┃                                  ┃\n");
    printf("┃          2. 보      통           ┃\n");
    printf("┃                                  ┃\n");
    printf("┃          3. 어  려  움           ┃\n");
    printf("┃                                  ┃\n");
    printf("┃          0. 종      료           ┃\n");
    printf("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n");
    printf(" Select > ");
    scanf("%d", &select);
    system("cls");

    switch(select) {
        case 1: // 난이도 쉬움
            lives = 5;
            gameStart((struct Difficulty){.lives = lives, .demonMoveEnabled = 0, .wallCount = 0, 
		    			.demonadd = 10, .randomfood = 0, .visibilityRadius = 0}, totalFood);
            break;
        case 2: // 난이도 보통
            lives = 3;
            gameStart((struct Difficulty){.lives = lives, .demonMoveEnabled = 1, .wallCount = 5, 
		    			.demonadd = 10, .randomfood = 15, .visibilityRadius = 0}, totalFood);
            break;
        case 3:// 난이도 어려움
    lives = 1;
    gameStart((struct Difficulty){.lives = lives, .demonMoveEnabled = 1, .wallCount = 10, 
	    					.demonadd = 20, .randomfood = 25, .visibilityRadius = 7}, totalFood);
    break;

        case 0: // 프로그램 종료
            exit(0);
        default:
            printf("잘못된 접근입니다.\n");
            break;
    }
}
    
    return 1;
}
