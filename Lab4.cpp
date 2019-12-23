// window
#include "stdafx.h"

// standard
#include <assert.h>
#include <math.h>

// glut
#include <GL/glut.h>

#include "vec3.h"
#include "quanternion.h"
#include "boid.h"
#include "boids.h"

// screen size
int g_screenWidth = 0;
int g_screenHeight = 0;

static int points = 0; 
int number_point=5;

static GLfloat t = 0;
GLfloat incret=0.01;
int number_boids = 20;

static boid boids[20];

static quanternion q_points[5]={
	quanternion(1,vec3(0,0,0)),
	quanternion(0,vec3(1,0,0)),
	quanternion(0,vec3(0,1,0)),
	quanternion(0,vec3(0,0,1)),
	quanternion(0,vec3(0,1,0)),
};

static vec3 pos_points[5]={
	vec3(0,-5,0),
	vec3(3,2,4),
	vec3(-5,0,-5),
	vec3(5,-3,7),
	vec3(-4,-6,3),
};

GLfloat b=1.0/6.0;
GLfloat b_spline[4][4]={
	{-b,3*b,-3*b,b},
	{3*b,-6*b,3*b,0},
	{-3*b,0,3*b,0},
	{b,4*b,b,0}} ;

void init()
{
	for (int i=0;i< number_boids;i++)
		boids[i]=boid(vec3(rand()%10,rand()%10,rand()%10));
}

quanternion interpolation(GLfloat t)
{
	vec3 pos;
	for (int i=0;i<4;i++)
	{
		int index=(i+points)%5;
		for (int j=0;j<4;j++)
		{
			pos.x+=b_spline[j][i]*pow(t,3-j)*pos_points[index].x;
			pos.y+=b_spline[j][i]*pow(t,3-j)*pos_points[index].y;
			pos.z+=b_spline[j][i]*pow(t,3-j)*pos_points[index].z;
		}
	}
	quanternion q1=q_points[points];
	quanternion q2=q_points[(points+1)%number_point];
	GLfloat q_angle=acos(q1.dot(q2));
	quanternion q=q1*(sin((1-t)*q_angle)/sin(q_angle))+q2*(sin(q_angle*t)/sin(q_angle));
	q.updatematrix(pos);
	return q;
}

void timer(int value) {
	// render
	glutPostRedisplay();
	// Set time increase by 0.01
	t += incret;
	if (t >= 1)
	{
		t = 0;
		if (points < number_point)
			points++;
		else
			points = 0;
	}
	// reset timer
	glutTimerFunc(16, timer, 0);
}

void render(void) {
	// clear buffer
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render state
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// light source attributes
	GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat LightSpecular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightPosition[] = { 5.0f, 5.0f, 5.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	// surface material attributes
	GLfloat material_Ka[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	GLfloat material_Kd[] = { 0.43f, 0.47f, 0.54f, 1.0f };
	GLfloat material_Ks[] = { 0.33f, 0.33f, 0.52f, 1.0f };
	GLfloat material_Ke[] = { 0.1f, 0.0f, 0.1f, 1.0f };
	GLfloat material_Se = 10;

	glMaterialfv(GL_FRONT, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT, GL_SHININESS, material_Se);
	// modelview matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, -10.0, -10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	// animation	
	quanternion inter_q= interpolation(t);
	glPushMatrix();
	glMultMatrixf(inter_q.matrix);
	glutSolidOctahedron();
	glPopMatrix();
	boidsmove(boids, inter_q.pos);
	// disable lighting
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	// swap back and front buffers
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch(key)
	{
		case 'a':
		incret=0.01;
		break;
	}
}

void reshape(int w, int h) {
	// screen size
	g_screenWidth = w;
	g_screenHeight = h;

	// viewport
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	// projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, (GLfloat)w / (GLfloat)h, 1.0, 50.0);
}

int main(int argc, char** argv) {
	// create opengL window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	
	glutCreateWindow("Lab 4");
	// user initialization
	init();
	// set callback functions
	glutDisplayFunc(render);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(16, timer, 0);
	// main loop
	glutMainLoop();
	return 0;
}