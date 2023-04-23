#include <GL/glut.h>
#include <iostream>
#include <math.h>

GLint WIDTH = 1200,
      HEIGHT = 600,
      BORDER_SIZE = 10,
      xi = 0,
      xf = BORDER_SIZE,
      yi = 0,
      yf = 40,
      incX = 0.0,
      incY = 100;

// Ball consts
GLint num_segments = 128;

// Score consts
GLint rightScore = 0,
      leftScore = 0;      
// Rackets consts
GLint leftRacketX = 40,
      leftRacketXf = leftRacketX + 15,
      leftRacketY = (HEIGHT / 2) - 60,
      leftRacketYf = leftRacketY + 60,

      rightRacketX = WIDTH - 40,
      rightRacketXf = rightRacketX - 15,
      rightRacketY = (HEIGHT / 2) - 60,
      rightRacketYf = rightRacketY + 60,

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
    /*
        GLUT_DOUBLE
            Bit mask to select a double buffered window. This overrides GLUT_SINGLE if it is also specified.

        GLUT_DEPTH
            Bit mask to select a window with a depth buffer.
    */
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
    glColor3fv(white);
    glVertex2i(leftRacketX, leftRacketY);
    glVertex2i(leftRacketXf, leftRacketY);
    glVertex2i(leftRacketXf, leftRacketYf);
    glVertex2i(leftRacketX, leftRacketYf);
    glEnd();

    // Dranwig the right racket
    glBegin(GL_POLYGON);
    glColor3fv(white);
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
        incX += 10;
        incY *= 0;
    }

    else if (xf >= WIDTH && yi == HEIGHT)
    {
        xi = WIDTH;
        xf = WIDTH - BORDER_SIZE;
        yi = yf - 40;
        incX *= 0;
        incY += -10;
    }

    else if (xi >= WIDTH && yi <= 0)
    {
        yi = 0;
        yf = BORDER_SIZE;
        xi = xf - 40;
        incX += -10;
        incY *= 0;
    }

    else if (xi <= 0 && yi <= 0)
    {
        xi = 0;
        xf = BORDER_SIZE;
        yf = yi + 40;
        incX *= 0;
        incY += 10;
    }
}

void displayScore(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    string rightScoreStr = to_string(rightScore);
    glColor3fv(white);
    glRasterPos2f(WIDTH / 4, 40);
    for (char& c : rightScoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    
    string leftScoreStr = to_string(leftScore);
    glColor3fv(white);
    glRasterPos2f(WIDTH - (WIDTH / 4) , 40);
    for (char& c : leftScoreStr) {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }
    
    glutPostRedisplay();
}

void draw(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT);
    displayScore();
    drawingBorders();
    drawingCenterLine();
    drawRackets();
    drawBall((WIDTH / 2), (HEIGHT / 2), 16.f, num_segments);
    borderEffect();
    glFlush();
    glutSwapBuffers();
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
    createWindow();
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    //glutDisplayFunc(displayScore);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKeys);
    glutMainLoop();
    return 0;
}

/*  Helpfull content

https://stackoverflow.com/questions/35514902/glutinitdisplaymodeglut-double-glut-rgb-glut-depth-explanation

*/