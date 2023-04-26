#include <GL/glut.h>
#include <iostream>
#include <math.h>

// Border consts
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
GLint numSegments = 64,
      ballRadius = 16,
      ballX = (WIDTH / 2),
      ballY = (HEIGHT / 2),
      speedX = 5,
      speedY = 2,
      maxBallSpeed = 15;

// Score consts
GLint rightScore = 0,
      leftScore = 0,
      WINCONDITION = 1;

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

// Pause var
bool isPaused = false;

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

void ballCollision(GLvoid)
{
    GLboolean xBallCollidesRightRacket = ballX + ballRadius + speedX >= rightRacketXf && ballX + ballRadius + speedX <= rightRacketX,
              yBallCollidesRightRacket = ballY + ballRadius + speedY >= rightRacketY && ballY - ballRadius + speedY <= rightRacketYf,
              xBallCollidesLefttRacket = ballX - ballRadius + speedX >= leftRacketX && ballX - ballRadius + speedX <= leftRacketXf,
              yBallCollidesLefttRacket = ballY + ballRadius + speedY >= leftRacketY && ballY - ballRadius + speedY <= leftRacketYf;

    // Ball collision on the rackets
    if (xBallCollidesRightRacket && yBallCollidesRightRacket || xBallCollidesLefttRacket && yBallCollidesLefttRacket)
    {
        if (speedX > 0 && speedX < maxBallSpeed)
        {
            speedX++;
        }
        else if (speedX > -maxBallSpeed)
        {
            speedX--;
        }
        speedX *= -1;
    }

    // Ball collision on the left border
    else if (ballX + ballRadius + speedX >= WIDTH - BORDER_SIZE)
    {
        speedX *= -1;
        rightScore++;
    }
    // Ball collision on the right border
    else if (ballX - ballRadius + speedX <= BORDER_SIZE)
    {
        speedX *= -1;
        leftScore++;
    }
    // Ball collision on the top or bottom borders
    else if (ballY + ballRadius + speedY >= HEIGHT - BORDER_SIZE || ballY - ballRadius + speedY <= BORDER_SIZE)
    {
        speedY *= -1;
    }
}

void drawBall()
{
    ballCollision();

    ballX += speedX;
    ballY += speedY;

    glBegin(GL_TRIANGLE_FAN);
    // Defines the circle color as white
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < numSegments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
        float x = ballRadius * cosf(theta);
        float y = ballRadius * sinf(theta);
        glVertex2f(x + ballX, y + ballY);
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

void displayWinner(GLvoid)
{
    if (rightScore == WINCONDITION)
    {
        isPaused = true;
        // Winner message
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(480, 284);
        string str("LEFT SIDE WON");
        int len = str.size();
        for (int i = 0; i <= len; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
        }
        // Right score
        string rightScoreStr = to_string(rightScore);
        glColor3fv(white);
        glRasterPos2f(173, 284);
        for (char &c : rightScoreStr)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        // Left score
        string leftScoreStr = to_string(leftScore);
        glColor3fv(white);
        glRasterPos2f(973, 284);
        for (char &c : leftScoreStr)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        // Final messages
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(445, 484);
        string ada("PRESS R TO RESTART");
        int auxa = ada.size();
        for (int i = 0; i <= auxa; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ada[i]);
        }
        glRasterPos2f(455, 524);
        string tur("PRESS ESC TO EXIT");
        int auxt = tur.size();
        for (int i = 0; i <= auxt; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tur[i]);
        }
    }
    else
    {

        // Winner message
        glColor3f(0.0f, 0.0f, 1.0f);
        glRasterPos2f(480, 284);
        string str("RIGHT SIDE WON");
        int len = str.size();
        for (int i = 0; i <= len; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
        }
        // Right score
        string rightScoreStr = to_string(rightScore);
        glColor3fv(white);
        glRasterPos2f(173, 284);
        for (char &c : rightScoreStr)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        // Left score
        string leftScoreStr = to_string(leftScore);
        glColor3fv(white);
        glRasterPos2f(973, 284);
        for (char &c : leftScoreStr)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
        }

        // Final messages
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(445, 484);
        string ada("PRESS R TO RESTART");
        int auxa = ada.size();
        for (int i = 0; i <= auxa; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, ada[i]);
        }
        glRasterPos2f(455, 524);
        string tur("PRESS ESC TO EXIT");
        int auxt = tur.size();
        for (int i = 0; i <= auxt; i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, tur[i]);
        }
    }
}

void scoreValidation(GLvoid)
{
    if (rightScore == WINCONDITION || leftScore == WINCONDITION)
    {
        displayWinner();
    }
}

void displayScore(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Right score
    string rightScoreStr = to_string(rightScore);
    glColor3fv(white);
    glRasterPos2f(WIDTH / 4, 40);
    for (char &c : rightScoreStr)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    // Left score
    string leftScoreStr = to_string(leftScore);
    glColor3fv(white);
    glRasterPos2f(WIDTH - (WIDTH / 4), 40);
    for (char &c : leftScoreStr)
    {
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c);
    }

    glutPostRedisplay();

    scoreValidation();
}

void displayPause(GLvoid)
{
    // Print game paused in center
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(490, 284);
    string str("GAME PAUSED");
    int len = str.size();
    for (int i = 0; i <= len; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    }

    // Right score
    string rightScoreStr = to_string(rightScore);
    glColor3fv(white);
    glRasterPos2f(WIDTH / 4, 284);
    for (char &c : rightScoreStr)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }

    // Left score
    string leftScoreStr = to_string(leftScore);
    glColor3fv(white);
    glRasterPos2f(WIDTH - (WIDTH / 4), 284);
    for (char &c : leftScoreStr)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
    }
    glutPostRedisplay();
    glFlush();
}

void draw(GLvoid)
{
    if (leftScore == WINCONDITION || rightScore == WINCONDITION)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        displayWinner();
        glFlush();
        glutSwapBuffers();
        glutPostRedisplay();
    }

    else
    {
        if (!isPaused)
        {
            glClear(GL_COLOR_BUFFER_BIT);
            displayScore();
            drawBall();
            drawingBorders();
            drawingCenterLine();
            drawRackets();
            borderEffect();
            glFlush();
            glutSwapBuffers();
            glutPostRedisplay();
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT);
            // displayScore();
            displayPause();
            glFlush();
            glutSwapBuffers();
            glutPostRedisplay();
        }
    }
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

    case 'r':
    case 'R':
        if (rightScore == WINCONDITION || leftScore == WINCONDITION)
        {
            isPaused = false;
            rightScore = 0;
            leftScore = 0;
            speedX = 5;
            speedY = 2;
            ballX = (WIDTH / 2);
            ballY = (HEIGHT / 2);
        }

        break;

    // Use space to pause
    case ' ':
        isPaused = !isPaused; // change pause condition
        break;

    // Use esc to exit
    case 27:
        if (isPaused)
        {
            exit(0);
        }
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
         << "\nGustavo Caminha da Silva Junior\t\t20210114817\n"
         << endl;

    initGlut(&argc, argv);
    createWindow();
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    // glutDisplayFunc(displayScore);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(arrowKeys);
    glutMainLoop();
    return 0;
}

/*  Helpfull content

https://stackoverflow.com/questions/35514902/glutinitdisplaymodeglut-double-glut-rgb-glut-depth-explanation

https://www.youtube.com/watch?v=-IlxN9UjU_8&t=737s

*/