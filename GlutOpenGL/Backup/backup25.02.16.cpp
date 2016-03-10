#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>
#include <GL/glut.h>
#include <Windows.h>
#include "iniParser.h"

int DELAY = 100;
int NUM = 3;
int N = 30, M = 20, Scale = 25;
int Start_x = 5, Start_y = 0;
int Max = 100;
int StBarHeight = Scale * 2;

const int MaxSnSize = 100;
const int Apples = 15;
int w = Scale * N;
int h = Scale * M;
int dir = 0;
bool allowKey = true;

struct Snake
{
	int x, y;
} s[MaxSnSize];

class Fruits
{
	int x, y;

public:

	void New()
	{
		int x_fut = 0, y_fut = 0;
		bool rep = false;

		while(true) // Apples wont appear on the snake
		{
			x_fut = rand() % N;
			y_fut = rand() % M;

			rep = false;

			for (int i = 0; i < Apples; i++) // All apples have unique coordinates
				if (m[i].GetX() == x_fut && m[i].GetY() == y_fut) {
					rep = true;
					break;
				}

			if (rep) continue;
			
			for (int i = 0; i < NUM; i++) // Apples don't place on snake
				if (s[i].x == x_fut && s[i].y == y_fut) {
					rep = true;
					break;
				}

			if (!rep) break;
		}

		x = x_fut;
		y = y_fut;
	}

	void DrawApple()
	{
		glColor3f(0.133333, 0.545098, 0.133333);
		glRectf((x + 0.15) * Scale, (y + 0.15) * Scale, (x + 0.85) * Scale, (y + 0.85) * Scale);
	}

	int GetX() { return x; }
	int GetY() { return y; }

} m[Apples];

void drawSnake()
{
	glColor3f(0.0, 0.0, 1.0);

	for (int i = 0; i < NUM; i++) {
		glRectf((s[i].x + 0.1) * Scale, (s[i].y + 0.1) * Scale, (s[i].x + 0.9) * Scale, (s[i].y + 0.9) * Scale);
	}

	glColor3f(0.972549, 0.972549, 1); // Eyes

	glRectf((s[0].x + 0.2) * Scale, (s[0].y + 0.4) * Scale, (s[0].x + 0.4) * Scale, (s[0].y + 0.6) * Scale);
	glRectf((s[0].x + 0.6) * Scale, (s[0].y + 0.4) * Scale, (s[0].x + 0.8) * Scale, (s[0].y + 0.6) * Scale);
}

void drawStatusBar()
{
	glColor3f(0, 0, 0);
	glRectf(0, h + 1, w, h + StBarHeight); // h + 1 for make visible top edge of the field grid
}

void tick()
{
	// MOVE

	for (int i = NUM; i > 0; --i) { // Copy all values into previous cells
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}
	
	if (s[0].x == N - 1 || s[0].x == 0 || s[0].y == M - 1 || s[0].y == 0) // If edge is reached
	{
		if  ( // If s[0] - one of the corners(two rules are suitable)
			(s[0].x == 0 && s[0].y == 0)		||
			(s[0].x == N - 1 && s[0].y == 0)	||
			(s[0].x == N - 1 && s[0].y == M - 1)||
			(s[0].x == 0 && s[0].y == M - 1)     )
		{
			if (s[0].x == 0 && s[0].y == 0)
			{
				if (dir == 1) s[0].x = N - 1; // appears right
				else if (dir == 3) s[0].y = M - 1; // appears up
				else goto move;
			}
			else if (s[0].x == N - 1 && s[0].y == 0)
			{
				if (dir == 2) s[0].x = 0; // appears left
				else if (dir == 3) s[0].y = M - 1; // appears up
				else goto move;
			}
			else if (s[0].x == N - 1 && s[0].y == M - 1)
			{
				if (dir == 2) s[0].x = 0; // appears left
				else if (dir == 0) s[0].y = 0; // appears down
				else goto move;
			}
			else if (s[0].x == 0 && s[0].y == M - 1)
			{
				if (dir == 0) s[0].y = 0; // appears down
				else if (dir == 1) s[0].x = N - 1; // appears right
				else goto move;
			}
		} // If s[0] is on one of the sides
		else if (s[0].x == N - 1)
			if (dir == 2) s[0].x = 0; // appears left
			else goto move;
		else if (s[0].x == 0) 
			if (dir == 1) s[0].x = N - 1; // appears right
			else goto move;
		else if (s[0].y == M - 1) 
			if (dir == 0) s[0].y = 0; // appears down
			else goto move;
		else if (s[0].y == 0) 
			if (dir == 3) s[0].y = M - 1; // appears up
			else goto move;
	}
	else
	{
		// Direction
		move:
		if (dir == 0) s[0].y += 1; // up
		else if (dir == 1) s[0].x -= 1; // left
		else if (dir == 2) s[0].x += 1; // right
		else if (dir == 3) s[0].y -= 1; // down
	}

	// SNAKE EATS

	for (int i = 0; i < Apples; i++)
		if (s[0].x == m[i].GetX() && s[0].y == m[i].GetY())
		{
			NUM++;
			m[i].New();
			break;
		}

	for (int i = 1; i < NUM; i++) // Sanke eats it tail
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			NUM = i;
			break;
		}
}

void drawField()
{
	glColor3f(0.0, 0.7, 0.0);

	glLineWidth(1.0f);
	
	glBegin(GL_LINES);

	for (int i = 0; i < w; i += Scale) {
		glVertex2f(i, 0);
		glVertex2f(i, h);
	}
	for (int j = 0; j <= h; j += Scale) {
		glVertex2f(0, j);
		glVertex2f(w, j);
	}

	glEnd();
}

void draw_string(void *font, const char* string, float x, float y, float AlphaW, float AlphaH)

{
	glLineWidth(3.0f);

	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();

	glTranslatef(x, y, 0.0f); // Position

	glScalef(AlphaW, AlphaH, 1.0f); // Font size

	while (*string)
		glutStrokeCharacter(font, *string++);

	glPopMatrix();

	glFinish();
}

void display() {

	glClear(GL_COLOR_BUFFER_BIT);

	drawField();

	drawStatusBar();

	drawSnake();

	for (int i = 0; i < Apples; i++)
		m[i].DrawApple();

	char temp[15], targ_s[80], *template1 = "Blocks: ", *template2 = "Max: "; // Drawing scores

	_itoa_s(NUM, temp, 15, 10); // if not Windows - delete "_" and "_s"

	strcpy_s(targ_s, template1);

	strcat_s(targ_s, temp);

	draw_string(GLUT_STROKE_ROMAN, targ_s, 5.0, h + StBarHeight - 40, 0.3, 0.3); // Current blocks number

	_itoa_s(Max, temp, 15, 10);

	strcpy_s(targ_s, template2);

	strcat_s(targ_s, temp);

	if (Max >= 100) draw_string(GLUT_STROKE_ROMAN, targ_s, w - 180, h + StBarHeight - 40, 0.3, 0.3); // Right align of "Max scores"
	else if (Max >= 10 && Max <= 99) draw_string(GLUT_STROKE_ROMAN, targ_s, w - 155, h + StBarHeight - 40, 0.3, 0.3);
	else if (Max < 10) draw_string(GLUT_STROKE_ROMAN, targ_s, w - 135, h + StBarHeight - 40, 0.3, 0.3);

	glFlush();
	glutSwapBuffers();
}

void keys(int key, int x, int y)
{
	if (allowKey)
	{
		switch (key) // Set direction if not 180 || 0 degrees 
		{
		case 101: // up
			if (dir != 3 && dir != 0) {
				dir = 0;
				allowKey = false;
			}
			break;
		case 102: // right
			if (dir != 1 && dir != 2) {
				dir = 2;
				allowKey = false;
			}
			break;
		case 100: // left 
			if (dir != 2 && dir != 1) {
				dir = 1;
				allowKey = false;
			}
			break;
		case 103: // down 
			if (dir != 0 && dir != 3) {
				dir = 3;
				allowKey = false;
			}
			break;
		}
	}
}

void keys_ASCII(unsigned char key, int x, int y)
{
	if (key == 27) // Escape -> Exit
	{
		exit(0);
	}
}

void timer(int = 0)
{
	display();

	tick();

	int temp = 0;
	
	std::ifstream file_read("max_score.txt", std::ios_base::in); // Check MAX scores
	if (file_read) file_read >> temp;
	file_read.close();

	if (NUM > temp) {
		std::ofstream file_write("max_score.txt", std::ios_base::out, std::ios_base::trunc);
		if (file_write) file_write << NUM;
		file_write.close();
	}

	allowKey = true; // Control of pressed keys; One tick - one acceptable key

	glutTimerFunc(DELAY, timer, 0); // Recursive call
}

int main(int argc, char **argv) {

	srand(time(0));

	cout << parse_settings("delay") << "\n";
	cout << parse_settings("start_x_position") << "\n";
	cout << parse_settings("scale") << "\n";

	dir = rand() % 4; // Random direction

	for (int i = 0; i < NUM; i++) { // Start snake cells position
		s[i].x = Start_x;
		s[i].y = Start_y;
	}

	for (int i = 0; i < Apples; i++) { // Initialisation of apples
		m[i].New();
	}

	std::ifstream file_read("max_score.txt", std::ios_base::in); // Read max scores from last games
	if (file_read) file_read >> Max;
	file_read.close();

	HWND console = GetConsoleWindow(); // Console hide
	//ShowWindow(console, SW_HIDE);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h + StBarHeight);
	glutCreateWindow("S.N.A.K.E");
	//102, 84, 29, 1
	glClearColor(0, 0, 0, 1); // Background color
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h + StBarHeight);

	glutDisplayFunc(display);
	glutSpecialFunc(keys);
	glutKeyboardFunc(keys_ASCII);
	glutTimerFunc(DELAY, timer, 0);

	glutMainLoop();
}