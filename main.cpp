#include <iostream>
#include <cstdlib>
#include <ncurses.h>
#include <deque>

using namespace std;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct SnakeSegment {
    int x, y;
};

class SnakeGame {
public:
    SnakeGame();
    ~SnakeGame();
    void Run();

private:
    void InitGame();
    void Draw();
    void Input();
    void Logic();

    bool IsCollision();
    void PlaceFood();

    int width, height;
    int score;
    bool gameover;
    Direction direction;
    deque<SnakeSegment> snake;
    SnakeSegment food;
};

SnakeGame::SnakeGame() {
    initscr();            // Initialize the ncurses library
    raw();                // Disable line buffering
    keypad(stdscr, TRUE); // Enable special keys
    noecho();             // Don't display input characters
    curs_set(0);          // Hide the cursor

    getmaxyx(stdscr, height, width); // Get the window size
    InitGame();
}

SnakeGame::~SnakeGame() {
    endwin(); // Clean up the ncurses library
}

void SnakeGame::InitGame() {
    score = 0;
    gameover = false;
    direction = RIGHT;

    snake.clear();
    snake.push_front({width / 2, height / 2}); // Initial snake position

    PlaceFood();
}

void SnakeGame::Draw() {
    clear();

    // Draw snake
    for (const auto &segment : snake) {
        mvprintw(segment.y, segment.x, "O");
    }

    // Draw food
    mvprintw(food.y, food.x, "X");

    // Draw score
    mvprintw(height, 0, "Score: %d", score);

    refresh();
}

void SnakeGame::Input() {
    int key = getch(); // Get key input

    switch (key) {
        case KEY_UP:
            if (direction != DOWN)
                direction = UP;
            break;
        case KEY_DOWN:
            if (direction != UP)
                direction = DOWN;
            break;
        case KEY_LEFT:
            if (direction != RIGHT)
                direction = LEFT;
            break;
        case KEY_RIGHT:
            if (direction != LEFT)
                direction = RIGHT;
            break;
        case 27: // ASCII code for ESC
            gameover = true;
            break;
    }
}

void SnakeGame::Logic() {
    // Move snake
    int headX = snake.front().x;
    int headY = snake.front().y;

    switch (direction) {
        case UP:
            headY--;
            break;
        case DOWN:
            headY++;
            break;
        case LEFT:
            headX--;
            break;
        case RIGHT:
            headX++;
            break;
    }

    // Check collision
    if (headX < 0 || headX >= width || headY < 0 || headY >= height || IsCollision()) {
        gameover = true;
        return;
    }

    // Check if snake eats food
    if (headX == food.x && headY == food.y) {
        score++;
        snake.push_front({food.x, food.y});
        PlaceFood();
    } else {
        // Move the snake
        snake.push_front({headX, headY});
        snake.pop_back();
    }
}

bool SnakeGame::IsCollision() {
    // Check if the snake collides with itself
    for (auto it = snake.begin() + 1; it != snake.end(); ++it) {
        if (it->x == snake.front().x && it->y == snake.front().y) {
            return true;
        }
    }

    return false;
}

void SnakeGame::PlaceFood() {
    // Randomly place food on the screen
    food.x = rand() % width;
    food.y = rand() % height;
}

void SnakeGame::Run() {
    while (!gameover) {
        Draw();
        Input();
        Logic();
        usleep(100000); // Sleep for 100 milliseconds
    }

    mvprintw(height / 2, width / 2 - 5, "Game Over");
    refresh();
    getch(); // Wait for a key press before exiting
}

int main() {
    SnakeGame game;
    game.Run();

    return 0;
}
