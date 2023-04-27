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
      speedY = 0,
      maxBallSpeed = 15;

// Score consts
GLint rightScore = 0,
      leftScore = 0,
      WINCONDITION = 10;

// Rackets consts
GLint leftRacketX = 40,
      leftRacketXf = leftRacketX + 15,
      leftRacketY = (HEIGHT / 2) - 60,
      leftRacketYf = leftRacketY + 60,

      rightRacketX = WIDTH - 40,
      rightRacketXf = rightRacketX - 15,
      rightRacketY = (HEIGHT / 2) - 60,
      rightRacketYf = rightRacketY + 60,

      racketsSpeed = 10;

// Pause variable
GLboolean isPaused = false;

// Variable to control keys
GLboolean keystates[256] = {false},
          specialKeyStates[256] = {false};

// Defining colors
GLfloat red[] = {1.0f, 0.0f, 0.0f};
GLfloat green[] = {0.0f, 1.0f, 0.0f};
GLfloat blue[] = {0.0f, 0.0f, 1.0f};
GLfloat white[] = {1.0f, 1.0f, 1.0f};

using namespace std;

void initGlut(int *argc, char **argv);
void createWindow(GLvoid);
void drawingBorders(GLvoid);
void borderEffect(GLvoid);
void drawingCenterLine(GLvoid);
void drawRackets(GLvoid);
void ballCollision(GLvoid);
void drawBall();
void displayScore(GLvoid);
void scoreValidation(GLvoid);
void displayWinner(GLvoid);
void displayPause(GLvoid);
void keyboard(char unsigned key, GLint x, GLint y);
void onKeyDown(unsigned char key, int x, int y);
void onKeyUp(unsigned char key, int x, int y);
void onSpecialDown(int key, int x, int y);
void onSpecialUp(int key, int x, int y);
void draw(GLvoid);

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
    GLboolean xBallCollidesRightRacket = ballX + ballRadius + speedX >= rightRacketXf,
              yBallCollidesRightRacket = ballY + ballRadius + speedY >= rightRacketY && ballY - ballRadius + speedY <= rightRacketYf,
              xBallCollidesLefttRacket = ballX - ballRadius + speedX <= leftRacketXf,
              yBallCollidesLefttRacket = ballY + ballRadius + speedY >= leftRacketY && ballY - ballRadius + speedY <= leftRacketYf;

    // Ball collision on the right racket
    if (xBallCollidesRightRacket && yBallCollidesRightRacket)
    {
        ballX = rightRacketXf - ballRadius;
        speedX *= -1;
        if (speedX != maxBallSpeed)
        {
            speedX--;
        }
    }
    // Ball collision on the left racket
    else if (xBallCollidesLefttRacket && yBallCollidesLefttRacket)
    {
        ballX = leftRacketXf + ballRadius;
        speedX *= -1;
        if (speedX != maxBallSpeed)
        {
            speedX++;
        }
    }
    // Ball collision on the left border
    else if (ballX + ballRadius + speedX >= WIDTH - BORDER_SIZE)
    {
        speedX *= -1; // Exclude later
        rightScore++;
    }
    // Ball collision on the right border
    else if (ballX - ballRadius + speedX <= BORDER_SIZE)
    {
        speedX *= -1; // Exclude later
        leftScore++;
    }
    // Ball collision on the bottom border
    else if (ballY + ballRadius + speedY >= HEIGHT - BORDER_SIZE)
    {
        ballY = HEIGHT - BORDER_SIZE - ballRadius;
        speedY *= -1;
    }
    // Ball collision on the top border
    else if (ballY - ballRadius + speedY <= BORDER_SIZE)
    {
        ballY = BORDER_SIZE + ballRadius;
        speedY *= -1;
    }
    else
    {
        ballX += speedX;
        ballY += speedY;
    }
}

void drawBall()
{
    ballCollision();

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

void scoreValidation(GLvoid)
{
    if (rightScore == WINCONDITION || leftScore == WINCONDITION)
    {
        displayWinner();
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

void keyboard(char unsigned key, GLint x, GLint y)
{

    glutPostRedisplay();
}

void onKeyDown(unsigned char key, int x, int y)
{
    // Ignore key repeat events
    if (!glutGetModifiers())
    {
        keystates[key] = true;
    }

    // Pause and ESC key implementation
    switch (key)
    {
    case 'r':
    case 'R':
        if (rightScore == WINCONDITION || leftScore == WINCONDITION)
        {
            cout << "r" << endl;
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
}

void onKeyUp(unsigned char key, int x, int y)
{
    keystates[key] = false;
}

void onSpecialDown(int key, int x, int y)
{
    specialKeyStates[key] = true;
}

void onSpecialUp(int key, int x, int y)
{
    specialKeyStates[key] = false;
}

void draw(GLvoid)
{
    glutSpecialFunc(onSpecialDown);
    glutSpecialUpFunc(onSpecialUp);
    glutKeyboardFunc(onKeyDown);
    glutKeyboardUpFunc(onKeyUp);

    // Right racket movement
    if (specialKeyStates[101] == true)
    {
        if (rightRacketY - racketsSpeed <= BORDER_SIZE)
        {
            rightRacketY = BORDER_SIZE + 10;
            rightRacketYf = rightRacketY + 60;
        }

        else
        {
            rightRacketY -= racketsSpeed;
            rightRacketYf -= racketsSpeed;
        }
    }

    if (specialKeyStates[103] == true)
    {
        if (rightRacketYf + racketsSpeed >= HEIGHT - BORDER_SIZE)
        {
            rightRacketYf = HEIGHT - BORDER_SIZE - 10;
            rightRacketY = rightRacketYf - 60;
        }

        else
        {
            rightRacketY += racketsSpeed;
            rightRacketYf += racketsSpeed;
        }
    }

    // Left racket movement
    if (keystates[119] == true)
    {
        if (leftRacketY - racketsSpeed <= BORDER_SIZE)
        {
            leftRacketY = BORDER_SIZE + 10;
            leftRacketYf = leftRacketY + 60;
        }

        else
        {
            leftRacketY -= racketsSpeed;
            leftRacketYf -= racketsSpeed;
        }
    }

    if (keystates[115] == true)
    {
        if (leftRacketYf + racketsSpeed >= HEIGHT - BORDER_SIZE)
        {
            leftRacketYf = HEIGHT - BORDER_SIZE - 10;
            leftRacketY = leftRacketYf - 60;
        }

        else
        {
            leftRacketY += racketsSpeed;
            leftRacketYf += racketsSpeed;
        }
    }

    if (leftScore == WINCONDITION || rightScore == WINCONDITION)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        displayWinner();
        glFlush();
        glutSwapBuffers();
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
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT);
            // displayScore();
            displayPause();
            glFlush();
            glutSwapBuffers();
        }
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

    cout << rightRacketX << endl;
    cout << rightRacketXf << endl;
    initGlut(&argc, argv);
    createWindow();
    glClear(GL_COLOR_BUFFER_BIT);
    glutDisplayFunc(draw);
    glutMainLoop();
    return 0;
}

/*  Helpfull content

https://stackoverflow.com/questions/35514902/glutinitdisplaymodeglut-double-glut-rgb-glut-depth-explanation

https://www.youtube.com/watch?v=-IlxN9UjU_8&t=737s

*/