#define OUTPUT_ANIMATION 0

#include <stdlib.h>
#include <stdio.h>

#include "Particles.h"

#if OUTPUT_ANIMATION
#include <opencv2/opencv.hpp>
#endif


inline float clip(const float& n, const float& lower, const float& upper)
{
    return glm::max(lower, glm::min(n, upper));
}

float theta = M_PI;
float phi = M_PI_2;
float dist = 2.5;
int width = 800;
int height = 800;
int frame = 0;
const int render_step = 3;
int mx, my;
int last_time = -1;
bool mouse_down = false;

Particles particles;

void display(void);

void reshape(int width, int height);

void idle(void)
{
    if (!(frame % 10)) {
    int new_time = glutGet(GLUT_ELAPSED_TIME);
    particles.step(new_time - last_time);
    last_time = new_time;}
    glutPostRedisplay();
    if(frame/render_step >= 300)
        return;
    if(frame%render_step == 0)
    {
        #if OUTPUT_ANIMATION
        cv::Mat3b image(height, width);
        glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, image.data);
        cv::flip(image, image, 0);
        char fn[512];
        sprintf(fn, "result/%04d.png", frame/render_step);
        cv::imwrite(fn, image);
        #endif
    }
    frame++;
}

void mouse(int button, int state, int x, int y);

void mouse_motion(int x, int y);

void motion(int x, int y);

void keyboard(unsigned char c, int x, int y)
{
    switch(c)
    {
    case 'o' :
        break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(width, height);

    (void)glutCreateWindow("GLUT Program");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutMainLoop();
    glutKeyboardFunc(keyboard);

    return EXIT_SUCCESS;
}

void reshape(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // your drawing code goes here
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, 1, 0.01, 100);
    gluLookAt(dist*sin(phi)*cos(theta), dist*cos(phi), dist*sin(phi)*sin(theta),
            0, 0, 0,
            0, 1, 0);

    particles.render();

    glutSwapBuffers();
}

void mouse(int button, int state, int x, int y)
{
    // if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    // {
    //     mouse_down = true;
    //     mx = x;
    //     my = y;
    // }
    // if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    // {
    //     // mouse_down = false;
    //     mx = x;
    //     my = y;
    // }
}

void motion(int x, int y)
{
    particles.spawn_smoke(1.0 - (double)y / glutGet(GLUT_WINDOW_HEIGHT), (double)x / glutGet(GLUT_WINDOW_WIDTH));
    // int dx = x - mx;
    // int dy = y - my;
    // mx = x;
    // my = y;
    // if(abs(dx) > abs(dy))
    //     theta += dx*0.005;
    // else
    //     phi -= dy*0.005;
    // if(theta > 2*M_PI)
    //     theta -= 2*M_PI;
    // if(theta < -2*M_PI)
    //     theta += 2*M_PI;
    // phi = clip(phi, M_PI/12, M_PI*11/12);
    // glutPostRedisplay();
}
