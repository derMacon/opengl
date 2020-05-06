
#include <GL/gl.h>
#include <GL/glut.h>
#include "scene.h"
#include "debug.h"
#include "logic.h"
#include "math.h"
#include "variables.h"
#include "types.h"
#include <time.h>

GLboolean show_wireframe = GL_FALSE;
static Block bloecke[BLOCKS_COUNT];
int tAngle = 0;

float stick_width = STICK_WIDTH;

static void drawTriangle(void) {
    glBegin(GL_TRIANGLES);

    float x = 0.7f;
    glColor3f(1, 0.843, 0);

    // Links unten
    glVertex2f(-x, -x);

    // Rechts unten
    glVertex2f(x, -x);

    // oben
    glVertex2f(0, x);
    glEnd();
}

/**
 * Zeichnet ein Rechteck mit der Breite und Hoehe 1.
 */
static void drawSquare(void) {
    glBegin(GL_QUADS);
    {
        // Links unten
        glVertex2f(-0.5, -0.5);

        // Rechts unten
        glVertex2f(0.5, -0.5);

        // Links oben
        glVertex2f(0.5, 0.5);

        // Rechts oben
        glVertex2f(-0.5, 0.5);
    }
    glEnd();
}

static void drawRound(void) {

    float radius = 1.0f;

    // TODO: WTF is delta theta
    float delta_theta = 0.01f;

    glBegin(GL_POLYGON);

    // TODO: Erklaerung hierfuer finden xD
    for (float angle = 0; angle < 2 * M_PI; angle += delta_theta) {
        glVertex3f(radius * cos(angle), radius * sin(angle), 0);
    }

    glEnd();
}

void drawExtra(const CGPoint2f coords) {
    GLfloat x = coords[0];
    GLfloat y = coords[1];

    glColor3f(0.0f, 1.0f, 0.0f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glRotatef(tAngle, 0, 0, 1);

        glScalef(EXTRA_WIDTH, EXTRA_HEIGHT, 2.0f);
        drawTriangle();

        tAngle++;
        if (tAngle > 360) {
            tAngle = 0;
        }
    }

    glPopMatrix();
}

static void drawBall(const CGPoint2f coords) {
    GLfloat x = coords[0];
    GLfloat y = coords[1];
    glColor3f(0.827f, 0.827f, 0.827f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(BALL_WIDTH, BALL_WIDTH, 1.0f);
        drawRound();
    }

    glPopMatrix();
}

float *selectColor(int randomNumber) {

    float *colors = malloc(3);

    switch (randomNumber) {
        // Lila
        case 1:
            colors[0] = 0.576f;
            colors[1] = 0.439f;
            colors[2] = 0.859f;
            break;

            // Rot
        case 2:
            colors[0] = 0.980f;
            colors[1] = 0.502f;
            colors[2] = 0.447f;
            break;

            // Blau
        case 3:
            colors[0] = 0.941f;
            colors[1] = 0.902f;
            colors[2] = 0.549f;
            break;

            // Gruen
        case 4:
            colors[0] = 0.596f;
            colors[1] = 0.984f;
            colors[2] = 0.596f;
            break;

            // Gelb
        case 5:
            colors[0] = 0.529f;
            colors[1] = 0.808f;
            colors[2] = 0.922f;
            break;

        default:
            colors[0] = 0.941f;
            colors[1] = 1.000f;
            colors[2] = 1.000f;
            break;
    }

    return colors;
}

/**
 * Generiert fuer die Bloecke Nummern, die anhand von selectColor in Farben umgewandelt werden.
 * @param maxNumber hoechste Nummer (standardmaessig 5)
 * @return
 */
int genNumber(int maxNumber) {
    return (rand() % (maxNumber - 1 + 1)) + 1;
}

static void drawBlock(const Block block) {
    GLfloat x = block.position[0];
    GLfloat y = block.position[1];

    glColor3f(block.color[0], block.color[1], block.color[2]);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(BLOCK_WIDTH, BLOCK_HEIGHT, 1.0f);
        drawSquare();
    }

    glPopMatrix();
}

static void drawStick(const CGPoint2f coords) {

    GLfloat x = coords[0];
    GLfloat y = coords[1];

    glColor3f(1.0f, 1.0f, 1.0f);

    glPushMatrix();
    {
        glTranslatef(x, y, ZERO);
        glScalef(stick_width, BAR_THICKNESS, 1.0f);
        drawSquare();
    }

    glPopMatrix();
}

/**
 * Zeichnet einen Teil der Spielfeldbegrenzung.
 *
 */
static void drawBorder(GLfloat posX, GLfloat posY, int showTop) {

    glPushMatrix();
    {
        glColor3f(1.0f, 1.0f, 1.0f);
        if (showTop) {
            glTranslatef(posX, posY, ZERO);
            glScalef(BAR_WIDTH, BAR_HEIGHT, 1.0f);
        } else {
            glTranslatef(posX, posY, ZERO);
            glScalef(BAR_HEIGHT, BAR_WIDTH, 1.0f);
        }

        drawSquare();
    }
    glPopMatrix();
}

void generateBlocks(Block *block) {
    int count = 0;
    float width = 0.65f;
    float height = 0.7f;
    float gap = 0.2f;

    for (float j = width; j > -width; j -= gap) {
        for (float i = -height; i <= height; i += gap) {

            float *colors = selectColor(genNumber(5));

            block[count].color[0] = colors[0];
            block[count].color[1] = colors[1];
            block[count].color[2] = colors[2];

            block[count].hidden = 0;
            block[count].position[0] = i;
            block[count].position[1] = j;

            count++;

            free(colors);
        }
    }
}

/**
 * Initialisierung der Szene (inbesondere der OpenGL-Statusmaschine).
 * Setzt Hintergrund- und Zeichenfarbe.
 * @return Rueckgabewert: im Fehlerfall 0, sonst 1.
 */
int
initScene(void) {
    /* Setzen der Farbattribute */
    /* Hintergrundfarbe */
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);

    /* Zeichenfarbe */
    glColor3f(1.0f, 1.0f, 1.0f);

    /* Groese von Punkten */
    glPointSize(4.0f);

    /* Breite von Linien */
    glLineWidth(2.0f);

    // Zeit fuer Random einbeziehen, um Zufallsfarben zu generieren
    // srand steht hier, weil es nur einmal aufgerufen werden darf
    srand(time(NULL));

    /* Bloecke generieren */
    generateBlocks(bloecke);

    /* Alles in Ordnung? */
    return (GLGETERROR == GL_NO_ERROR);
}

/**
 * Schaltet den Wireframe-Modus an oder aus
 */
void toggleWireframe() {
    show_wireframe = !show_wireframe;
    if (show_wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

/**
 * Alle Bloecke zeichnen, wenn sie nicht versteckt sind und
 * der Ball gerade nicht kollidiert
 */
void drawBlocks() {
    for (int i = 0; i < BLOCKS_COUNT; i++) {
        if (!bloecke[i].hidden && checkBlockCollision(&bloecke[i])) {
            drawBlock(bloecke[i]);
        }
    }
}

void drawScene(void) {

    /* In der Logik berechnet Position beziehen */
    CGPoint2f *stickCenter = getStickCenter();
    CGPoint2f *ballCenter = getBallCenter();
    CGPoint2f *extraCenter = getExtraCenter();

    /* Rahmen zeichnen */
    drawBorder(BAR_X_OFFSET, ZERO, 1);
    drawBorder(-BAR_X_OFFSET, ZERO, 1);
    drawBorder(ZERO, BAR_X_OFFSET - BAR_WIDTH, 0);

    drawStick(*stickCenter);
    drawBall(*ballCenter);

    // TODO: Pruefen, dass ein neues Extra gespawnt werden muss?
    if (show_extra) {
        drawExtra(*extraCenter);
    }

    // Bloecke zeichnen
    drawBlocks();
}