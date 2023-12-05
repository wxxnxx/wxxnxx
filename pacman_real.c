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
    }

// 악마 움직임 로직
void moveDemon() {
    if (lives <= 0) {
        return;
    }
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == DEMON) {
                if (rand() % 2 == 0 && isEaten == 0) {
                    int move_x, move_y;
                    do {
                        move_x = (rand() % 3 - 1) * 2;
                        move_y = (rand() % 3 - 1) * 2;
                    } while (j + move_x < 0 || j + move_x >= WIDTH || i + move_y < 0 || i + move_y >= HEIGHT ||
                             board[i + move_y][j + move_x] == WALL || board[i + move_y][j + move_x] == PACMAN ||
                             board[i + move_y][j + move_x] == FOOD || board[i + move_y][j + move_x] == DEMON);

                    int new_x = j + move_x;
                    int new_y = i + move_y;

                    board[i][j] = EMPTY;
                    board[new_y][new_x] = DEMON;
                }
            }
        }
    }
}

void draw(struct Difficulty difficulty, int totalFood) {
    system("cls");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (difficulty.demonMoveEnabled == 1 && difficulty.visibilityRadius > 0) {
                int distance = abs(pacman_x - j) + abs(pacman_y - i); 

                if (distance <= difficulty.visibilityRadius) {
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

    printf("Score: %d\n", score);
    printf("Lives: %d\n", lives);
    printf("Total Food: %d\n", totalFood);
}

void move(int move_x, int move_y) {
    int x = pacman_x + move_x;
    int y = pacman_y + move_y;

    if (board[y][x] != WALL) {
        char originalContent = board[y][x];

        if (originalContent == FOOD) {
            score++;
            food--;
            curr++;
            if (food == 0) {
                res = 2;
                return;
            }
        } else if (originalContent == DEMON) {
            isEaten = 1;
            lives--;
            if (lives == 0) {
                res = 1;
                return;
            }
        }

        board[pacman_y][pacman_x] = EMPTY;
        pacman_x = x;
        pacman_y = y;
        board[pacman_y][pacman_x] = PACMAN;
    }
}


int gameStart(struct Difficulty difficulty) {
    int totalFood;

    initialize(difficulty);
    placeFood(difficulty, &totalFood);
    char ch;

    while (1) {
        if (difficulty.demonMoveEnabled == 1) {
            moveDemon();
        }
        draw(difficulty, totalFood);

        printf("Total Food eaten: %d\n", curr);

        ch = getch();

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

        if (res == 1) {
            printf("Game Over! Dead by Demon\n Your Score: %d\n", score);
            exit(0);
        } else if (res == 2) {
            printf("You Win! \n Your Score: %d\n", score);
            exit(0);
        }
    }
}

int main() {
   
    int select;

    while (1) {
    system("cls");
    puts("┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
    puts("┃        ▤▤ 팩맨 게임 ▤▤         ┃");
    puts("┃      ▤▤  난이도 선택  ▤▤       ┃");
    puts("┃                                  ┃");
    puts("┃          1. 쉬      움            ┃");
    puts("┃                                  ┃");
    puts("┃          2. 보      통           ┃");
    puts("┃                                  ┃");
    puts("┃          3. 어  려  움           ┃");
    puts("┃                                  ┃");
    puts("┃          0. 종      료           ┃");
    puts("┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
    printf(" Select > ");
    scanf("%d", &select);
    system("cls");

    switch(select) {
        case 1:
            lives = 5;
            gameStart((struct Difficulty){.lives = lives, .demonMoveEnabled = 0, .wallCount = 0, .demonadd = 10, .randomfood = 0, .visibilityRadius = 0});
            break;
        case 2:
            lives = 3;
            gameStart((struct Difficulty){.lives = lives, .demonMoveEnabled = 1, .wallCount = 5, .demonadd = 10, .randomfood = 15, .visibilityRadius = 0});
            break;
        case 3:
    lives = 1;
    gameStart((struct Difficulty){.lives = lives, .demonMoveEnabled = 1, .wallCount = 10, .demonadd = 20, .randomfood = 25, .visibilityRadius = 7});
    break;

        case 0:
            exit(0);
        default:
            printf("잘못된 접근입니다.\n");
            break;
    }
}
    
    return 1;
}
