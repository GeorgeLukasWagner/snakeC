#include "snake.h"

Snake	*new_snake(int x, int y) {
	Snake *snake = malloc(sizeof(Snake));
	if (!snake) {
		perror("malloc");
		exit(1);
	}
	snake->x = x;
	snake->y = y;
	snake->next = NULL;
	return snake;
}

Snake	*snake_last(Snake *snake) {
	Snake *last = snake;
	while (last->next != NULL) {
		last = last->next;
	}
	return last;
}

void	snake_add(Snake **snake, Snake *snake_part) {
	Snake *new;
	if (*snake) {
		new = snake_last(*snake);
		new->next = snake_part;
	} else {
		*snake = snake_part;
		(*snake)->next = NULL;
	}
}

void	destruct_info(Info *info) {
	Snake *part = info->snake;
	Snake *next;
	while (part) {
		next = part->next;
		free(part);
		part = next;
	}
}

bool	self_collision(Snake *snake) {
	Snake *head = snake;
	if (!head->next) {
		return false;
	}
	Snake *part = snake->next;
	while (part) {
		if (head->x == part->x && head->y == part->y) {
			return true;
		}
		part = part->next;
	}
	return false;
}

bool	wall_collision(Snake *snake) {
	Snake *head = snake;
	if (head->x < 0 || head->x >= WIDTH || head->y < 0 || head->y >= HEIGHT) {
		return true;
	}
	return false;
}

bool	check_valid_food(Info *info) {
	Snake *part = info->snake;
	while (part) {
		if (part->x == info->food.x && part->y == info->food.y) {
			return false;
		}
		part = part->next;
	}
	return true;
}

void	make_new_food(Info *info) {
	while (!check_valid_food(info)) {
		info->food.x = rand() % WIDTH;
		info->food.y = rand() % HEIGHT;
	}
}

void get_direction(Info *info) {
	chtype key = wgetch(info->win);

	switch (key) {
		case UP_KEY:
		if (info->direction != DOWN) {
			info->direction = UP;
		}
		break;
		case DOWN_KEY:
		if (info->direction != UP) {
			info->direction = DOWN;
		}
		break;
		case RIGHT_KEY:
		if (info->direction != LEFT) {
			info->direction = RIGHT;
		}
		break;
		case LEFT_KEY:
		if (info->direction != RIGHT) {
			info->direction = LEFT;
		}
		break;
		case Q_KEY:
		endwin();
		destruct_info(info);
		exit(0);
	}
}

bool	snake_eat(Info *info) {
	Snake *head = info->snake;
	if (head->x == info->food.x && head->y == info->food.y) {
		info->score += 10;
		info->size++;
		snake_add(&info->snake, new_snake(info->food.x, info->food.y));
		return true;
	}
	return false;
}

void	print_border() {
	for (int i = 0; i <= WIDTH; i++) {
		mvprintw(21, i, "@");
	}
	for (int i = 0; i <= HEIGHT; i++) {
		mvprintw(i, 41, "@");
	}
}

void	redraw(Info *info) {
	clear();
	Snake *part = info->snake;
	while (part) {
		mvprintw(part->y, part->x, "O");
		part = part->next;
	}
	mvprintw(info->food.y, info->food.x, "x");
	mvprintw(22, 0, "Score: %d", info->score);
	print_border();
	refresh();
}

void	update_snake(Info *info) {
	Snake *part = info->snake;
	int x = part->x;
	int y = part->y;
	if (info->direction == UP) {
		part->y--;
	} else if (info->direction == DOWN) {
		part->y++;
	} else if (info->direction == RIGHT) {
		part->x++;
	} else if (info->direction == LEFT) {
		part->x--;
	}
	if (info->size == 1) {
		return;
	}
	Snake *next = part->next;
	int tmp_x;
	int tmp_y;
	while (next) {
		tmp_x = next->x;
		tmp_y = next->y;
		next->x = x;
		next->y = y;
		x = tmp_x;
		y = tmp_y;
		next = next->next;
	}
}

void	game_loop(Info *info) {
	wrefresh(info->win);
	while (1) {
		if (self_collision(info->snake) || wall_collision(info->snake)) {
			break;
		}
		if (snake_eat(info)) {
			make_new_food(info);
		}
		get_direction(info);
		update_snake(info);
		redraw(info);
		if (info->direction == UP || info->direction == DOWN)
			usleep(100000 / info->speed);
		else
			usleep(50000 / info->speed);
	}
}

void	init_info(Info *info) {
	info->snake = new_snake(0, 0);
	info->win = initscr();
	info->size = 1;
	info->score = 0;
	info->speed = 1;
	info->food.x = 0;
	info->food.y = 0;
	make_new_food(info);
	info->direction = RIGHT;
	halfdelay(1);
}



int	main() {
	Info	info;
	
	init_info(&info);
	clear();
	refresh();
	game_loop(&info);
	destruct_info(&info);
	endwin();
	printf("Game over! Your score was %d\n", info.score);
}