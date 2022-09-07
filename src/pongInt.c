// Copyright 2021 <School 21>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#define HEIGHT 26
#define WIDTH 81
#define S_BALLX 41
#define S_BALLY 13
#define S_LINE1Y 12
#define S_LINE2Y 12

void printField(int, int, int, int, int, int);  // отрисовка всего
int interact();

int main() {
    int score1 = 0;
    int score2 = 0;
    int ballX = S_BALLX;  // 41 - нахождение в центре карты по х
    int ballY = S_BALLY;  // 13 - нахождение в центре карты по у
    int line1Y = S_LINE1Y;  // 12
    int line2Y = S_LINE2Y;  // 12
    int vectX = 1;
    int vectY = -1;
    printField(line1Y, line2Y, ballY, ballX, score1, score2);
    while ((score1 <= 21) || (score2 <= 21)) {
        int flag = 0;
        char c;
        system("/bin/stty -icanon");
        if (interact() == 1) {
            scanf("%c", &c);
            switch (c) {
                case 'z':
                    if ((line1Y + 3) != HEIGHT) {  // рокетка не вышла за поле
                        line1Y += 1;  // Движение первой рокетки вниз
                    }
                    break;
                case 'a':
                    if (line1Y != 1) {  // Чтобы рокетка не вышла за поле
                        line1Y -= 1;  // Движение первой рокетки вверх
                    }
                    break;
                case 'm':
                    if ((line2Y + 3) != HEIGHT) {  // рокетка не вышла за поле
                        line2Y += 1;  // Движение второй рокетки вниз
                    }
                    break;
                case 'k':
                    if (line2Y != 1) {  // Чтобы рокетка не вышла за поле
                        line2Y -= 1;  // Движение второй рокетки вверх
                    }
                case ' ':
                    break;
                case 'q':
                    flag = 2;
                    break;
                default:
                    flag = 1;
                    break;
            }
        }
        system("/bin/stty -icanon");
        if (flag == 1) {
            printField(line1Y, line2Y, ballY, ballX, score1, score2);
            continue;
        } else if (flag == 2) {
            break;
        }
        if ((ballY == 1) || (ballY == HEIGHT - 1)) {
            vectY = -(vectY);
        }
        if ((ballX == 3) && ((ballY == line1Y) ||
                              (ballY == line1Y + 1) ||
                              (ballY == line1Y + 2) ||
                              (ballY == line2Y + 3))) {
            vectX = -(vectX);
        } else if ((ballX == WIDTH - 3) && ((ballY == line2Y) ||
                                     (ballY == line2Y + 1) ||
                                     (ballY == line2Y + 2) ||
                                     (ballY == line2Y + 3))) {
            vectX = -(vectX);
        }
        ballX += vectX;
        ballY += vectY;
        printField(line1Y, line2Y, ballY, ballX, score1, score2);
        if ((ballX == 2) && ((ballY != line1Y) ||
                                    (ballY != line1Y + 1) ||
                                    (ballY != line1Y + 2) ||
                                    (ballY != line1Y + 3))) {
            score2 += 1;
            ballX = S_BALLX;
            ballY = S_BALLY;
            line1Y = S_LINE1Y;
            line2Y = S_LINE2Y;
            vectX = 1;
            vectY = -1;
        } else if ((ballX == WIDTH - 2) && ((ballY != line2Y) ||
                                             (ballY != line2Y + 1) ||
                                             (ballY != line2Y + 2) ||
                                             (ballY != line2Y + 3))) {
            score1 += 1;
            ballX = S_BALLX;
            ballY = S_BALLY;
            line1Y = S_LINE1Y;
            line2Y = S_LINE2Y;
            vectX = -1;
            vectY = -1;
        }
        if (score1 == 21) {
            flag = 21;
        } else if (score2 == 21) {
            flag = 22;
        } else {
            flag = 0;
            printf("\nPlayer 1 -- A/M. ");
            printf("Player 2 -- K/M. ");
            printf("Press 'q' to Exit. \n\n");
        }
        if (flag == 21) {
            printField(line1Y, line2Y, ballY, ballX, score1, score2);
            printf("\nPlayer_1 won the game!\n\n");
            break;
        } else if (flag == 22) {
            printField(line1Y, line2Y, ballY, ballX, score1, score2);
            printf("\nPlayer_2 won the game!\n\n");
            break;
        }
        system("sleep 0.2*");
    }
    return 0;
}

void printField(int line1Y, int line2Y, int ballY,
                int ballX, int score1, int score2) {
    printf("\n\n\n\n\n");
    printf("\t\t\t====\t====\t=     =    ====\n");  // пишем pong знаками "="
    printf("\t\t\t=  =\t=  =\t==    =    =\n");
    printf("\t\t\t=  =\t=  =\t= =   =    =\n");
    printf("\t\t\t====\t=  =\t=  =  =    = ===\n");
    printf("\t\t\t=   \t=  =\t=   = =    =   =\n");
    printf("\t\t\t=   \t=  =\t=    ==    =   =\n");
    printf("\t\t\t=   \t====\t=     =    =====\n");
    printf("\n\n\n\n\n\t\tPlayer one: %d\t\t\t\t", score1);
    printf("Player two: %d\n", score2);
    printf(" ");
    for (int i = 0; i < WIDTH - 1; ++i) {
        printf("%c", '-');  // отрисовка верха по х "-"
    }
    printf(" ");
    printf("\n");
    for (int y = 0; y < HEIGHT - 1; ++y) {  // HEIGHT = 27
        printf("|");  // отрисовка слева по у "|"
        for (int x = 0; x < WIDTH - 1; ++x) {  // WIDTH = 82, поле шириной 80.
            if ((x == ballX - 1) && (y == ballY - 1) && (ballX != 1)) {
                printf("%c", '*');
            } else {
                if (x == WIDTH / 2) {  // S_BALLX = 42, S_BALLY = 13
                    printf("|");  // линии в центре поля '|'
                } else {
                    if ((x == 1) && ((y - 1 == line1Y) ||
                                     (y - 1 == (line1Y - 1)) ||
                                     (y - 1 == (line1Y - 2)))) {
                        printf("|");  // отрисовка ракетки первого игрока
                    } else {
                        if ((x == WIDTH - 3) && ((y - 1 == line2Y) ||
                                                 (y - 1 == line2Y - 1) ||
                                                 (y - 1 == line2Y - 2))) {
                            printf("|");  // отрисовка ракетки второго игрока
                        } else {
                            printf(" ");
                        }
                    }
                }
            }
        }
        printf("|\n");  // отрисовка справа по у "|"
    }
    printf(" ");
    for (int i = 0; i < WIDTH - 1; ++i) {
        printf("%c", '-');  // отрисовка низа по х: "------"
    }
    printf(" ");
}

int interact() {
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF) {
    ungetc(ch, stdin);
    return 1;
  }
  return 0;
}
