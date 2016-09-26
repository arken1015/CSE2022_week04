#include <glfw3.h>
#include <cstring>
#include <stdlib.h>		  // srand, rand
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include <math.h>

#define PI 3.14159

const int width = 800;
const int height = 600;

double xpos = 0;
double ypos = 0;
int cursor_pos;

float* pixels = new float[width*height * 3];
void drawPixel(const int& i, const int& j, const float& red, const float& green, const float& blue)
{
	pixels[(i + width* j) * 3 + 0] = red;
	pixels[(i + width* j) * 3 + 1] = green;
	pixels[(i + width* j) * 3 + 2] = blue;
}

inline float issame(int pos, const int num)
{
	if (num == pos) return 1;
	else return 0;
}

// scratched from https://courses.engr.illinois.edu/ece390/archive/archive-f2000/mp/mp4/anti.html
// see 'Rasterization' part.
// j0 =< j1
// i0 =< i1
void drawLine(const int& i0, const int& j0, const int& i1, const int& j1, const float& red, const float& green, const float& blue)
{
	if (i0 == i1)
	{
		for (int y = j0; y <= j1; y++)
		{
			drawPixel(i0, y, red, green, blue);
		}
		return;
	}

	if (j0 == j1)
	{
		for (int x = i0; x <= i1; x++)
		{
			drawPixel(x, j0, red, green, blue);
		}
		return;
	}

	for (int i = i0; i <= i1; i++)
	{
		const int j = (j1 - j0)*(i - i0) / (i1 - i0) + j0;

		drawPixel(i, j, red, green, blue);
	}
}

// x0 =< x1
// y0 =< y1
// filled: false->only border line; true->filled with colour
void drawSquare(const int& x0, const int& x1, const int& y0, const int& y1, const float& red, const float& green, const float& blue, bool filled)
{
	if (!filled)
	{
		for (int x = x0; x <= x1; x++)
		{
			drawPixel(x, y0, red, green, blue);
			drawPixel(x, y1, red, green, blue);
		}

		for (int y = y0; y <= y1; y++)
		{
			drawPixel(x0, y, red, green, blue);
			drawPixel(x1, y, red, green, blue);
		}
	}

	else
	{
		for (int x = x0; x <= x1; x++)
		{
			for (int y = y0; y <= y1; y++)
			{
				drawPixel(x, y, red, green, blue);
			}
		}
	}
}

// x0 =< x1
// y0 =< y1
void drawTriangle(const int& x0, const int& x1, const int& y0, const int& y1, const float& red, const float& green, const float& blue)
{
	const int half_x = (x1 + x0) / 2;
	for (int x = x0; x <= x1; x++)
	{
		drawPixel(x, y0, red, green, blue);
	}

	drawLine(x0, y0, half_x, y1, red, green, blue);
	drawLine(half_x, y1, x1, y0, red, green, blue);
}

// center position = (x,y)
void drawCircle(const int& x, const int& y, const int& r, const float& red, const float& green, const float& blue)
{
	float r_square = r * r;

	for (int i = -r; i <= 0; i++)
	{
		drawPixel(i + x, sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(i + x, -1 * sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(sqrt(r_square - i *i) + x, i + y, red, green, blue);
		drawPixel(-1 * sqrt(r_square - i *i) + x, i + y, red, green, blue);
	}

	for (int i = r; i >= 0; i--)
	{
		drawPixel(i + x, sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(i + x, -1 * sqrt(r_square - i*i) + y, red, green, blue);
		drawPixel(sqrt(r_square - i *i) + x, i + y, red, green, blue);
		drawPixel(-1 * sqrt(r_square - i *i) + x, i + y, red, green, blue);
	}
}

int circle[10][2] = { {100,200},{ 250,200 },{ 400,200 },{ 550,200 },{ 700,200 },{ 100,400 },{ 250,400 },{ 400,400 },{ 550,400 },{ 700,400 } };

void determine(int& pos)
{
	pos = 0;
	if ((int)ypos < 300) pos += 5;
	else pos = 0;

	pos += ((int)xpos - 50) / (int)150;

	if (pow((int)xpos - (circle[pos % 5][0]), 2) + pow((int)ypos - circle[(int)(pos / 5)][1],2) <= 2500);
	else pos = 11;
}

void drawOnPixelBuffer()
{

	determine(cursor_pos);

	//std::memset(pixels, 1.0f, sizeof(float)*width*height * 3); // doesn't work
	std::fill_n(pixels, width*height * 3, 1.0f);	// white background

													//for (int i = 0; i<width*height; i++) {
													//	pixels[i * 3 + 0] = 1.0f; // red 
													//	pixels[i * 3 + 1] = 1.0f; // green
													//	pixels[i * 3 + 2] = 1.0f; // blue
													//}

													//const int i = rand() % width, j = rand() % height;
													//drawPixel(i, j, 0.0f, 0.0f, 0.0f);

													// drawing a line
													//TODO: anti-aliasing

	//1st and 2nd
	int i0 = 75, i1 = 125;
	int j0 = 375, j1 = 425;
	for (int i = 0; i < 3; i++)
	{
		drawLine(i0 + i, j0, i1 + i, j1, 1.0f, 0.0f, 0.0f);
		drawCircle(250, 400, 14 + i, 1.0f, 0.0f, 0.0f);
	}

	//3rd
	drawSquare(380, 420, 380, 420, 1.0f, 0.0f, 0.0f, false);

	//4th
	drawLine(525, 375, 575, 425, 1.0f, 0.0f, 0.0f);
	drawLine(525, 425, 575, 375, 1.0f, 0.0f, 0.0f);

	//5th
	drawLine(700, 375, 700, 425, 1.0f, 0.0f, 0.0f);
	drawLine(680, 405, 700, 425, 1.0f, 0.0f, 0.0f);
	drawLine(700, 425, 720, 405, 1.0f, 0.0f, 0.0f);

	//6th
	drawLine(75, 200, 125, 200, 1.0f, 0.0f, 0.0f);
	drawLine(105, 220, 125, 200, 1.0f, 0.0f, 0.0f);
	drawLine(105, 180, 125, 200, 1.0f, 0.0f, 0.0f);

	//7th
	drawLine(240, 200, 260, 200, 1.0f, 0.0f, 0.0f);
	drawLine(231, 180, 250, 220, 1.0f, 0.0f, 0.0f);
	drawLine(250, 220, 269, 180, 1.0f, 0.0f, 0.0f);

	//8th
	drawLine(375, 200, 425, 200, 1.0f, 0.0f, 0.0f);

	//9th
	drawLine(525, 200, 575, 200, 1.0f, 0.0f, 0.0f);
	drawLine(525, 200, 545, 220, 1.0f, 0.0f, 0.0f);
	drawLine(525, 200, 545, 180, 1.0f, 0.0f, 0.0f);

	//10th
	drawLine(700, 175, 700, 225, 1.0f, 0.0f, 0.0f);
	drawLine(680, 195, 700, 175, 1.0f, 0.0f, 0.0f);
	drawLine(700, 175, 720, 195, 1.0f, 0.0f, 0.0f);

	//THOSE CIRCLES
	drawCircle(100, 400, 50, !issame(cursor_pos, 5), 0.0f, issame(cursor_pos, 5));
	drawCircle(250, 400, 50, !issame(cursor_pos, 6), 0.0f, issame(cursor_pos, 6));
	drawCircle(400, 400, 50, !issame(cursor_pos, 7), 0.0f, issame(cursor_pos, 7));
	drawCircle(550, 400, 50, !issame(cursor_pos, 8), 0.0f, issame(cursor_pos, 8));
	drawCircle(700, 400, 50, !issame(cursor_pos, 9), 0.0f, issame(cursor_pos, 9));

	drawCircle(100, 200, 50, !issame(cursor_pos, 0), 0.0f, issame(cursor_pos, 0));
	drawCircle(250, 200, 50, !issame(cursor_pos, 1), 0.0f, issame(cursor_pos, 1));
	drawCircle(400, 200, 50, !issame(cursor_pos, 2), 0.0f, issame(cursor_pos, 2));
	drawCircle(550, 200, 50, !issame(cursor_pos, 3), 0.0f, issame(cursor_pos, 3));
	drawCircle(700, 200, 50, !issame(cursor_pos, 4), 0.0f, issame(cursor_pos, 4));

	//drawPentagon(360, 120, 60, 0.0f, 0.0f, 0.0f);
	//Didn't work with Pentagon

	//TODO: try moving object

}

int main(void)
{

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glClearColor(1, 1, 1, 1); // while background

							  /* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		printf_s("X: %lf, Y: %lf, cursor_status: %3d\n", xpos, ypos, cursor_pos);
		/* Render here */
		//glClear(GL_COLOR_BUFFER_BIT);

		drawOnPixelBuffer();

		//TODO: RGB struct
		//Make a pixel drawing function
		//Make a line drawing function

		glfwGetCursorPos(window, &xpos, &ypos);

		glDrawPixels(width, height, GL_RGB, GL_FLOAT, pixels);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();

		system("cls");

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	glfwTerminate();

	delete[] pixels; // or you may reuse pixels array

	return 0;
}


