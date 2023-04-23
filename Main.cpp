#include <GL/glut.h>
#include <iostream>
#include <math.h>

GLfloat WIDTH = 1200,
        HEIGHT = 600,
        BORDER_SIZE = 10,
        xi = 0,
        xf = BORDER_SIZE,
        yi = 0,
        yf = 40,
        incX = 0.0,
        incY = 0.25;

// Rackets consts
GLint leftRacketX = 40,
      leftRacketXf = leftRacketX + 15,
      leftRacketY = (HEIGHT / 2) - 40,
      leftRacketYf = leftRacketY + 40,

      rightRacketX = WIDTH - 40,
      rightRacketXf = rightRacketX - 15,
      rightRacketY = (HEIGHT / 2) - 40,
      rightRacketYf = rightRacketY + 40,

      racketsSpeed = 20;

// Defining colors
GLfloat red[] = {1.0f, 0.0f, 0.0f};
GLfloat green[] = {0.0f, 1.0f, 0.0f};
GLfloat blue[] = {0.0f, 0.0f, 1.0f};
GLfloat white[] = {1.0f, 1.0f, 1.0f};

using namespace std;

void initGlut(int *argc, char **argv)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Pong With C++ and OpenGL");
}

void createWindow(GLvoid)
{
    // Set the window background color as black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // Defines the current matrix
    glMatrixMode(GL_PROJECTION);
    // Defines the current projection matrix as a 2D orthographic matrix position (left, right, bottom, top)
    gluOrtho2D(0, WIDTH, HEIGHT, 0);
}

void drawingBorders(GLvoid)
{
    glLineWidth(1.0);
    // Top
    glBegin(GL_POLYGON);
    glColor3fv(green);
    glVertex2i(0, 0);
    glVertex2i(BORDER_SIZE, BORDER_SIZE);
    glColor3fv(red);
    glVertex2i(WIDTH - BORDER_SIZE, BORDER_SIZE);
    glVertex2i(WIDTH, 0);
    glEnd();

    // Bottom
    glBegin(GL_POLYGON);
    glColor3fv(blue);
    glVertex2i(0, HEIGHT);
    glColor3fv(green);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(WIDTH - BORDER_SIZE, HEIGHT - BORDER_SIZE);
    glColor3fv(blue);
    glVertex2i(BORDER_SIZE, HEIGHT - BORDER_SIZE);
    glEnd();

    // Left
    glBegin(GL_POLYGON);
    glColor3fv(green);
    glVertex2i(0, 0);
    glColor3fv(blue);
    glVertex2i(0, HEIGHT);
    glVertex2i(BORDER_SIZE, HEIGHT - BORDER_SIZE);
    glColor3fv(green);
    glVertex2i(BORDER_SIZE, BORDER_SIZE);
    glEnd();

    // Right
    glBegin(GL_POLYGON);
    glColor3fv(red);
    glVertex2i(WIDTH, 0);
    glColor3fv(green);
    glVertex2i(WIDTH, HEIGHT);
    glVertex2i(WIDTH - BORDER_SIZE, HEIGHT - BORDER_SIZE);
    glColor3fv(red);
    glVertex2i(WIDTH - BORDER_SIZE, BORDER_SIZE);
    glEnd();
}

void drawingCenterLine(GLvoid)
{
    glLineWidth(2.0);
    GLfloat distance = WIDTH / 2,
            SPACE = 30,
            cont = 0;

    glColor3fv(white);
    while (cont < 14)
    {
        glBegin(GL_POLYGON);
        glVertex2i(distance - 2, SPACE);
        glVertex2i(distance + 2, SPACE);
        SPACE += 20;
        glVertex2i(distance + 2, SPACE);
        glVertex2i(distance - 2, SPACE);
        SPACE += 20;
        glEnd();
        cont++;
    }
}

void drawRackets(GLvoid)
{
    // Dranwig the left racket
    glBegin(GL_POLYGON);
    glVertex2i(leftRacketX, leftRacketY);
    glVertex2i(leftRacketXf, leftRacketY);
    glVertex2i(leftRacketXf, leftRacketYf);
    glVertex2i(leftRacketX, leftRacketYf);
    glEnd();

    // Dranwig the right racket
    glBegin(GL_POLYGON);
    glVertex2i(rightRacketX, rightRacketY);
    glVertex2i(rightRacketXf, rightRacketY);
    glVertex2i(rightRacketXf, rightRacketYf);
    glVertex2i(rightRacketX, rightRacketYf);
    glEnd();
}

void drawBall(float cx, float cy, float r, int num_segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0f, 1.0f, 1.0f); // Define a cor do círculo como branco
    for (int i = 0; i < num_segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

void borderEffect(GLvoid)
{
    glBegin(GL_POLYGON);
    glColor3fv(white);
    glVertex2i(xi, yi);
    glVertex2i(xf, yi);
    glVertex2i(xf, yf);
    glVertex2i(xi, yf);
    glEnd();

    yi += incY;
    yf += incY;
    xi += incX;
    xf += incX;

    if (yf >= HEIGHT && xi == 0)
    {
        yf = HEIGHT - BORDER_SIZE;
        yi = HEIGHT;
        xf = xi + 40;
        incX += (0.25);
        incY *= 0;
    }

    else if (xf >= WIDTH && yi == HEIGHT)
    {
        xi = WIDTH;
        xf = WIDTH - BORDER_SIZE;
        yi = yf - 40;
        incX *= 0;
        incY += -(0.25);
    }

    else if (xi >= WIDTH && yi <= 0)
    {
        yi = 0;
        yf = BORDER_SIZE;
        xi = xf - 40;
        incX += -(0.25);
        incY *= 0;
    }

    else if (xi <= 0 && yi <= 0)
    {
        xi = 0;
        xf = BORDER_SIZE;
        yf = yi + 40;
        incX *= 0;
        incY += 0.25;
    }
}

void draw(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT);
    drawingBorders();
    drawingCenterLine();
    drawRackets();
    drawBall((WIDTH / 2), (HEIGHT / 2), 16.f, 64);
    borderEffect();
    glFlush();
    glutPostRedisplay();
}

void keyboard(char unsigned key, GLint x, GLint y)
{
    switch (key)
    {
    case 'w':
    case 'W':
        if (leftRacketY == 20)
        {
            return;
        }
        leftRacketY -= racketsSpeed;
        leftRacketYf -= racketsSpeed;
        cout << leftRacketY << endl;
        break;
    case 's':
    case 'S':
        if (leftRacketYf == 580)
        {
            return;
        }
        leftRacketY += racketsSpeed;
        leftRacketYf += racketsSpeed;
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void arrowKeys(GLint key, GLint x, GLint y)
{
    if (key == GLUT_KEY_UP)
    {
        if (rightRacketY == 20)
        {
            return;
        }
        rightRacketY -= racketsSpeed;
        rightRacketYf -= racketsSpeed;
        cout << rightRacketY << endl;
    }

    if (key == GLUT_KEY_DOWN)
    {
        if (rightRacketYf == 580)
        {
            return;
        }
        rightRacketY += racketsSpeed;
        rightRacketYf += racketsSpeed;
    }

    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    cout << "\nALUNOS:\t\t\t\t\tMATRÍCULA:\n\nArthur Ramón Souza Ferreira Martins\t20210027186\n"
         << "\nDavi Baratto\t\t\t\t20210025940\n"
         << "\nJoão Roberto de Oliveira Ferreira\t20200083646\n"
         << endl;

    initGlut(&argc, argv);
    // Defining double buffering and RGBA exhibition mode
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    createWindow();
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKeys);
    glutMainLoop();
    return 0;
}
