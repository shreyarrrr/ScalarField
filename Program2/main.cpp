/*
    Hello Triangle, OpenGL 2 level
    No shaders
*/

#include "GL/glut.h"
#include <Windows.h>
#include <math.h>
#include <iostream>
static int window;
static int menu_id;
static int defines;
static int selects;
static int eval;

static int width = 600;
static int height = 600;
static int value = 0;

static float lines[3][40]; //pt 1, pt 2, color
static int points[2][40];
static float line[2];
static int init_coordinates[3]; //x, y, i
static int selector_coord[2];

static int division = 10;
static int index = 0;
static int move_ind = 40;
static bool draw = true;
static bool select = false;

float groupProduct(int color, float x, float y);

void init() {
  // Set initial OpenGL states
    
}
void menu(int num) {
    if (num == 0) {
        glutDestroyWindow(window);
        exit(0);
    }
    else {
        value = num;
    }
    glutPostRedisplay();
}
void createMenu(void) {
    defines = glutCreateMenu(menu);
    glutAddMenuEntry("Define line", 1);
    glutAddMenuEntry("Move line", 2);
    glutAddMenuEntry("Delete line", 3);
    glutAddMenuEntry("Do nothing", 4);
    glutAddMenuEntry("Reset", 5);
    glutAddMenuEntry("Exit", 0);

    selects = glutCreateMenu(menu);
    glutAddMenuEntry("Select A", 6);
    glutAddMenuEntry("Select B", 7);
    glutAddMenuEntry("Unselect", 8);
    glutAddMenuEntry("Define Selector", 9);
    glutAddMenuEntry("Move Selector", 10);

    eval = glutCreateMenu(menu);
    glutAddMenuEntry("Density", 11);

    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Define", defines);
    glutAddSubMenu("Select", selects);
    glutAddSubMenu("Eval", eval);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}
// display function
void display() {
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    /*Program 1
    glColor3f(0.4, 0.0, 0.0);
    glPointSize(3.0);
    glBegin(GL_LINES);
    glVertex2d(0.0f, 0.0f);
    glVertex2d(1.0f, 0.0f);
    glEnd();

    glColor3f(0.0, 0.4, 0.0);
    glBegin(GL_LINES);
    glVertex2d(0.0f, 0.0f);
    glVertex2d(0.0f, 1.0f);
    glEnd();

    glColor3f(0.4, 0.4, 0.0);
    glBegin(GL_LINES);
    glVertex2d(0.0f, 0.0f);
    glVertex2d(-1.0f, -1.0f);
    glEnd();
    */

    //Reset
    if (value == 5) {
        index = 0;
        draw = true;
    }
    //Draw points
    for (int i = 0; i < index; i++) {
        glColor3f(0.0, 0.5, 0.5);
        glPointSize(5);
        glBegin(GL_POINTS);
        glVertex2f(points[0][i] / (width / 2.0) - 1, 1 - points[1][i] / (width / 2.0));
        glEnd();
    }
    //Draw selector point
    glColor3f(0.0, 0.0, 0.5);
    glPointSize(5);
    glBegin(GL_POINTS);
    glVertex2f(selector_coord[0] / (width / 2.0) - 1, 1 - selector_coord[1] / (width / 2.0));
    glEnd();

    //Draw lines
    for (int i = 0; i < index - 1; i += 2) {
        if (lines[2][i] == 0)
            glColor3f(0.0, 0.5, 0.5);
        else if (lines[2][i] == 1)
            glColor3f(0.5, 0.0, 0.5);
        else if (lines[2][i] == 2)
            glColor3f(0.5, 0.5, 0.0);

        glBegin(GL_LINES);
        glVertex2d(lines[0][i] / (width / 2.0) - 1, 1 - lines[1][i] / (width / 2.0));
        glVertex2d(lines[0][i + 1] / (width / 2.0) - 1, 1 - lines[1][i + 1] / (width / 2.0));
        glEnd();
    }

    //Draw field
    if (value == 9 && selector_coord[0] != 0) {
        float productA = groupProduct(1, selector_coord[0], selector_coord[1]);
        float productB = groupProduct(2, selector_coord[0], selector_coord[1]);
        float scalarS = productA / (productA - productB);
        printf("%f %f ", productA, productB);
       for (float i = 0; i <= width; i += width / division / 2) {
            for (float j = 0; j <= width; j += width / division / 2) {
                float a = groupProduct(1, i, j);
                printf("%f ", a);
                float b = groupProduct(2, i, j);
                printf("%f ", b);
                float result = (1 - scalarS) * a + scalarS * b;
                printf("%f \n", result);

                if (result > 0)
                    glColor3f(0.5, 0.5, 0.0);
                else if (result < 0)
                    glColor3f(0.5, 0.0, 0.0);
                else {
                    glColor3f(0.0, 0.0, 0.0);
                    printf("***(%f, %f)", i, j);
                }

                glPointSize(5);
                glBegin(GL_POINTS);
                glVertex2f(i / (width / 2.0) - 1, 1 - j / (width / 2.0));
                glEnd();
            }
        }
    }
    
    // Swap buffers for animation
    glutSwapBuffers();
}
void createLine(int ind) {

    float m = (points[1][ind] - points[1][ind - 1])*1.0 / (points[0][ind] - points[0][ind - 1]);
    float b = points[1][ind] - m * points[0][ind];
    lines[0][ind] = 1000;
    lines[1][ind] = m * 1000 + b;
    lines[0][ind - 1] =-1000;
    lines[1][ind - 1]= m * -1000 + b;
}
void findLine(int ind, int x, int y) {
    float m = (points[1][ind] - points[1][ind - 1]) * 1.0 / (points[0][ind] - points[0][ind - 1]);
    float b = points[1][ind] - m * points[0][ind];
    int y_eq = m * x + b;
    //printf("x = %d, y = %d -> %d = %fx + %f = %d\n", x, y, y / 15 * 15, m, b, y_eq / 15 * 15);
    if (y / 15 * 15 == y_eq / 15 * 15) {
        select = true;
    }
    if (select) {
        line[0] = m;
        line[1] = b;
    }
}
void deleteLine(int ind, int x, int y) {
    float m = (points[1][ind] - points[1][ind - 1]) * 1.0 / (points[0][ind] - points[0][ind - 1]);
    float b = points[1][ind] - m * points[0][ind];
    int y_eq = m * x + b;
    if (y / 15 * 15 == y_eq / 15 * 15) {
        points[0][ind] = 0;
        points[1][ind] = 0;
        points[0][ind-1] = 0;
        points[1][ind-1] = 0;
        createLine(ind);
    }
}
void colorLine(int ind, int x, int y) {
    float m = (points[1][ind] - points[1][ind - 1]) * 1.0 / (points[0][ind] - points[0][ind - 1]);
    float b = points[1][ind] - m * points[0][ind];
    int y_eq = m * x + b;
    if (y / 15 * 15 == y_eq / 15 * 15) {
        if (value == 6) {
            lines[2][ind] = 1;
            lines[2][ind - 1] = 1;
        }
        else if (value == 7) {
            lines[2][ind] = 2;
            lines[2][ind - 1] = 2;
        }
        else if (value == 8) {
            lines[2][ind] = 0;
            lines[2][ind - 1] = 0;
        }
    }
}
float groupProduct(int color, float x, float y) {
    float product = 1;
    for (int ind = 0; ind <= index; ind += 2) {
        if (lines[2][ind] == color) {
            float m = (points[1][ind] - points[1][ind + 1]) * 1.0 / (points[0][ind] - points[0][ind + 1]);
            float b = points[1][ind] - m * points[0][ind];
            product *= (y - m * x - b)/(sqrt(pow(m, 2) + 1));
        }
    }

    return product;
}
/*
Function invoked when an event on a regular keys occur.
*/
void keyboard(unsigned char k, int x, int y)
{
  // Close application if ESC is pressed 
  if (k == 27)
  {
    exit(0);
  }
}
/*
Mouse Handler
*/
void mouseClick(int button, int mode, int x, int y) {
  // event happens once on pushing the button and once more when releasing it
  // button identifies what you clicked, mode if its down or up
    //printf("%d %d %d %d\n", button, mode, x, y);
    if (index <= 40 && value == 1 && button == GLUT_LEFT_BUTTON && mode == GLUT_DOWN) {
        if (draw) {
            points[0][index] = x;
            points[1][index] = y;
            index++;
            draw = false;
        }
        else {
            points[0][index] = x;
            points[1][index] = y;
            index++;
            draw = true;
            createLine(index - 1);
        }
    }
    if (value == 2) {
        if (mode == GLUT_DOWN) {
            //move points
            move_ind = 40;
            for (int i = 0; i <= index; i++) {
                if (round(x / 15) * 15 == round(points[0][i] / 15) * 15 &&
                    round(y / 15) * 15 == round(points[1][i] / 15) * 15) {
                    move_ind = i;
                    printf("%d\n", i);
                }
            }
            //shift line
            if (move_ind == 40) {
                for (int i = 0; i <= index; i+=2) {
                    findLine(i+1, x, y);
                    if (select) {
                        init_coordinates[0] = x;
                        init_coordinates[1] = y;
                        init_coordinates[2] = i;
                        printf("i = %d", i);
                        break;
                    }
                }
            }
        }
        else if (mode == GLUT_UP) {
            //move points
            if (move_ind != 40) {
                points[0][move_ind] = x;
                points[1][move_ind] = y;
                if (move_ind % 2 == 1)
                    createLine(move_ind);
                else
                    createLine(move_ind + 1);
            }
            //shift line
            if (select) {
                //printf("***x-shift:%d, y-shift:%d\n", x - init_coordinates[0], y - init_coordinates[1]);
                points[0][init_coordinates[2]] += x - init_coordinates[0];
                points[1][init_coordinates[2]] += y - init_coordinates[1];
                points[0][init_coordinates[2]+1] += x - init_coordinates[0];
                points[1][init_coordinates[2]+1] += y - init_coordinates[1];
                createLine(init_coordinates[2] +1);
                select = false;
            }
        }
    }
    if (value == 3) {
        for (int i = 0; i <= index; i += 2) {
            deleteLine(i + 1, x, y);
        }
    }
    if (value == 6 || value == 7 || value == 8) {
        for (int i = 0; i <= index; i += 2) {
            colorLine(i + 1, x, y);
        }
    }
    if (value == 9 && mode == GLUT_UP) {
        selector_coord[0] = x;
        selector_coord[1] = y;
    }
   
}
void mouseMotion(int x, int y) {
  // called when the mouse moves
  // active motion means a button is down, passive means it is up
}

void resize_viewport(int w, int h) {
    if (w >= h) {
        glViewport(0, 0, h, h);
        width = h;
        height = h;
    }
    else {
        glViewport(0, 0, w, w);
        width = w;
        height = w;
    }
}

/*
The main function.
*/
int main(int argc, char** argv)
{
  // Initialize the GLUT window
  glutInit(&argc, argv);
  glutInitWindowSize(800, 600);
  glutInitWindowPosition(30, 30);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  window = glutCreateWindow("Program 2");
  createMenu();
  
  glutReshapeFunc(resize_viewport);
  glutDisplayFunc(display);
  glutMouseFunc(mouseClick);
  glutMotionFunc(mouseMotion);

  glutMainLoop();
  return 0;
}