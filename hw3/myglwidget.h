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

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void complie_shader();
private:
    QTimer *timer;

    // 你可以将你的shader或者vbo声明为私有变量
    // GLuint vertexShader;
    // GLuint fragmentShader;
    const char *vsChar = R"(
    #version 120

    varying vec3 vertex_to_light_vector;
    varying vec4 vertex_in_modelview_space;
    void main()
    {
            //顶点变换到裁剪空间位置，作为输出
            gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

            //进行近大远小
            gl_PointSize = 100*(0.6+gl_Position.z);
            gl_FrontColor = gl_Color;
            vertex_in_modelview_space = (gl_ModelViewMatrix * gl_Vertex);
            vertex_to_light_vector = vec3(gl_LightSource[0].position - vertex_in_modelview_space);

    })";

    const char *frChar = R"(
    #version 120
    varying vec3 vertex_to_light_vector;
    varying vec4 vertex_in_modelview_space;
    void main()
    {
            vec2 n_xy = gl_PointCoord - vec2(0.5, 0.5);

            float dist = sqrt(dot(n_xy,n_xy));//距离
            if (dist >= 0.5)discard;

            vec3 n = normalize(vec3(n_xy, sqrt(pow(0.5,2)-pow(dist,2))));   // 法向量
            vec3 l = normalize(vertex_to_light_vector);   // 入射方向
            vec3 r = normalize(reflect(-l, n));  // 反射方向
            vec3 v = normalize(vec3(0, 0, 1) - vertex_in_modelview_space.xyz);     // 观察方向
            vec3 h = normalize(v + l);

            vec4 ambient = gl_LightSource[0].ambient;
            vec4 diffuse = gl_LightSource[0].diffuse;
            vec4 specular = gl_LightSource[0].specular;

            //漫反射
            float diffuse_term = clamp(max(dot(n, l),0.0), 0.0, 1.0);
            //镜面反射
            int a = 5;
            float specular_term = max(pow(dot(r, v), a), 0.0);

            //Blinn-phong
            float specular_term1 = max(pow(dot(n, h), a), 0.0);


            if (dot(r,v) < 0)
                gl_FragColor = (ambient + diffuse*diffuse_term+0.0) * gl_Color;
            else
                gl_FragColor = (ambient + diffuse*diffuse_term + 0.5*specular*specular_term) * gl_Color;
    }
)";
};
#endif // MYGLWIDGET_H
