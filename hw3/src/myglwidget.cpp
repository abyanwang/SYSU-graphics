#include "myglwidget.h"
#include <iostream>
using namespace std;
/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/
void UseMyShader();
float R = 0.5;
int theta = 0;
const double PI = acos(-1);

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
	timer = new QTimer(this); // 实例化一个定时器
	timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Your Implementation

    glViewport(0, 0, width(), height());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    //材料特性决定对光的反射 设定材料的反射系数
    GLfloat mat_ambient[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat mat_diffuse[] = {0.8f, 0.0f, 0.8f, 1.0f};
    GLfloat mat_specular[] = {1.0f, 0.0f, 1.0f, 1.0f};
    GLfloat mat_shininess[] = {50.0f};
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    //设置光源的位置
    GLfloat light_position[] = {0.0f, 1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    //分别设置环境光，漫反射光，镜面光
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    GLfloat diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    //打开光照
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);//更改点的大小
    glEnable(GL_POINT_SPRITE);//启用点精灵
    glEnable(GL_DEPTH_TEST);
    complie_shader();
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
	// Your Implementation
    glClear(GL_COLOR_BUFFER_BIT);

    theta %= 360;//theta 绘制圆型轨迹的极坐标角度
    theta += 1;

    //绘制光源的位置，但是没有加上贴纸
    glPushMatrix();
    glBegin(GL_POINTS);
    glColor3d(1, 1, 1);
    glVertex3f(0, 1, 0);
    glEnd();
    glPopMatrix();


    //绘制竖向的转动的球
    glPushMatrix();
    glBegin(GL_POINTS);
    glColor3d(0.5, 0, 1);
    glVertex3f(0, R * cos(theta * PI / 180), R * sin(theta * PI / 180));
    glEnd();
    glPopMatrix();

    //绘制横向的转动的球
    glPushMatrix();
    glBegin(GL_POINTS);
    glColor3d(0.489, 0.3903, 0.12);
    glVertex3f(R * cos(theta * PI / 180), 0, R * sin(theta * PI / 180));
    glEnd();
    glPopMatrix();
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

/*###################################################
##  函数: complie_shader
##  函数描述： 编译vertex着色器和fragment着色器
##  参数描述： 无
#####################################################*/
void MyGLWidget::complie_shader()
{
    //创建一个着色器对象
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //替换着色器对象中的源代码
    glShaderSource(vertexShader, 1, &vsChar, 0);
    glShaderSource(fragmentShader, 1, &frChar, 0);

    //编译一个着色器对象
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);

    //创建一个program
    GLuint program = glCreateProgram();

    //将着色器对象附加到program对象
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    //连接一个program对象
    glLinkProgram(program);

    //指明在接下来的绘制中使用program所代表 的着色器程序
    glUseProgram(program);
}



