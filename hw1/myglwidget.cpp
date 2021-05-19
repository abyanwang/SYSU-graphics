/*
################################
#author:张富瑞
#文件说明:根据老师要求，在scene_1中不使用glBegin(GL_LINE_STRIP)，glTranslatef()及glRotatef()函数的情况下重现scene_0场景
################################
*/
#include "myglwidget.h"
#include<math.h>
const float PI  = acos(-1);

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
	scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_0();
	}
	else {
		scene_1();
	}
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_0) {
		scene_id = 0;
		update();
	}
	else if (e->key() == Qt::Key_1) {
		scene_id = 1;
		update();
	}
}

void MyGLWidget::scene_0()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0f, 50.0f, 0.0f);
	
	//draw a triangle
	glPushMatrix();
	glColor3f(0.839f, 0.153f, 0.157f);
	glTranslatef(-20.0f, -10.0f, 0.0f);
    glRotatef(45.0f, 1.0f, 0.0f, 1.0f);
	glTranslatef(-50.0f, -30.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(10.0f, 10.0f);
	glVertex2f(50.0f, 50.0f);
	glVertex2f(80.0f, 10.0f);
	glEnd();
	glPopMatrix();	

	//draw a quad
	glPushMatrix();
	glColor3f(0.122f, 0.467f, 0.706f);
	glTranslatef(20.0f, 20.0f, 0.0f);
    glRotatef(30.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-20.0f, -20.0f);
	glVertex2f(20.0f, -20.0f);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(-20.0f, 20.0f);
	glEnd();
	glPopMatrix();
}

/*
####################################
#函数:tanslate
#函数描述:进行点的坐标平移
#参数描述:
#part1:x,y,z是点在三维坐标系下的坐标
#part2:dx,dy,dz是三个维度上的平移的距离
####################################
*/
void trans(float &x,float &y,float &z,float dx,float dy,float dz)
{
	x+=dx;
	y+=dy;
	z+=dz;
}

/*
####################################
#函数:rotate
#函数描述:进行点的坐标旋转
#参数描述:
#part1:x,y,z 点的坐标
#part2:angle 旋转的角度
#part3:a,b,c 旋转轴的坐标
####################################
*/
void rotate(float &x,float &y,float &z,float angle,float a,float b,float c)
{
	float len = sqrt(a*a+b*b+c*c);
    a/=len;b/=len;c/=len;
    float sin_val = float(sin(-angle * (PI/180.0)));
    float cos_val = float(cos(angle * (PI/180.0)));
    float matrix[3][4] = {{a*a*(1-cos_val)+cos_val, a*b*(1-cos_val)+c*sin_val, a*c*(1-cos_val)-b*sin_val, 0},
                          {a*b*(1-cos_val)-c*sin_val, b*b*(1-cos_val)+cos_val, b*c*(1-cos_val)+a*sin_val, 0},
                          {a*c*(1-cos_val)+b*sin_val, b*c*(1-cos_val)-a*sin_val, c*c*(1-cos_val)+cos_val, 0}};
	float t[3];
    for (int i=0; i<3; ++i)
        t[i] = x * matrix[i][0] + y * matrix[i][1] + z * matrix[i][2] + matrix[i][3];
    x = t[0];
    y = t[1];
    z = t[2];
}

/*
######################################
#函数:drawline
#函数描述:实现两点之间的画线,实现的逻辑是光栅化中的Bresenham’s algorithm
#参数描述:
#part1:x1,y1 为一个点的二维坐标,x2,y2为另一个点的二维坐标
######################################
*/
void Drawline(int x1, int y1, int x2, int y2)
{
     int dx = x2 - x1;
     int dy = y2 - y1;
	 int ux = (dx > 0) ? 1:-1;
	 int uy = (dy > 0) ? 1:-1; 
     int x = x1, y = y1, eps = 0;//eps为累加误差
     dx = abs(dx); dy = abs(dy); 
     if (dx > dy) 
     {
         for (x = x1; x != x2; x += ux)
         {
              glVertex2i(x,y);
//			  printf("%d %d\n",x,y);
              eps += dy;
              if ((eps << 1) >= dx)
              {
                   y += uy; eps -= dx;
              }
         }
     }
     else
     {
         for (y = y1; y != y2; y += uy)
         {
              glVertex2i(x,y);
              eps += dx;
              if ((eps << 1) >= dy)
              {
                   x += ux; eps -= dy;
              }
         }
     }             
}

/*
#####################################
#函数:scene_1
#函数描述:实现scene_0()中同样的功能
#####################################
*/
void MyGLWidget::scene_1()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

    //glBegin(GL_POINTS);
 //   glPointSize(2);
   // glBegin(GL_LINE_LOOP);
	//your implementation
	//glVertex2i()
	glColor3f(0.839f, 0.153f, 0.157f); //rgb
	float node[4][4] = {0.0};
    for (int i = 0;i < 4;i++){
        for (int j = 0;j < 4;j++){
            printf("%f",node[i][j]);
        }
    }
    for(int i = 1;i <= 3;i++)node[i][3] = 0.0;
	node[1][1] = 10.0;node[1][2] = 10.0;
	node[2][1] = 50.0;node[2][2] = 50.0;
	node[3][1] = 80.0;node[3][2] = 10.0;

	for(int i = 1;i <= 3;i++){
		trans(node[i][1],node[i][2],node[i][3],-50.0f, -30.0f, 0.0f);
    }
	for(int i = 1;i <= 3;i++){
		rotate(node[i][1],node[i][2],node[i][3],45.0f, 1.0f, 0.0f, 1.0f);
	}
	for(int i = 1;i <= 3;i++){
		trans(node[i][1],node[i][2],node[i][3],-20.0f, -10.0f, 0.0f);
	}
    for(int i = 1;i <= 3;i++){
        trans(node[i][1],node[i][2],node[i][3],50.0f, 50.0f, 0.0f);
    }
	for(int i = 1;i <= 3;i++){
		printf("node%d: %f %f %f\n",i,node[i][1],node[i][2],node[i][3]);
	}
    glBegin(GL_POINTS);
    Drawline(int(node[1][1]*width()/100),int(node[1][2]*height()/100),int(node[2][1]*width()/100),int(node[2][2]*height()/100));
    Drawline(int(node[1][1]*width()/100),int(node[1][2]*height()/100),int(node[3][1]*width()/100),int(node[3][2]*height()/100));
    Drawline(int(node[3][1]*width()/100),int(node[3][2]*height()/100),int(node[2][1]*width()/100),int(node[2][2]*height()/100));
    glEnd();

	//
	glColor3f(0.122f, 0.467f, 0.706f);
    float node1[5][5] = {0.0};
    for(int i = 1;i <= 4;i++)node1[i][3] = 0.0;
    node1[1][1] = -20.0f;node1[1][2] = -20.0f;
    node1[2][1] = 20.0f;node1[2][2] = -20.0f;
    node1[3][1] = 20.0f;node1[3][2] = 20.0f;
    node1[4][1] = -20.0f;node1[4][2] = 20.0f;
    for(int i = 1;i <= 4;i++)rotate(node1[i][1],node1[i][2],node1[i][3],30.0f, 1.0f, 1.0f, 1.0f);
    for(int i = 1;i <= 4;i++)trans(node1[i][1],node1[i][2],node1[i][3],20.0f, 20.0f, 0.0f);
    for(int i = 1;i <= 4;i++)trans(node1[i][1],node1[i][2],node1[i][3],50.0f, 50.0f, 0.0f);
    glBegin(GL_POINTS);
    Drawline(int(node1[1][1]*width()/100),int(node1[1][2]*height()/100),int(node1[2][1]*width()/100),int(node1[2][2]*height()/100));
    Drawline(int(node1[2][1]*width()/100),int(node1[2][2]*height()/100),int(node1[3][1]*width()/100),int(node1[3][2]*height()/100));
    Drawline(int(node1[3][1]*width()/100),int(node1[3][2]*height()/100),int(node1[4][1]*width()/100),int(node1[4][2]*height()/100));
    Drawline(int(node1[4][1]*width()/100),int(node1[4][2]*height()/100),int(node1[1][1]*width()/100),int(node1[1][2]*height()/100));
    glEnd();
}



