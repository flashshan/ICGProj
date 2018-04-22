#include "GUI/GLUTWindow.h"

#include <GL/freeglut.h>
#include <cyTriMesh.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>


int main(int argc, char* argv[])
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutInitWindowSize(1000, 1000);
	glutInitWindowPosition(100, 0);
	glutCreateWindow("ICG Practice");

	GLUTWindow::InitOpenGL();

	glutKeyboardFunc(GLUTWindow::NormalKeyPress);
	glutSpecialFunc(GLUTWindow::SpecialKeyPress);
	glutSpecialUpFunc(GLUTWindow::SpecialKeyRelease);
	glutPassiveMotionFunc(GLUTWindow::PassiveMouseCB);


	glutDisplayFunc(GLUTWindow::Display);
	glutIdleFunc(GLUTWindow::Display);
	glutReshapeFunc(GLUTWindow::Reshape);

	glutMainLoop();

	GLUTWindow::CloseOpenGL();
	return 0;
}