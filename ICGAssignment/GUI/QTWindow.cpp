#include <gl\glew.h>

#include "QTWindow.h"

#include <QtGui\qkeyevent>

void QTWindow::initializeGL()
{
	timer_ = new QTimer(this);

	connect(timer_, SIGNAL(timeout()), this, SLOT(timerUpdate()));
	timer_->start(0);

	setMinimumSize(1280, 720);
	glViewport(0, 0, width(), height());
	glewInit();

}


void QTWindow::paintGL()
{
	float alpha = (float)curFrame_ / (float)bgAnimationFrame_;
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.5f + alpha, 1.0f - alpha, 0.5f + alpha, 1.0f);


	if (++curFrame_ > bgAnimationFrame_)
		curFrame_ -= bgAnimationFrame_;
}

void QTWindow::timerUpdate()
{
	repaint();
}

void QTWindow::keyPressEvent(QKeyEvent* e)
{
	switch (e->key())
	{
	case Qt::Key::Key_Escape:
		exit(0);
		break;
	}
	repaint();
}