#include <iostream>
#include <chrono>
#include <thread>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <sys/select.h>
using namespace std;

char getchNonBlocking() {
    char ch = 0;
    termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    fd_set set;
    struct timeval tv{0, 0};
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    if (select(STDIN_FILENO + 1, &set, NULL, NULL, &tv) > 0) {
        read(STDIN_FILENO, &ch, 1);
    }
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

struct node {
    int mi, mj;
    char data;
    node *next;
};

class Snake {
public:
    node *head;
    int m1, m2;
    int dx, dy;
    char hch, bch;
    int cs, hs;

    Snake() {
        m1 = 1; m2 = 1;
        dx = 0; dy = 1;
        hch = '>'; bch = '+';
        cs = 0; hs = 0;
        head = NULL;
        init();
    }

    void insert(char data) {
        node *n = new node{0, 0, data, NULL};
        if (head == NULL) head = n;
        else {
            node *tn = head;
            while (tn->next) tn = tn->next;
            tn->next = n;
        }
    }

    void init() {
        head = new node{m1, m2, hch, NULL};
        insert('+');
        insert('+');
    }

    bool onSnake(int x, int y) {
        if (x == m1 && y == m2) return true;
        node *tn = head;
        while (tn) {
            if (tn->mi == x && tn->mj == y) return true;
            tn = tn->next;
        }
        return false;
    }

    bool hitSnake(int x, int y, char arr[20][50]) {
        node *tn = head->next;
        while (tn) {
            if (tn->mi == x && tn->mj == y) return true;
            else if (arr[x][y] == '#') return true;
            tn = tn->next;
        }
        return false;
    }

    void changeIndex() {
        if (!head) return;
        node *tn = head;
        int prev_i = m1, prev_j = m2;
        while (tn) {
            int temp_i = tn->mi;
            int temp_j = tn->mj;
            tn->mi = prev_i;
            tn->mj = prev_j;
            prev_i = temp_i;
            prev_j = temp_j;
            tn = tn->next;
        }
    }

    void reset() {
        cs = 0; m1 = 1; m2 = 1; dx = 0; dy = 1; hch = '>';
        head = NULL;
        init();
    }
};

class Food {
public:
    int fi, fj;
    Food() { srand(time(0)); randomize(20, 50); }

    void randomize(int r, int c) {
        fi = (rand() % (r - 2)) + 1;
        fj = (rand() % (c - 2)) + 1;
    }

    void newFood(int r, int c, Snake &snake, char arr[20][50]) {
        do {
            fi = (rand() % (r - 2)) + 1;
            fj = (rand() % (c - 2)) + 1;
        } while (snake.onSnake(fi, fj) || arr[fi][fj] == '#');
    }
};

class Obstacle {
public:
    static const int obsCount = 28;
    int obsarr[obsCount][2] = {
        {7,23},{7,24},{7,25},{7,26},{12,23},{12,24},{12,25},{12,26},
        {9,21},{10,21},{9,28},{10,28},{4,16},{5,15},{6,14},{7,13},
        {15,16},{14,15},{13,14},{12,13},{4,33},{5,34},{6,35},{7,36},
        {15,33},{14,34},{13,35},{12,36}
    };

    void draw(char arr[20][50]) {
        for (int i = 0; i < obsCount; i++)
            arr[obsarr[i][0]][obsarr[i][1]] = '#';
    }
};

class GameBoard {
    static const int r = 20;
    static const int c = 50;
    char arr[r][c];

public:
    Snake snake;
    Food food;
    Obstacle obstacle;

    void display() {
        string tempstr = "";
        if (c > 35)
            for (int i = 0; i < (c - 31); i++) tempstr += " ";
        else tempstr += "\n";

        cout << "Current Score:" << snake.cs << tempstr << "High Score:" << snake.hs << endl;
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if (arr[i][j] == ' ')
                    cout << "\u2B1C";
                else if (arr[i][j] == '#')
                    cout << "\U0001F9F1";
                else if (arr[i][j] == 'o')
                    cout << "\U0001F34E";
                else if (arr[i][j] == '>' || arr[i][j] == '<' || arr[i][j] == '^' || arr[i][j] == 'v')
                    cout << "\U0001F438";
                else if (arr[i][j] == '+')
                    cout << "\U0001F7E9";
                else
                    cout << arr[i][j];
            }
            cout << endl;
        }
    }

    void createGrid() {
        cout << "\033[H\033[J";
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < c; j++) {
                if ((i == 0 || i == r - 1) || (j == 0 || j == c - 1))
                    arr[i][j] = '#';
                else if (i == food.fi && j == food.fj) {
                    if (snake.m1 == food.fi && snake.m2 == food.fj) {
                        snake.insert(snake.bch);
                        snake.cs += 10;
                        food.newFood(r, c, snake, arr);
                    } else arr[i][j] = 'o';
                } else arr[i][j] = ' ';

                node *tn = snake.head;
                while (tn) {
                    arr[tn->mi][tn->mj] = tn->data;
                    tn = tn->next;
                }
            }
        }
        obstacle.draw(arr);

        if ((snake.m1 == 0 || snake.m1 == r - 1) || (snake.m2 == 0 || snake.m2 == c - 1) || snake.hitSnake(snake.m1, snake.m2, arr)) {
            handleGameOver();
        } else {
            arr[snake.m1][snake.m2] = snake.hch;
        }
    }

    void handleGameOver() {
        snake.m1 -= snake.dx;
        snake.m2 -= snake.dy;
        arr[snake.m1][snake.m2] = snake.hch;
        if (snake.cs > snake.hs) snake.hs = snake.cs;
        display();
        cout << "Game Over!\nFinal Score: " << snake.cs << "\n" << endl;
        cout << "Press any key-Restart\nX-Exit\n";
        char c = 0;
        while (c == 0) c = getchNonBlocking();
        switch (c) {
            case 'x': case 'X': exit(0);
            default: snake.reset(); food.newFood(r, c, snake, arr);
        }
    }

    void handleInput() {
        char ch = getchNonBlocking();
        if (ch == '\033') {
            getchNonBlocking();
            switch (getchNonBlocking()) {
                case 'A': snake.dx = -1; snake.dy = 0; snake.hch = '^'; break;
                case 'B': snake.dx = 1;  snake.dy = 0; snake.hch = 'v'; break;
                case 'C': snake.dx = 0;  snake.dy = 1; snake.hch = '>'; break;
                case 'D': snake.dx = 0;  snake.dy = -1; snake.hch = '<'; break;
            }
        } else {
            switch (ch) {
                case 'w': case 'W': snake.dx = -1; snake.dy = 0; snake.hch = '^'; break;
                case 'a': case 'A': snake.dx = 0; snake.dy = -1; snake.hch = '<'; break;
                case 's': case 'S': snake.dx = 1; snake.dy = 0; snake.hch = 'v'; break;
                case 'd': case 'D': snake.dx = 0; snake.dy = 1; snake.hch = '>'; break;
            }
        }
    }

    void run() {
        while (true) {
            handleInput();
            snake.m1 += snake.dx;
            snake.m2 += snake.dy;
            snake.changeIndex();
            createGrid();
            display();
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
};

int main() {
    GameBoard game;
    game.run();
    return 0;
}
