#include "MATRIX.h"

#include <glut.h>

void Lighting(void)
{
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);


	//float light_pos[] = { 25.0, 50.0, 50.0, 0.0 };
	//////////////////////////////////////////
	float light_pos[] = { 0, 50, 0, 1.0f};
	//////////////////////////////////////////

	
	float light_dir[] = { 0, -1, -1 };
	float light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	float light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
	float light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light_dir);

	//////////////////////////////////////////
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 15);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 100);
	//////////////////////////////////////////

	

	/*
	float noMat[] = { 0, 0, 0, 1 };
	float matAmb[] = { 0, 0, 0, 1 };
	float matDif[] = { 0, 0, 0, 1 };
	float matSpec[] = { 0.2f, 0.2f, 0.2f, 1.0 };
	float matShininess = 200;
	
	
	//////////////////////////////////////////
	glMaterialfv(GL_FRONT, GL_EMISSION, noMat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDif);
	//////////////////////////////////////////

	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
	glMaterialf(GL_FRONT, GL_SHININESS, matShininess);

	*/

	
}

void Initialize(int argc, char** argv)
{
	glutInit(&argc, argv);

	// ������ ����
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(1024, 576);
	glutCreateWindow("OpenGL Lecture");
	
	// depth test
	glEnable(GL_DEPTH_TEST);

	// ���� ����
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glDisable(GL_CULL_FACE);

	Lighting();
}