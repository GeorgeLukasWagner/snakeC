#ifndef __SNAKE_H__
#  define __SNAKE_H__

#  include <stdbool.h>
#  include <stdint.h>
#  include <stdlib.h>
#  include <time.h>
#  include <stdio.h>
#  include <string.h>
#  include <ncurses.h>
#  include <unistd.h>

#define WIDTH 40
#define HEIGHT 20

typedef enum e_direction {
    UP,     // 0
    RIGHT,  // 1
    DOWN,   // 2
    LEFT    // 3
}   Direction;

typedef enum e_key {
    UP_KEY = 65,
    DOWN_KEY = 66,
    RIGHT_KEY = 67,
    LEFT_KEY = 68,
    Q_KEY = 113
}   Key;

typedef struct s_snake {
    int x;
    int y;
    struct s_snake *next;
}   Snake;

typedef struct s_food {
    int x;
    int y;
}   Food;

typedef struct s_info {
    Snake           *snake;
    Food            food;
    WINDOW          *win;
    unsigned int    score;
    unsigned int    size;
    unsigned int    speed;
    Direction       direction;
}   Info;

#endif // __SNAKE_H__