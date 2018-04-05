#pragma once



namespace GLUTWindow
{
	void InitOpenGL();
	void Display();
	void CloseOpenGL();

	void Reshape(int w, int h);

	void NormalKeyPress(unsigned char key, int x, int y);
	void SpecialKeyPress(int key, int x, int y);
	void SpecialKeyRelease(int key, int x, int y);

	void PassiveMouseCB(int x, int y);
	void MouseButtonPress(int button, int state, int x, int y);
};

