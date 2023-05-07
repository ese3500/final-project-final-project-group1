//snake/tron game
//display divided into grid with each square 5x5, total of 64x48
#include "linked_list.h"
#include <stdbool.h>

Linked_List p1_snake;
Linked_List p2_snake;
Node fruit;
bool paused = false;
long time_counter = 0;

//helper to check if cell is in any snake
bool cellInSnake(int x, int y) {
	Node *cur = p1_snake.head;
	while (cur) {
		if (x == cur->xcoord && y == cur->ycoord) {
			return true;
		}
		cur = cur->next;
	}
	Node *curp2 = p2_snake.head;
	while (curp2) {
		if (x == curp2->xcoord && y == curp2->ycoord) {
			return true;
		}
		curp2 = curp2->next;
	}
	return false;
}

//helper to draw snake
void draw_snake() {
	//we fill twice as theres sometimes a 1 pixel off visual glitch
	//orange p1
	Node *cur = p1_snake.head;
	setColor_rgb(255, 151, 15);
	while (cur) {
		fillRect(cur->xcoord*5, cur->ycoord*5, cur->xcoord*5+4, cur->ycoord*5+4);
		fillRect(cur->xcoord*5, cur->ycoord*5, cur->xcoord*5+4, cur->ycoord*5+4);
		cur = cur->next;
	}
	//blue p2
	Node *curp2 = p2_snake.head;
	setColor_rgb(15, 143, 255);
	while (curp2) {
		fillRect(curp2->xcoord*5, curp2->ycoord*5, curp2->xcoord*5+4, curp2->ycoord*5+4);
		fillRect(curp2->xcoord*5, curp2->ycoord*5, curp2->xcoord*5+4, curp2->ycoord*5+4);
		curp2 = curp2->next;
	}
}

void spawn_fruit() {
	//fruit spawn
	fruit = (Node){rand()%64, rand()%48, NULL, NULL};
	while (cellInSnake(fruit.xcoord, fruit.ycoord)) {
		fruit.xcoord = rand()%64;
		fruit.ycoord = rand()%48;
	}
}

void draw_fruit() {
	setColor_rgb(255, 0, 0);
	fillRect(fruit.xcoord*5, fruit.ycoord*5, fruit.xcoord*5+4, fruit.ycoord*5+4);
}

void reset_snake_game() {
	//clear background
	fillScr_rgb(0, 0, 0);
	//start as a 4 length snake taking tiles (40, 10) and horizontally onwards(+x)
	init_ll(&p1_snake);
	for (int i = 9; i >= 0; i --) {
		push_front(&p1_snake, 40+i, 10, 0);
	}
	//p2 snake starts at bottom left going right (24, 38)
	init_ll(&p2_snake);
	for (int i = 0; i < 10; i ++) {
		push_front(&p2_snake, 14+i, 38, 0);
	}
	//rng seeding
	int seed = 0;
	for (int i = 0; i < 16; i ++) {
		seed = seed<<1 | (ADC & 0b1);
	}
	srand(seed);
	
	spawn_fruit();
	//draw snake and fruits
	draw_snake();
	draw_fruit();
}


void update_snake_game() {
	if (paused) {
		//X is restart button for both players
		if (P1Y || P2Y) {
			paused = false;
			reset_snake_game();
		}
		return;
	}
	//A0/PF0 - UP
	//A1/PF1 - DOWN
	//A2/PF2 - LEFT
	//A3/PF3 - RIGHT
	int newx = p1_snake.head->xcoord;
	int newy = p1_snake.head->ycoord;
	//dont allow backwards movement
	Node *second_node = p1_snake.head->next;
	if (P1UP) {
		if (second_node->ycoord != newy-1) {
			newy --;
		}
	} else if (P1DOWN) {
		if (second_node->ycoord != newy+1) {
			newy ++;
		}
	} else if (P1LEFT) {
		if (second_node->xcoord != newx-1) {
			newx --;
		}
	} else if (P1RIGHT) {
		if (second_node->xcoord != newx+1) {
			newx ++;
		}
	}
	//if no input or a backwards input retain prev move dir
	if (newx == p1_snake.head->xcoord && newy == p1_snake.head->ycoord) {
		//retain motion in prev direction
		int deltaX = newx - second_node->xcoord;
		int deltaY = newy - second_node->ycoord;
		newx += deltaX;
		newy += deltaY;
	}
	
	//PF5 - RIGHT
	//PF6 - LEFT
	//PF7 - DOWN
	//PK0 - UP
	int newx2 = p2_snake.head->xcoord;
	int newy2 = p2_snake.head->ycoord;
	//dont allow backwards movement
	Node *second_node2 = p2_snake.head->next;
	if (P2UP) {
		if (second_node2->ycoord != newy2-1) {
			newy2 --; //UP
		}
	} else if (P2DOWN) {
		if (second_node2->ycoord != newy2+1) {
			newy2 ++; //DOWN
		}
	} else if (P2LEFT) {
		if (second_node2->xcoord != newx2-1) {
			newx2 --; //LEFT
		}
	} else if (P2RIGHT) {
		if (second_node2->xcoord != newx2+1) {
			newx2 ++; //RIGHT
		}
	}
	//if no input or a backwards input retain prev move dir
	if (newx2 == p2_snake.head->xcoord && newy2 == p2_snake.head->ycoord) {
		//retain motion in prev direction
		int deltaX = newx2 - second_node2->xcoord;
		int deltaY = newy2 - second_node2->ycoord;
		newx2 += deltaX;
		newy2 += deltaY;
	}
	
	bool p1_lose = (newx < 0 || newy < 0 || newx > 63 || newy > 47 || cellInSnake(newx, newy));
	bool p2_lose = (newx2 < 0 || newy2 < 0 || newx2 > 63 || newy2 > 47 || cellInSnake(newx2, newy2));
	
	//tie
	if (p1_lose && p2_lose) {
		paused = true;
		setBackColor_rgb(0, 0, 0);
		setColor_rgb(255, 255, 255);
		setFont(BigFont);
		print("It's a tie!", 75, 100, 0);
		sound_buzzer();
		return;
	//p1 loses
	} else if (p1_lose) {
		paused = true;
		setBackColor_rgb(0, 0, 0);
		setColor_rgb(255, 255, 255);
		setFont(BigFont);
		print("Player 2 wins!", 50, 100, 0);
		sound_buzzer();
		return;
	//p2 loses
	} else if (p2_lose) {
		paused = true;
		setBackColor_rgb(0, 0, 0);
		setColor_rgb(255, 255, 255);
		setFont(BigFont);
		print("Player 1 wins!", 50, 100, 0);
		sound_buzzer();
		return;
	}
	push_front(&p1_snake, newx, newy, 0);
	push_front(&p2_snake, newx2, newy2, 0);
	
	bool p1_fruit = (newx == fruit.xcoord && newy == fruit.ycoord);
	bool p2_fruit = (newx2 == fruit.xcoord && newy2 == fruit.ycoord);
	
	if (p1_fruit || p2_fruit) {
		sound_buzzer();
		spawn_fruit();
		draw_fruit();
	}
	if (!p1_fruit && time_counter % 10 != 9) {
		//erase tail block
		setColor_rgb(0, 0, 0);
		fillRect(p1_snake.tail->xcoord*5, p1_snake.tail->ycoord*5, p1_snake.tail->xcoord*5+4, p1_snake.tail->ycoord*5+4);
		fillRect(p1_snake.tail->xcoord*5, p1_snake.tail->ycoord*5, p1_snake.tail->xcoord*5+4, p1_snake.tail->ycoord*5+4);
		pop_back(&p1_snake);
	}
	if (!p2_fruit && time_counter % 10 != 9) {
		//erase tail block
		setColor_rgb(0, 0, 0);
		fillRect(p2_snake.tail->xcoord*5, p2_snake.tail->ycoord*5, p2_snake.tail->xcoord*5+4, p2_snake.tail->ycoord*5+4);
		fillRect(p2_snake.tail->xcoord*5, p2_snake.tail->ycoord*5, p2_snake.tail->xcoord*5+4, p2_snake.tail->ycoord*5+4);
		pop_back(&p2_snake);
	}
	draw_snake();
	time_counter ++;
}

















































































































































































































































