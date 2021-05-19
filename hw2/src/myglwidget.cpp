#include "myglwidget.h"

void draw_cube();  //绘制基础的立方体
void drawSphere(float,float,float);   //绘制球体
void draw_sth(float ,float ,float ,float ,float ,float); //绘制给定大小，颜色的立方体
const double PI = acos(-1);

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

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
	glViewport(0, 0, width(), height());  
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
    glEnable(GL_DEPTH_TEST);
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(85, width()/height(), 1, 500);//设置透视投影

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 80, 80, 40*cos(2*PI/360*th), 0, 40*sin(2*PI/360*th), 0, 1, 0);//定义视图
    //gluLookAt(0, 80, 80, 0, 0, 0, 0, 1, 0);
    //绘制机器人行走的路径，路径为r=40的圆，中心为(0,0,0)
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    for (int i=0; i<360; ++i)
    {
        double theta = 2*PI/360*i;
        glVertex3f(40 * cos(theta), 0,40 * sin(theta));
        //printf("%f %f\n", cos(theta), sin(theta));

    }
    glEnd();

    //利用极坐标的方法，将机器人旋转到圆的切线角度，在将机器人放置到圆的指定位置上
    th%=360;
    th+=1;
    printf("%f %f %f\n", float(th),cos(2*PI/360*th), sin(2*PI/360*th));
    glTranslatef(40*cos(2*PI/360*th),0,40*sin(2*PI/360*th));
    glRotatef(-th,0.0f,1.0f,0.0f);

    //绘制球形的头
    glPushMatrix();
    glTranslatef(0.0f, 40.0f, 0.0f);
    //draw_sth(1.0f,1.0f,1.0f,0.95f, 0.82f, 0.69f);
    drawSphere(4,20,20);
    glPopMatrix();


    //绘制立方体的躯干
    glPushMatrix();
    glTranslatef(0.0f, 28.0f, 0.0f);
    draw_sth(2.0f, 4.0f, 2.0f,0.2f, 0.82f, 0.69f);
    glPopMatrix();

    //绘制左大腿
    glPushMatrix();
    glTranslatef(4.0f,10.0f,0.0f);
    left_leg_angle+=leg_angle*left_leg_plus;  //进行腿部的摆动
    if (left_leg_angle*left_leg_plus >= leg_bound) //判断腿部摆动的方向
    {
        left_leg_plus*=(-1);
    }
    glTranslatef(0.0f,10.0f,0.0f);
    glRotatef(left_leg_angle,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-10.0f,0.0f);
    draw_sth(1.0f, 5.0f, 1.0f,0.65f, 0.82f, 0.69f);
    glPopMatrix();


    //绘制右大腿
    glPushMatrix();
    glTranslatef(-4.0f,10.0f,0.0f);
    right_leg_angle+=leg_angle*right_leg_plus; //进行腿部的摆动
    if (right_leg_angle*right_leg_plus >= leg_bound) //判断腿部摆动的方向
    {
        right_leg_plus*=(-1);
    }
    glTranslatef(0.0f,10.0f,0.0f);
    glRotatef(right_leg_angle,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-10.0f,0.0f);
    draw_sth(1.0f, 5.0f, 1.0f,0.65f, 0.82f, 0.69f);
    glPopMatrix();


    //绘制左手
    glPushMatrix();
    glTranslatef(6.0f,30.0f,0.0f);
    left_arm_angle+=arm_angle*left_arm_plus; //进行手部的摆动
    if (left_arm_angle*left_arm_plus >= arm_bound) //判断手部摆动的方向
    {
        left_arm_plus*=(-1);
    }
    glTranslatef(0.0f,6.0f,0.0f);
    glRotatef(left_arm_angle,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-6.0f,0.0f);
    draw_sth(1.0f, 3.0f, 1.0f,0.35f, 0.82f, 0.69f);
    glPopMatrix();


    //绘制右手
    glPushMatrix();
    glTranslatef(-6.0f,30.0f,0.0f);
    right_arm_angle+=arm_angle*right_arm_plus;  //进行手部的摆动
    if (right_arm_angle*right_arm_plus >= arm_bound) //判断手部摆动的方向
    {
        right_arm_plus*=(-1);
    }
    glTranslatef(0.0f,6.0f,0.0f);
    glRotatef(right_arm_angle,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,-6.0f,0.0f);
    draw_sth(1.0f, 3.0f, 1.0f,0.35f, 0.82f, 0.69f);
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
##  函数: draw_sth
##  函数描述： 画出指定大小，和指定颜色的立方体
##  参数描述： x,y,z是glScalef()的参数，来进行在基础立方体上的缩放。a,b,c是指定的颜色的参数。
#####################################################*/
void draw_sth(float x,float y,float z,float a,float b,float c)
{
    glPushMatrix();
    glColor3f(a,b,c);
    glScalef(x, y, z);
    draw_cube();  //调用绘制最基本的立方体函数
    glPopMatrix();
}

/*###################################################
##  函数: drawSphere
##  函数描述：利用极坐标的方式来绘制指定半径，中心为(0,0,0)的球体
##  参数描述：r是球体的半径，M,N分别表示球的横纵分成多少份
#####################################################*/
void drawSphere(float r, float M, float N)
{
    float step_z = PI / M;             //每次在z轴上增加的角度
    float step_xy = 2 * PI / N;        //每次在x,y平面上增加的角度
    float x[4], y[4], z[4];

    float angle_z = 0.0;
    float angle_xy = 0.0;
    int i = 0, j = 0;
    glColor3f(0.95f, 0.82f, 0.20f);
    glBegin(GL_QUADS);
    for(i = 0; i < M; i++)
    {
        angle_z = i * step_z;         //角度递增

        for(j = 0; j < N; j++)
        {
            angle_xy = j * step_xy;   //角度递增
            //绘制空间中的四边形
            x[0] = r * sin(angle_z) * cos(angle_xy);
            y[0] = r * sin(angle_z) * sin(angle_xy);
            z[0] = r * cos(angle_z);

            x[1] = r * sin(angle_z + step_z) * cos(angle_xy);
            y[1] = r * sin(angle_z + step_z) * sin(angle_xy);
            z[1] = r * cos(angle_z + step_z);

            x[2] = r * sin(angle_z + step_z) * cos(angle_xy + step_xy);
            y[2] = r * sin(angle_z + step_z) * sin(angle_xy + step_xy);
            z[2] = r * cos(angle_z + step_z);

            x[3] = r * sin(angle_z) * cos(angle_xy + step_xy);
            y[3] = r * sin(angle_z) * sin(angle_xy + step_xy);
            z[3] = r * cos(angle_z);
            for(int k = 0; k < 4; k++)
            {
                glVertex3f(x[k], y[k], z[k]);
            }
        }
    }
    glEnd();
}




/*###################################################
##  函数: draw_cube
##  函数描述：绘制边长为4的，中心为(0,0,0)的立方体
##  参数描述：无
#####################################################*/
void draw_cube()
{
    glPushMatrix();
    glBegin(GL_QUADS);

    // 绘制上面
    glVertex3f(2.0f, 2.0f, 2.0f);
    glVertex3f(2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, 2.0f, 2.0f);
    // 绘制下面
    glVertex3f(2.0f, -2.0f, 2.0f);
    glVertex3f(2.0f, -2.0f, -2.0f);
    glVertex3f(-2.0f, -2.0f, -2.0f);
    glVertex3f(-2.0f, -2.0f, 2.0f);
    // 绘制前面
    glVertex3f(2.0f, 2.0f, 2.0f);
    glVertex3f(-2.0f, 2.0f, 2.0f);
    glVertex3f(-2.0f, -2.0f, 2.0f);
    glVertex3f(2.0f, -2.0f, 2.0f);
    // 绘制后面
    glVertex3f(2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, -2.0f, -2.0f);
    glVertex3f(2.0f, -2.0f, -2.0f);
    // 绘制左面
    glVertex3f(-2.0f, 2.0f, 2.0f);
    glVertex3f(-2.0f, 2.0f, -2.0f);
    glVertex3f(-2.0f, -2.0f, -2.0f);
    glVertex3f(-2.0f, -2.0f, 2.0f);
    // 绘制右面
    glVertex3f(2.0f, 2.0f, 2.0f);
    glVertex3f(2.0f, 2.0f, -2.0f);
    glVertex3f(2.0f, -2.0f, -2.0f);
    glVertex3f(2.0f, -2.0f, 2.0f);

    glEnd();
    glPopMatrix();
}

