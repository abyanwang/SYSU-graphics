#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#ifdef MAC_OS
#include <QtOpenGL/QtOpenGL>
#else
#include <GL/glew.h>
#endif
#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <math.h>

class MyGLWidget : public QOpenGLWidget{
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QTimer *timer;

    int left_arm_plus = -1;  //左手摆动的方向
    int right_arm_plus = 1;  //右手摆动的方向
    int arm_angle = 6;       //手臂每次增加的角度
    int left_arm_angle = 36;  //手臂初始角度
    int right_arm_angle = -36;
    int arm_bound = 36;      //手臂摆动的边界

    int left_leg_plus = 1;   //左腿摆动的方向
    int right_leg_plus = -1; //右腿摆动的方向
    int leg_angle = 8;       //腿部增加的角度
    int left_leg_angle = -48; //腿部初始角度
    int right_leg_angle = 48;
    int leg_bound = 48;

    int th = 0;           //行进中的角度，用来将机器人平移到指定的位置
};
#endif // MYGLWIDGET_H
