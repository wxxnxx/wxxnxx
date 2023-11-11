void moveDemon() { // moveDemon() 함수 추가
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == DEMON) {
                int move_x = (rand() % 3) - 1; // -1, 0, 1 중 하나를 랜덤하게 선택
                int move_y = (rand() % 3) - 1; // -1, 0, 1 중 하나를 랜덤하게 선택

                int x = j + move_x;
                int y = i + move_y;

                // 해당 위치가 벽이거나 팩맨의 위치가 아닐 때 악마 이동 
                if (board[y][x] != WALL && board[y][x] != PACMAN) { 
                    board[i][j] = EMPTY; // 이동 전의 위치를 빈 공간으로 설정
                    board[y][x] = DEMON; // 새로운 위치에 악마 표시
                }
            }
        }
    }
}

// main 함수  
{
...
moveDemon(); // 악마 움직임 처리
...
}
