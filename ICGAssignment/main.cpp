#include "GUI/GLUTWindow.h"

#include <GL/freeglut.h>
#include <cyTriMesh.h>

#include <stdio.h>
#include <string.h>
#include <assert.h>

//void GLUTWindow::keyPressEvent(QKeyEvent* e)
//{
//	switch (e->key())
//	{
//	case Qt::Key::Key_Escape:
//		exit(0);
//		break;
//	}
//	repaint();
//}


int main(int argc, char* argv[])
{
	//QApplication app(argc, argv);
	//
	//GLWindow glWindow;
	//glWindow.show();
	//return app.exec();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("ICG Practice");

	GLUTWindow::initOpenGL();


	glutKeyboardFunc(GLUTWindow::NormalKeyPress);
	glutSpecialFunc(GLUTWindow::SpecialKeyPress);
	glutSpecialUpFunc(GLUTWindow::SpecialKeyRelease);
	//glutMouseFunc(GLUTWindow::MouseButtonPress);
	glutPassiveMotionFunc(GLUTWindow::PassiveMouseCB);


	glutDisplayFunc(GLUTWindow::display);
	glutIdleFunc(GLUTWindow::display);
	glutReshapeFunc(GLUTWindow::reshape);

	glutMainLoop();

	GLUTWindow::closeOpenGL();
	return 0;
}