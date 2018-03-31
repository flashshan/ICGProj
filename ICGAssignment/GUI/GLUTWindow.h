#pragma once


// a static class

class GLUTWindow
{
public:
	static void initOpenGL();
	static void display();
	static void closeOpenGL();

	static void reshape(int w, int h);
	
	static void NormalKeyPress(unsigned char key, int x, int y);
	static void SpecialKeyPress(int key, int x, int y);
	static void SpecialKeyRelease(int key, int x, int y);

	static void PassiveMouseCB(int x, int y);
	static void MouseButtonPress(int button, int state, int x, int y);
private:

	static bool ctrlPressed_;

};

