//
//  C++ lang
//  Created by this.aislan
//  Created with Visual Studio in Windows
//  Source: http://fabiensanglard.net/doom_fire_psx/index.html
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdlib.h>   

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define FIRE_HEIGHT 168
// FIRE_VELOCITY exists because the current processes are very fast
// The consecutive sum of this value must be an integer at some point
#define FIRE_VELOCITY 0.5

#define SPREAD_FIRE 3

#define PALETTE_COLORS 37

// Frames control
#define FRAME_TO_START_FIRE 0
#define FRAME_TO_KEEP_BURN 500
#define FRAME_TO_START_STOP_FIRE 300
#define FRAME_TO_FINISH_STOP_FIRE 400

struct Color {
	float r;
	float g;
	float b;
	float a;
};

void setInitialFrame(float *frame);
void populatePalette(Color *palette);
void populatePoints(int *points);
void handleFrame(float *frame, int *points, Color *palette);

void startFire(int *points);
void doFire(int *points);
void spreadFire(int src, int *points);
void showFire(int *points, Color *palette);
void stopFire(int *points);

int main(void) {
	float frame;
	GLFWwindow *window;
	Color palette[PALETTE_COLORS];
	int *points = new int[SCREEN_WIDTH * FIRE_HEIGHT];

	// Initializing the libs to use OpenGL
	if (!glfwInit()) { return EXIT_FAILURE; }

	window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Doom Fire", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	
	// If the libs have been correctly initialized, the other variables are initialized 
	setInitialFrame(&frame);
	populatePalette(palette);
	populatePoints(points);

	// OpenGL Settings
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glViewport(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Render loop
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Here is called method that handle the fire
		handleFrame(&frame, points, palette);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}

void stopFire(int *points) {
	for (int y = FIRE_HEIGHT - 1; y > 160; y--) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			if (points[y * SCREEN_WIDTH + x] > 0) {
				points[y * SCREEN_WIDTH + x] -= rand() % (SPREAD_FIRE + 1);
			}
		}
	}
}

// Magic method
void spreadFire(int src, int *points) {
	int point = points[src];

	if (point == 0) {
		points[src - SCREEN_WIDTH] = 0;
	} else {
		int randIdx = rand() % (SPREAD_FIRE + 1);
		int dst = src - randIdx + 1;
		points[dst - SCREEN_WIDTH] = point - (randIdx & 1);
	}
}

void doFire(int *points) {
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 1; y < FIRE_HEIGHT; y++) {
			spreadFire(y * SCREEN_WIDTH + x, points);
		}
	}
}

void startFire(int *points) {
	// Initializes the first line with the white color
	for (int i = 0; i < SCREEN_WIDTH; i++) { points[(FIRE_HEIGHT - 1) * SCREEN_WIDTH + i] = 36; }
}

void handleFrame(float *frame, int *points, Color *palette) {
	int exactFrame = std::floor(*frame);

	if (exactFrame == *frame) {
		if (exactFrame == FRAME_TO_START_FIRE) {
			startFire(points);
		} else if (exactFrame < FRAME_TO_KEEP_BURN) {
			if (exactFrame >= FRAME_TO_START_STOP_FIRE && exactFrame <= FRAME_TO_FINISH_STOP_FIRE) {
				stopFire(points);
			}
			doFire(points);
		} else {
			// Restart the animation
			setInitialFrame(frame);
		}
	}

	*frame += FIRE_VELOCITY;
	showFire(points, palette);
}

void showFire(int *points, Color *palette) {
	glEnable(GL_POINT_SMOOTH);
	glPointSize(1);
	glBegin(GL_POINTS);

	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < FIRE_HEIGHT; y++) {
			Color color = palette[points[y * SCREEN_WIDTH + x]];
			glColor4f(color.r, color.g, color.b, color.a);
			glVertex2f(SCREEN_WIDTH - x, FIRE_HEIGHT - y);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glEnd();
	glFlush();
}

void setInitialFrame(float *frame) {
	*frame = FIRE_VELOCITY * -1;
}

void populatePalette(Color *palette) {

	// RGB colors with alpha
	// RGB / 255 valores e arredondados para usar em glColor4f que usa um intervalo entre 0 e 1 por valor
	float rgbas[] = {
		0.027f, 0.027f, 0.027f, 0.0f,
		0.122f, 0.027f, 0.027f, 1.0f,
		0.184f, 0.059f, 0.027f, 1.0f,
		0.278f, 0.059f, 0.027f, 1.0f,
		0.341f, 0.09f, 0.027f, 1.0f,
		0.404f, 0.122f, 0.027f, 1.0f,
		0.467f, 0.122f, 0.027f, 1.0f,
		0.561f, 0.153f, 0.027f, 1.0f,
		0.624f, 0.184f, 0.027f, 1.0f,
		0.686f, 0.247f, 0.027f, 1.0f,
		0.749f, 0.278f, 0.027f, 1.0f,
		0.78f, 0.278f, 0.027f, 1.0f,
		0.875f, 0.31f, 0.027f, 1.0f,
		0.875f, 0.341f, 0.027f, 1.0f,
		0.875f, 0.341f, 0.027f, 1.0f,
		0.843f, 0.373f, 0.027f, 1.0f,
		0.843f, 0.373f, 0.027f, 1.0f,
		0.843f, 0.404f, 0.059f, 1.0f,
		0.812f, 0.435f, 0.059f, 1.0f,
		0.812f, 0.467f, 0.059f, 1.0f,
		0.812f, 0.498f, 0.059f, 1.0f,
		0.812f, 0.529f, 0.09f, 1.0f,
		0.78f, 0.529f, 0.09f, 1.0f,
		0.78f, 0.561f, 0.09f, 1.0f,
		0.78f, 0.592f, 0.122f, 1.0f,
		0.749f, 0.624f, 0.122f, 1.0f,
		0.749f, 0.624f, 0.122f, 1.0f,
		0.749f, 0.655f, 0.153f, 1.0f,
		0.749f, 0.655f, 0.153f, 1.0f,
		0.749f, 0.686f, 0.184f, 1.0f,
		0.718f, 0.686f, 0.184f, 1.0f,
		0.718f, 0.718f, 0.184f, 1.0f,
		0.718f, 0.718f, 0.216f, 1.0f,
		0.812f, 0.812f, 0.435f, 1.0f,
		0.875f, 0.875f, 0.624f, 1.0f,
		0.937f, 0.937f, 0.78f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	for (int i = 0; i < PALETTE_COLORS; i++) {
		palette[i].r = rgbas[i * 4 + 0];
		palette[i].g = rgbas[i * 4 + 1];
		palette[i].b = rgbas[i * 4 + 2];
		palette[i].a = rgbas[i * 4 + 3];
	}
}

void populatePoints(int *points) {
	// Populate all points with black
	// Black should be transparent to allow see the Doom Logo behind
	for (int i = 0; i < SCREEN_WIDTH * FIRE_HEIGHT; i++) { points[i] = 0; }
}