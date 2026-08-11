#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif
using namespace std;

void setupConsole() {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    SetConsoleOutputCP(CP_UTF8);
#endif
}

char getchNonBlocking() {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
    return 0;
#else
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
#endif
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
        if (head == NULL) {
            n->mi = m1;
            n->mj = m2;
            head = n;
        }
        else {
            node *tn = head;
            while (tn->next) tn = tn->next;
            n->mi = tn->mi;
            n->mj = tn->mj;
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
        node* current = head;
        while(current != NULL) {
            node* next = current->next;
            delete current;
            current = next;
        }
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

    void newFood(int r, int c, Snake &snake, Snake &snake2, char arr[20][50]) {
        do {
            fi = (rand() % (r - 2)) + 1;
            fj = (rand() % (c - 2)) + 1;
        } while (snake.onSnake(fi, fj) || snake2.onSnake(fi, fj) || arr[fi][fj] == '#');
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
    Snake snake2;
    Food food;
    Obstacle obstacle;

    GameBoard() {
        snake2.m1 = r - 2;
        snake2.m2 = c - 2;
        snake2.dx = 0;
        snake2.dy = -1;
        snake2.hch = '<';
        node* tn = snake2.head;
        while(tn) {
            tn->mi = snake2.m1;
            tn->mj = snake2.m2;
            tn = tn->next;
        }
    }

    void display() {
        string tempstr = "";
        if (c > 35)
            for (int i = 0; i < (c - 31); i++) tempstr += " ";
        else tempstr += "\n";

        cout << "P1 Score:" << snake.cs << "  P2 Score:" << snake2.cs << endl;
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
                        food.newFood(r, c, snake, snake2, arr);
                    } else if (snake2.m1 == food.fi && snake2.m2 == food.fj) {
                        snake2.insert(snake2.bch);
                        snake2.cs += 10;
                        food.newFood(r, c, snake, snake2, arr);
                    } else arr[i][j] = 'o';
                } else arr[i][j] = ' ';

                node *tn = snake.head;
                while (tn) {
                    arr[tn->mi][tn->mj] = tn->data;
                    tn = tn->next;
                }
                tn = snake2.head;
                while (tn) {
                    arr[tn->mi][tn->mj] = tn->data;
                    tn = tn->next;
                }
            }
        }
        obstacle.draw(arr);

        bool s1Hit = (snake.m1 == 0 || snake.m1 == r - 1) || (snake.m2 == 0 || snake.m2 == c - 1) || snake.hitSnake(snake.m1, snake.m2, arr) || snake2.onSnake(snake.m1, snake.m2);
        bool s2Hit = (snake2.m1 == 0 || snake2.m1 == r - 1) || (snake2.m2 == 0 || snake2.m2 == c - 1) || snake2.hitSnake(snake2.m1, snake2.m2, arr) || snake.onSnake(snake2.m1, snake2.m2);

        if (s1Hit || s2Hit) {
            handleGameOver(s1Hit, s2Hit);
        } else {
            arr[snake.m1][snake.m2] = snake.hch;
            arr[snake2.m1][snake2.m2] = snake2.hch;
        }
    }

    void handleGameOver(bool s1Hit, bool s2Hit) {
        if (s1Hit) { snake.m1 -= snake.dx; snake.m2 -= snake.dy; arr[snake.m1][snake.m2] = snake.hch; }
        if (s2Hit) { snake2.m1 -= snake2.dx; snake2.m2 -= snake2.dy; arr[snake2.m1][snake2.m2] = snake2.hch; }
        if (snake.cs > snake.hs) snake.hs = snake.cs;
        if (snake2.cs > snake2.hs) snake2.hs = snake2.cs;
        display();
        if (s1Hit && s2Hit) cout << "Game Over! Both players lost!\n" << endl;
        else if (s1Hit) cout << "Game Over! Player 1 lost!\n" << endl;
        else cout << "Game Over! Player 2 lost!\n" << endl;
        cout << "Press any key-Restart\nX-Exit\n";
        char c = 0;
        while (c == 0) c = getchNonBlocking();
        switch (c) {
            case 'x': case 'X': exit(0);
            default: 
                snake.reset(); snake2.reset();
                snake2.m1 = r - 2; snake2.m2 = c - 2; snake2.dx = 0; snake2.dy = -1; snake2.hch = '<'; 
                node* tn = snake2.head; while(tn) { tn->mi = snake2.m1; tn->mj = snake2.m2; tn = tn->next; }
                food.newFood(r, c, snake, snake2, arr);
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
        }
#ifdef _WIN32
        else if (ch == -32 || ch == 224) {
            switch (getchNonBlocking()) {
                case 72: snake.dx = -1; snake.dy = 0; snake.hch = '^'; break;
                case 80: snake.dx = 1;  snake.dy = 0; snake.hch = 'v'; break;
                case 77: snake.dx = 0;  snake.dy = 1; snake.hch = '>'; break;
                case 75: snake.dx = 0;  snake.dy = -1; snake.hch = '<'; break;
            }
        }
#endif
        else {
            switch (ch) {
                case 'w': case 'W': snake2.dx = -1; snake2.dy = 0; snake2.hch = '^'; break;
                case 'a': case 'A': snake2.dx = 0; snake2.dy = -1; snake2.hch = '<'; break;
                case 's': case 'S': snake2.dx = 1; snake2.dy = 0; snake2.hch = 'v'; break;
                case 'd': case 'D': snake2.dx = 0; snake2.dy = 1; snake2.hch = '>'; break;
            }
        }
    }

    void run() {
        while (true) {
            handleInput();
            snake.m1 += snake.dx;
            snake.m2 += snake.dy;
            snake.changeIndex();
            snake2.m1 += snake2.dx;
            snake2.m2 += snake2.dy;
            snake2.changeIndex();
            createGrid();
            display();
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }
};

int main() {
    setupConsole();
    GameBoard game;
    game.run();
    return 0;
}
