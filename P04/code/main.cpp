#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

GLuint buffers[1];
int sides = 100;
int qvertex = sides * 4*3;
float *vertexB = (float*)malloc(sizeof(float) * qvertex * 3);


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
		
		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = -height*0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0)*radius;
		
		vertexB[vbi++] = cos(i * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0)*radius;
	}

	// body
	for (i = 0; i < sides; i++) {
		vertexB[vbi++] = cos(i * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0)*radius;
		
		vertexB[vbi++] = cos(i * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0)*radius;
		
		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0)*radius;


		vertexB[vbi++] = cos(i * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin(i * step * M_PI / 180.0)*radius;

		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = -height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0)*radius;

		vertexB[vbi++] = cos((i + 1) * step * M_PI / 180.0)*radius;
		vertexB[vbi++] = height * 0.5;
		vertexB[vbi++] = -sin((i + 1) * step * M_PI / 180.0)*radius;
	}
}


void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
	          0.0, 0.0, 0.0,
	          0.0f, 1.0f, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);

	glDrawArrays(GL_TRIANGLES,0, qvertex);


	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

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
	cylinder1(1, 2, sides);


	printInfo();

	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1,buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(float)*qvertex*3, vertexB,GL_STATIC_DRAW);

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







*/