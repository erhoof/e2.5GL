#include <iostream>

// compatibility with original GLUT
#if !defined(GLUT_WINDOW_SCALE)
#	define GLUT_WINDOW_SCALE 199
#endif

#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>

int scale;
float px, py;

int mapX = 8,
    mapY = 8,
    mapS = 16;

int map[] = {
        1,1,1,1,1,1,1,1,
        1,0,1,0,0,0,0,1,
        1,0,1,0,0,0,0,1,
        1,0,1,0,0,0,0,1,
        1,0,0,0,0,0,0,1,
        1,0,0,0,0,1,1,1,
        1,0,0,0,0,0,0,1,
        1,1,1,1,1,1,1,1,
};

void drawMap() {
    int x, y, xo, yo;

    for (y = 0; y < mapY; y++) {
        for (x = 0; x < mapX; x++) {
            if (map[y * mapY + x])
                glColor3f(1, 1, 1);
            else
                glColor3f(0, 0, 0);

            xo = x * mapS + 10;
            yo = y * mapS + 10;

            glBegin(GL_QUADS);
            glVertex2i(xo + 1, yo + 1);
            glVertex2i(xo + 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + mapS - 1);
            glVertex2i(xo + mapS - 1, yo + 1);
            glEnd();
        }
    }
}

void drawPlayer() {
    glColor3f(1, 1, 0);
    glPointSize(8);

    // Draw character
    glBegin(GL_POINTS);
    glVertex2i(px, py);
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawMap();
    drawPlayer();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            px -= 10;
            break;
        case 'd':
            px += 10;
            break;
        case 'w':
            py -= 10;
            break;
        case 's':
            py += 10;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void init() {
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 800, 600, 0);
    //glViewport(400,300,400,300);


    px = 100;
    py = 100;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayString("rgb double hidpi");
    glutInitWindowSize(800, 800);
    glutCreateWindow("e2.5GL");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);

    // HIDPI Support
    int W = 800;
    int H = 600;
    int s0 = glutGet(GLUT_WINDOW_SCALE);
    if(s0 < 0) // unsupported by original GLUT
        s0 = 1;
    printf("%i - %i : %i\n", W, H, s0);
    glutReshapeWindow(W*s0, H*s0);
    glutPositionWindow(0, 0);

    glutMainLoop();
    return 0;
}
