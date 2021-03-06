#include <iostream>
#include <cmath>
#define PI 3.1415926535
#define P2 PI/2
#define P3 3*PI/2
#define DR 0.0174533 // Deg -> Rad

// compatibility with original GLUT
#if !defined(GLUT_WINDOW_SCALE)
#	define GLUT_WINDOW_SCALE 199
#endif

#define GL_SILENCE_DEPRECATION

#include <GLUT/glut.h>

int scale;
float px, py, pdx, pdy, pa;

int mapX = 8,
    mapY = 8,
    mapS = 25;

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
            if (map[y * mapY + x]) {
                glColor3f(1, 1, 1);

                xo = x * mapS;
                yo = y * mapS;

                glBegin(GL_QUADS);
                glVertex2i(xo + 1, yo + 1);
                glVertex2i(xo + 1, yo + mapS - 1);
                glVertex2i(xo + mapS - 1, yo + mapS - 1);
                glVertex2i(xo + mapS - 1, yo + 1);
                glEnd();

            } else
                glColor3f(0, 0, 0);


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

    // Draw eye-line
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2i(px, py);
    glVertex2i(px + pdx * 2, py + pdy * 2);
    glEnd();
}

float dist(float ax, float ay, float bx, float by, float ang) {
    return (sqrt((bx - ax) * (bx - ax) + (by - ay)*(by - ay)));
}

void drawRays() {
    // Floor
    glColor3f(0.2, 0.5, 0.2);
    glBegin(GL_QUADS);
    glVertex2i(0, 300);
    glVertex2i(0 + 800, 300);
    glVertex2i(800, 600);
    glVertex2i(0, 600);
    glEnd();


    // Rays
    int r, mx, my, mp, dof;
    float rx, ry, ra, xo, yo, aTan, nTan, hx, hy, vx, vy, disT;

    ra = pa - DR * 30; // Angle from player
    if (ra < 0)
        ra += 2 * PI;
    if (ra > 2 * PI)
        ra -= 2 * PI;
    for (r = 0; r < 60; r++) {
        // --- Check Horizontal Lines ---
        dof = 0;
        float disH = 100000000;
        hx = px; // Check distance with Height/Width
        hy = py;
        aTan = -1/tan(ra);

        if (ra > PI) { // Looking Down
            ry = (((int) py / mapS) * mapS) - 0.0001; // div 64 mul 64 + accuracy
            rx = (py - ry) * aTan + px;
            yo = -mapS;
            xo = -yo * aTan;
        }

        if (ra < PI) { // Looking Up
            ry = (((int) py / mapS) * mapS) + mapS; // div 64 mul 64 + accuracy
            rx = (py - ry) * aTan + px;
            yo = mapS;
            xo = -yo * aTan;
        }

        if (ra == 0 || ra == PI) { // Parallel lines
            rx = px;
            py = py;
            dof = 8;
        }

        while (dof < 8) {
            mx = (int)(rx) / mapS;
            my = (int)(ry) / mapS;

            mp = my * mapX + mx;

            if (mp > 0 && mp < mapX * mapY && map[mp]) { // Hit wall
                dof = 8;
                hx = rx;
                hy = ry;
                disH = dist(px, py, hx, hy, ra);
            } else {
                rx += xo;
                ry += yo;
                dof++; // Next line
            }
        }

        // Draw ray
        /* glColor3f(0, 1, 0);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd(); */

        // --- Check Vertical Lines ---
        dof = 0;
        float disV = 100000000;
        vx = px;
        vy = py;
        nTan = -tan(ra);

        if (ra > P2 && ra < P3) { // Looking left
            rx = (((int) px / mapS) * mapS) - 0.0001; // div 64 mul 64 + accuracy
            ry = (px - rx) * nTan + py;
            xo = -mapS;
            yo = -xo * nTan;
        }

        if (ra < P2 || ra > P3) { // Looking right
            rx = (((int) px / mapS) * mapS) + mapS; // div 64 mul 64 + accuracy
            ry = (px - rx) * nTan + py;
            xo = mapS;
            yo = -xo * nTan;
        }

        if (ra == 0 || ra == PI) { // Parallel lines (straight up/down)
            rx = px;
            py = py;
            dof = 8;
        }

        while (dof < 8) {
            mx = (int)(rx) / mapS;
            my = (int)(ry) / mapS;

            mp = my * mapX + mx;

            if (mp > 0 && mp < mapX * mapY && map[mp]) { // Hit wall
                dof = 8;

                // Check dist and save it
                vx = rx;
                vy = ry;
                disV = dist(px, py, vx, vy, ra);
            } else {
                rx += xo;
                ry += yo;
                dof++; // Next line
            }
        }

        // Check for shortest distance
        if (disV < disH) {
            glColor3f(0.9, 0, 0);

            rx = vx;
            ry = vy;
            disT = disV;
        }
        if (disH < disV) {
            glColor3f(0.7, 0, 0);

            rx = hx;
            ry = hy;
            disT = disH;
        }

        // Draw ray
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2i(px, py);
        glVertex2i(rx, ry);
        glEnd();

        // --- Draw 2.5D ---
        float ca = pa - ra; // Fix fish-eye
        if (ca < 0)
            ca += 2 * PI;
        if (ca > 2 * PI)
            ca -= 2 * PI;
        disT = disT * cos(ca);

        float lineH = (mapS * 600) / disT; // Line Height
        if (lineH > 600) lineH = 600;
        float lineO = 300 - lineH/2;

        // Draw line x2
        /*glLineWidth(8);
        glBegin(GL_LINES);
        glVertex2i(r * 8, lineO);
        glVertex2i(r * 8, lineH + lineO);
        glEnd(); */
        int width = 16;
        glBegin(GL_QUADS);
        glVertex2i(r * width, lineO);
        glVertex2i(r * width + width, lineO);
        glVertex2i(r * width + width, lineH + lineO);
        glVertex2i(r * width, lineH + lineO);
        glEnd();


        // Next ray
        ra += DR;
        if (ra < 0)
            ra += 2 * PI;
        if (ra > 2 * PI)
            ra -= 2 * PI;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawRays();
    drawMap();
    drawPlayer();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
            pa -= 0.1;
            if (pa < 0)
                pa += 2 * PI;

            pdx = cos(pa) * 5;
            pdy = sin(pa) * 5;
            break;
        case 'd':
            pa += 0.1;
            if (pa > 2 * PI)
                pa -= 2 * PI;

            pdx = cos(pa) * 5;
            pdy = sin(pa) * 5;
            break;
        case 'w':
            px += pdx;
            py += pdy;
            break;
        case 's':
            px -= pdx;
            py -= pdy;
            break;
        default:
            break;
    }

    glutPostRedisplay();
}

void init() {
    glClearColor(0.3, 0.3, 0.3, 0);
    gluOrtho2D(0, 800, 600, 0);

    px = 100;
    py = 70;
    pdx = cos(pa) * 5;
    pdy = sin(pa) * 5;
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayString("rgb double hidpi");
    glutInitWindowSize(400, 300);
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
