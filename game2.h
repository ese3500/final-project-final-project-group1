//game2, final name tbd

#include <stdbool.h>

int p1hp, p2hp;
int p1[2];
int p2[2];
int p1_vel[2];
int p2_vel[2];
bool p1_face_right = true;
bool p2_face_right = false;
int platforms[3][2];

int p1bullets_left, p2bullets_left;
Linked_List p1_bullets;
Linked_List p2_bullets;
int p1rt, p2rt;

//for reload timings
int g2_counter;
bool paused_g2 = false;

void draw_players_g2() {
	setColor_rgb(255, 151, 15);
	fillRect(p1[0]-6, p1[1]-10, p1[0]+6, p1[1]+10);
	fillRect(p1[0]-6, p1[1]-10, p1[0]+6, p1[1]+10);
	//gun
	if (p1_face_right) {
		fillRect(p1[0], p1[1] - 2, p1[0]+10, p1[1]-1);
	} else {
		fillRect(p1[0], p1[1] - 2, p1[0]-10, p1[1]-1);
	}
	setColor_rgb(15, 143, 255);
	fillRect(p2[0]-6, p2[1]-10, p2[0]+6, p2[1]+10);
	fillRect(p2[0]-6, p2[1]-10, p2[0]+6, p2[1]+10);
	//gun
	if (p2_face_right) {
		fillRect(p2[0], p2[1] - 2, p2[0]+10, p2[1]-1);
		} else {
		fillRect(p2[0], p2[1] - 2, p2[0]-10, p2[1]-1);
	}
}

void draw_platforms_g2() {
	setColor_rgb(255, 255, 255);
	drawHLine(platforms[0][0], platforms[0][1], 80);
	drawHLine(platforms[1][0], platforms[1][1], 80);
	drawHLine(platforms[2][0], platforms[2][1], 80);
}

void draw_hp_bars() {
	setColor_rgb(0, 0, 0);
	fillRect(0, 0, 319, 20);
	setColor_rgb(255, 255, 255);
	drawRect(5, 9, 155, 20);
	drawRect(165, 9, 315, 20);
	setColor_rgb(255, 0, 0);
	if (p1hp > 0) {
		int p1hpx = (p1hp/100.f)*149 + 5;
		fillRect(6, 10, p1hpx, 19);
	}
	if (p2hp > 0) {
		int p2hpx = 315 - (p2hp/100.f)*149;
		fillRect(p2hpx, 10, 314, 19);
	}
}

void setup_game2() {
	fillScr_rgb(0, 0, 0);
	fillScr_rgb(0, 0, 0);
	//draw p1 and p2
	p1[0] = 40;
	p1[1] = 229;
	p2[0] = 280;
	p2[1] = 229;
	draw_players_g2();
	
	//draw platforms
	platforms[0][0] = 40;
	platforms[0][1] = 170;
	platforms[1][0] = 200;
	platforms[1][1] = 170;
	
	platforms[2][0] = 120;
	platforms[2][1] = 100;
	draw_platforms_g2();
	
	//draw hp bars
	p1hp = 100;
	p2hp = 100;
	draw_hp_bars();
	
	g2_counter = 0;
	p1bullets_left = 6;
	p2bullets_left = 6;
	ll_clear(&p1_bullets);
	ll_clear(&p2_bullets);
	
}

int clamp(int val, int min, int max) {
	const int t = val < min ? min : val;
	return t > max ? max : t;
}

bool on_platform(int x, int y) {
	for (int i = 0; i < 3; i ++) {
		if (x >= platforms[i][0] && x <= platforms[i][0]+80) {
			if (y == platforms[i][1] - 11) {
				return true;
			}
		}
	}
	return false;
}
bool on_ground(int x_val, int y_val) {
	return on_platform(x_val, y_val) || y_val == 229;
}

void draw_bullets() {
	Node *curr = p1_bullets.head;
	setColor_rgb(0, 0, 0);
	while (curr) {
		//clear prev bullet
		fillRect(curr->xcoord-2, curr->ycoord-1, curr->xcoord+2, curr->ycoord+1);
		fillRect(curr->xcoord-2, curr->ycoord-1, curr->xcoord+2, curr->ycoord+1);
		curr = curr->next;
	}
	curr = p1_bullets.head;
	setColor_rgb(255, 151, 15);
	while (curr) {
		Node *next = curr->next;
		//oob
		curr->xcoord += curr->xvel;
		if (curr->xcoord-4 < 0 || curr->xcoord+4 > 319) {
			remove_node(&p1_bullets, curr);
		} else {
			fillRect(curr->xcoord-2, curr->ycoord-1, curr->xcoord+2, curr->ycoord+1);
		}
		curr = next;
	}
	
	curr = p2_bullets.head;
	setColor_rgb(0, 0, 0);
	while (curr) {
		//clear prev bullet
		fillRect(curr->xcoord-4, curr->ycoord-2, curr->xcoord+4, curr->ycoord+2);
		fillRect(curr->xcoord-4, curr->ycoord-2, curr->xcoord+4, curr->ycoord+2);
		curr = curr->next;
	}
	curr = p2_bullets.head;
	setColor_rgb(15, 143, 255);
	while (curr) {
		Node *next = curr->next;
		//oob
		curr->xcoord += curr->xvel;
		if (curr->xcoord-4 < 0 || curr->xcoord+4 > 319) {
			remove_node(&p2_bullets, curr);
			} else {
			fillRect(curr->xcoord-2, curr->ycoord-1, curr->xcoord+2, curr->ycoord+1);
		}
		curr = next;
	}
}

void update_game2() {
	//player movement
	//no x momentum
	p1_vel[0] = 0;
	p2_vel[0] = 0;
	if (paused_g2) {
		//X is restart button for both players
		if (P1Y || P2Y) {
			paused_g2 = false;
			setup_game2();
		}
		return;
	}
	
	
	if (P1LEFT) {
		p1_vel[0] -= 3;
		p1_face_right = false;
	} else if (P1RIGHT) {
		p1_vel[0] += 3;
		p1_face_right = true;
	}
	
	if (P1UP) {
		//if on ground/platform jump
		if (on_ground(p1[0], p1[1])) {
			p1_vel[1] -= 20;
		}
	}
	
	if (P2LEFT) {
		p2_vel[0] -= 3;
		p2_face_right = false;
	} else if (P2RIGHT) {
		p2_vel[0] += 3;
		p2_face_right = true;
	}
	
	if (P2UP) {
		//if on ground/platform jump
		if (on_ground(p2[0], p2[1])) {
			p2_vel[1] -= 20;
		}
	}
	
	//gravity
	p1_vel[1] = p1_vel[1] < 5 ? p1_vel[1]+1 : p1_vel[1];
	p2_vel[1] = p2_vel[1] < 5 ? p2_vel[1]+1 : p2_vel[1];
	
	//clear players
	setColor_rgb(0, 0, 0);
	fillRect(p1[0]-10, p1[1]-10, p1[0]+10, p1[1]+10);
	fillRect(p1[0]-10, p1[1]-10, p1[0]+10, p1[1]+10);
	fillRect(p2[0]-10, p2[1]-10, p2[0]+10, p2[1]+10);
	fillRect(p2[0]-10, p2[1]-10, p2[0]+10, p2[1]+10);
	
	p1[0] = clamp(p1[0]+p1_vel[0], 11, 309);
	p2[0] = clamp(p2[0]+p2_vel[0], 11, 309);
	
	//platform and ground checks for y
	int p1newy = p1[1] + p1_vel[1];
	int p2newy = p2[1] + p2_vel[1];
	for (int i = 0; i < 3; i ++) {
		if (p1[0] >= platforms[i][0] && p1[0] <= platforms[i][0] + 80) {
			if (p1_vel[1] > 0) {
				//down
				if (p1[1]+10 < platforms[i][1] && !(P1DOWN)) {
					p1newy = p1newy+10 > platforms[i][1] ? platforms[i][1]-11 : p1newy;
				}
			} else {
				//up
				if (p1[1]-10 > platforms[i][1]) {
					p1newy = p1newy-10 < platforms[i][1] ? platforms[i][1]+11 : p1newy;
				}
			}
		}
		
		//p2
		if (p2[0] >= platforms[i][0] && p2[0] <= platforms[i][0] + 80) {
			if (p2_vel[1] > 0) {
				//down
				if (p2[1]+10 < platforms[i][1] && !(P2DOWN)) {
					p2newy = p2newy+10 > platforms[i][1] ? platforms[i][1]-11 : p2newy;
				}
			} else {
				//up
				if (p2[1]-10 > platforms[i][1]) {
					p2newy = p2newy-10 < platforms[i][1] ? platforms[i][1]+11 : p2newy;
				}
			}
		}
	}
	
	p1[1] = clamp(p1newy, 32, 229);
	p2[1] = clamp(p2newy, 32, 229);
	
	if (g2_counter == p1rt) {
		p1bullets_left = 6;
	}
	if (g2_counter == p2rt) {
		p2bullets_left = 6;
	}
	
	
	//shooting
	if (P1X) {
		if (p1bullets_left > 0) {
			//spawn bullet
			int xvel = p1_face_right ? 10 : -10;
			push_front(&p1_bullets, p1[0], p1[1], xvel);
			p1bullets_left --;
			//reload timer starts after last shot bullet
			p1rt = g2_counter-1;
			if (p1rt == -1) p1rt = 99;
		}
	}
	
	if (P2X) {
		if (p2bullets_left > 0) {
			int xvel = p2_face_right ? 10 : -10;
			push_front(&p2_bullets, p2[0], p2[1], xvel);
			p2bullets_left --;
			p2rt = g2_counter-1;
			if (p2rt == -1) p2rt = 99;
		}
	}
	
	//check bullet collisions
	//if collide with hitbox of opponent remove bullet, -hp
	Node *bullet = p1_bullets.head;
	while (bullet) {
		Node *next = bullet->next;
		if (bullet->ycoord >= p2[1]-10 && bullet->ycoord <= p2[1]+10 && bullet->xcoord >= p2[0]-6 && bullet->xcoord <= p2[0]+6) {
			p2hp -= 6;
			remove_node(&p1_bullets, bullet);
			draw_hp_bars();
			sound_buzzer();
		}
		bullet = next;
	}
	
	bullet = p2_bullets.head;
	while (bullet) {
		Node *next = bullet->next;
		if (bullet->ycoord >= p1[1]-10 && bullet->ycoord <= p1[1]+10 && bullet->xcoord >= p1[0]-6 && bullet->xcoord <= p1[0]+6) {
			p1hp -= 6;
			remove_node(&p2_bullets, bullet);
			draw_hp_bars();
			sound_buzzer();
		}
		bullet = next;
	}
	
	//game over conditions
	if (p1hp <= 0 && p2hp <= 0) {
		paused_g2 = true;
		setBackColor_rgb(0, 0, 0);
		setColor_rgb(255, 255, 255);
		setFont(BigFont);
		print("It's a tie!", 75, 100, 0);
		sound_buzzer();
	} else if (p1hp <= 0) {
		paused_g2 = true;
		setBackColor_rgb(0, 0, 0);
		setColor_rgb(255, 255, 255);
		setFont(BigFont);
		print("Player 2 wins!", 50, 100, 0);
		sound_buzzer();
	} else if (p2hp <= 0) {
		paused_g2 = true;
		setBackColor_rgb(0, 0, 0);
		setColor_rgb(255, 255, 255);
		setFont(BigFont);
		print("Player 1 wins!", 50, 100, 0);
		sound_buzzer();
	}
	
	draw_platforms_g2();
	draw_bullets();
	draw_players_g2();
	
	g2_counter  = (g2_counter+1)%100;
}