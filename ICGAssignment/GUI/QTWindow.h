#pragma once


#include <QtOpenGL\qglwidget>

#include <string>
#include <Qt\qtimer.h>


class QTWindow : public QGLWidget
{
	Q_OBJECT
protected:
	void initializeGL();
	void paintGL() override;
	
private slots:
	void timerUpdate();
	
public:
	void keyPressEvent(QKeyEvent*);

	


private:
	QTimer *timer_;
	int bgAnimationFrame_ = 60;
	int curFrame_ = 0;

};

