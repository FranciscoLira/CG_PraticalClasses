#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <string>
using namespace std;

/*
	INTEL
SIDES                    |      4096 |     16384 |     65536 |    262144 |
Immediate mode           |   120/190 |        60 |     11/13 |       4/5 |
VBOs (vertices)          |   275/440 |    70/161 |     17/36 |      7/10 |
VBOs (vert and indices)  |           |           |           |           |
	NVIDIA
SIDES                    |      4096 |     16384 |     65536 |    262144 |
Immediate mode           |       223 |        90 |        23 |         6 |
VBOs (vertices)          |       240 |   130/121 |     37/55 |      9/14 |
VBOs (vert and indices)  |           |           |           |           |

*/

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLuint buffers[1];
int sides = 4096;
int qvertex = sides * 4 * 3;
float *vertexB = (float*)malloc(sizeof(float) * qvertex * 3);
int frame,fps;
int currtime, timebase;


void spherical2Cartesian() {
	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if (h == 0)
		h = 1;

	// compute window's aspect ratio
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f , ratio, 1.0f , 1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


/*-----------------------------------------------------------------------------------
	Draw Cylinder with strips and fans

	  parameters: radius, height, sides

-----------------------------------------------------------------------------------*/


void cylinder0(float radius, float height, int sides) {

	int i;
	float step;

	step = 360.0 / sides;

	glColor3f(1, 0, 0);
	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(0, height * 0.5, 0);
	for (i = 0; i <= sides; i++) {
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
	}
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(0, -height * 0.5, 0);
	for (i = 0; i <= sides; i++) {
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height * 0.5, sin(i * step * M_PI / 180.0)*radius);
	}
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLE_STRIP);

	for (i = 0; i <= sides; i++) {
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
	}
	glEnd();
}


/*-----------------------------------------------------------------------------------
	Draw Cylinder

		parameters: radius, height, sides

-----------------------------------------------------------------------------------*/


void cylinder(float radius, float height, int sides) {

	int i;
	float step;

	step = 360.0 / sides;

	glBegin(GL_TRIANGLES);

	// top
	for (i = 0; i < sides; i++) {
		glVertex3f(0, height * 0.5, 0);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height * 0.5, -sin((i + 1) * step * M_PI / 180.0)*radius);
	}

	// bottom
	for (i = 0; i < sides; i++) {
		glVertex3f(0, -height * 0.5, 0);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height * 0.5, -sin((i + 1) * step * M_PI / 180.0)*radius);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
	}

	// body
	for (i = 0; i <= sides; i++) {

		glVertex3f(cos(i * step * M_PI / 180.0)*radius, height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height * 0.5, -sin((i + 1) * step * M_PI / 180.0)*radius);

		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height * 0.5, -sin(i * step * M_PI / 180.0)*radius);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height * 0.5, -sin((i + 1) * step * M_PI / 180.0)*radius);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height * 0.5, -sin((i + 1) * step * M_PI / 180.0)*radius);
	}
	glEnd();
}

/*
	Draw a cylinder with VBO vertices
*/

void cylinder1(float radius, float height, int sides) {

	int i;
	int qvertex;
	float step;
	int vbi = 0;
	step = 360.0 / sides;

	// top
	for (i = 0; i < sides; i++) {
		vertexB[vbi++] = 0;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = 0;

		vertexB[vbi++] = cos(i * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0) * radius;

		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0) * radius;
	}


	// bottom
	for (i = 0; i < sides; i++) {
		vertexB[vbi++] = 0;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = 0;

		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0) * radius;

		vertexB[vbi++] = cos(i * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0) * radius;
	}

	// body
	for (i = 0; i < sides; i++) {
		vertexB[vbi++] = cos(i * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0) * radius;

		vertexB[vbi++] = cos(i * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0) * radius;

		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0) * radius;


		vertexB[vbi++] = cos(i * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0) * radius;

		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0) * radius;

		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0) * radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0) * radius;
	}
}

void menu() {

    int q = 1;
    string toPrint[q] = { "FPS:" + std::to_string(fps) };

    int len[q];
    for (int i = 0; i < q; i++) {
        len[i] = toPrint[i].length();
    }

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 1280, 0, 1024);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    for (int j = 0; j < q; j++) {
        glRasterPos2i(20, 1000 - (j * 20));
        for (int i = 0; i < len[j]; ++i) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, toPrint[j][i]);
        }
    }
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

};

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
	          0.0, 0.0, 0.0,
	          0.0f, 1.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, qvertex);

	cylinder(1, 2, sides); //this is for immediate mode

	frame++;
	currtime = glutGet(GLUT_ELAPSED_TIME);
	if (currtime - timebase > 1000) {
		fps = frame * 1000.0 / (currtime - timebase);
		timebase = currtime;
		frame = 0;
	}

	// End of frame
	menu();
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

	switch (c){

		case 'm':
			sides *=4;
			printf("%d\n", sides);
			break;
		case 'n':
			sides /=4;
			printf("%d\n", sides);
			break;
	}

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("CG@DI-UM");

// Required callback registry
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

#ifndef __APPLE__
	glewInit();
#endif

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	spherical2Cartesian();
	
	//cylinder1(1,2,sides);

	printInfo();

	// This is for VBOs
	// glEnableClientState(GL_VERTEX_ARRAY);
	// glGenBuffers(1, buffers);
	// glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(float)*qvertex * 3, vertexB, GL_STATIC_DRAW);

// enter GLUT's main cycle
	glutMainLoop();

	return 1;
}


/*

glGenBuffer(1, &b); b é um int

glBindBuffer(GL_ARRAY_BUFFER, b); o mesmo b de cima

glBufferData(GL_ARRAY_BUFFER,dimArray, vertex3); dimArray em bytes

glVertexPointer(3,float,0,0);

glDrawArray(GL_TRIANGLES,0,count);


vblank_mode=0 primusrun ./class4

*/