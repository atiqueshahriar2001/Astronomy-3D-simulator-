#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <cstdlib>

float cameraAngle = 0.0f;
float cameraHeight = 20.0f;
float cameraDistance = 60.0f;

bool pauseAnimation = false;
bool showOrbits = true;
bool lightingEnabled = true;

int windowWidth = 1200;
int windowHeight = 800;
float globalTime = 0.0f;

struct Star { float x, y, z; };
std::vector<Star> stars;

struct Planet {
    float distance, radius;
    float orbitSpeed, rotationSpeed;
    float orbitAngle, rotationAngle;
    float r, g, b;
    bool hasMoon;
    float moonDistance, moonSpeed, moonAngle;
    bool hasRing;
};
std::vector<Planet> planets;

void initStars() {
   for (int i = 0; i < 2000; i++) { 
    Star s;
    s.x = (rand() % 4000 - 2000) / 10.0f; 
    s.y = (rand() % 4000 - 2000) / 10.0f;
    s.z = -50.0f - (rand() % 400);        
    stars.push_back(s);
}}

void drawSphere(float r) {
    GLUquadric* q = gluNewQuadric();
    gluSphere(q, r, 40, 40);
    gluDeleteQuadric(q);
}

void drawSunRays(float innerRadius, float outerRadius, float rotationAngle) {
    glDisable(GL_LIGHTING);
    glLineWidth(2.0f);
    glBegin(GL_LINES);

    int numRays = 200;
    for (int i = 0; i < numRays; i++) {
        float theta = ((float)rand() / RAND_MAX) * 2 * M_PI + rotationAngle;
        float phi = ((float)rand() / RAND_MAX) * M_PI;

        float x = sin(phi) * cos(theta);
        float y = cos(phi);
        float z = sin(phi) * sin(theta);

        float pulse = 0.7f + 0.3f * sin(globalTime * 5 + i);

        glColor3f(1.0f, 0.3f, 0.0f); 
        glVertex3f(innerRadius * x, innerRadius * y, innerRadius * z);

        glColor3f(1.0f * pulse, 0.3f * pulse, 0.0f * pulse);
        glVertex3f(outerRadius * x, outerRadius * y, outerRadius * z);
    }

    glEnd();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void drawSun(float radius) {
    glDisable(GL_LIGHTING);

    glColor3f(1.0f, 0.4f, 0.1f);
    drawSphere(radius);

    for (int i = 1; i <= 3; i++) { 
    float scale = radius + i;  
    float alpha = 0.25f - i * 0.05f;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(1.0f, 0.3f, 0.0f, alpha); 
    drawSphere(scale);
    glDisable(GL_BLEND);
    }
    drawSunRays(radius + 0.5f, radius * 2.4f, globalTime * 0.5f);

    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void drawOrbit(float radius) {
    if (!showOrbits) return;
    glDisable(GL_LIGHTING);
    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 120; i++) {
        float a = 2 * M_PI * i / 120;
        glVertex3f(radius * cos(a), 0, radius * sin(a));
    }
    glEnd();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void drawRing(float inner, float outer) {
    glDisable(GL_LIGHTING);
    glColor3f(0.8f, 0.7f, 0.5f);
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= 100; i++) {
        float angle = 2 * M_PI * i / 100;
        float x = cos(angle), z = sin(angle);
        glVertex3f(inner * x, 0, inner * z);
        glVertex3f(outer * x, 0, outer * z);
    }
    glEnd();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void drawStarfield() {
    glDisable(GL_LIGHTING);
    glPointSize(3);
    glBegin(GL_POINTS);
    for (auto &s : stars) {
        float twinkle = 0.6f + 0.4f * sin(globalTime + s.x);
        glColor3f(twinkle, twinkle * 0.9f, twinkle * 0.3f);
        glVertex3f(s.x, s.y, s.z);
    }
    glEnd();
    if (lightingEnabled) glEnable(GL_LIGHTING);
}

void drawPlanet(Planet &p) {
    glPushMatrix();
    glRotatef(p.orbitAngle, 0,1,0);
    glTranslatef(p.distance, 0, 0);
    glRotatef(p.rotationAngle, 0,1,0);
    glColor3f(p.r, p.g, p.b);
    drawSphere(p.radius);

    if (p.hasRing) {
        glRotatef(90, 1,0,0);
        drawRing(p.radius + 1, p.radius + 3);
    }

    if (p.hasMoon) {
        glPushMatrix();
        glRotatef(p.moonAngle, 0,1,0);
        glTranslatef(p.moonDistance, 0, 0);
        glColor3f(0.8,0.8,0.8);
        drawSphere(0.7);
        glPopMatrix();
    }
    glPopMatrix();
}

void initPlanets() {
    planets.push_back({10,1.2,4,2,0,0,0.7,0.7,0.7,false,0,0,0,false});
    planets.push_back({16,1.8,2.5,1,0,0,0.9,0.7,0.4,false,0,0,0,false});
    planets.push_back({24,2,1.8,3,0,0,0.2,0.5,1,true,4,6,0,false});
    planets.push_back({32,1.5,1.4,2,0,0,0.8,0.3,0.2,false,0,0,0,false});
    planets.push_back({45,4.5,0.8,4,0,0,0.8,0.6,0.4,false,0,0,0,false});
    planets.push_back({58,3.8,0.6,3,0,0,0.9,0.8,0.5,false,0,0,0,true});
    planets.push_back({72,3.2,0.4,2.5,0,0,0.6,0.9,0.9,false,0,0,0,false});
    planets.push_back({88,3.0,0.3,2.0,0,0,0.1,0.2,0.8,false,0,0,0,false});
}

void init() {
    glEnable(GL_DEPTH_TEST);
    GLfloat ambient[] = {0.2,0.2,0.2,1};
    GLfloat diffuse[] = {1,1,1,1};
    GLfloat specular[] = {1,1,1,1};
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glClearColor(0.0f, 0.0f, 0.1f, 1);
    initStars();
    initPlanets();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (double)windowWidth/windowHeight, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float camX = cameraDistance * sin(cameraAngle * M_PI/180);
    float camZ = cameraDistance * cos(cameraAngle * M_PI/180);
    gluLookAt(camX, cameraHeight, camZ, 0,0,0, 0,1,0);

    GLfloat lightPos[] = {0,0,0,1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    if (lightingEnabled) glEnable(GL_LIGHTING);
    else glDisable(GL_LIGHTING);

    drawStarfield();

    glPushMatrix();
    glRotatef(globalTime * 10, 0,1,0);
    drawSun(3); 
    glPopMatrix();

    for (auto &p : planets) {
        drawOrbit(p.distance);
        drawPlanet(p);
    }

    glutSwapBuffers();
}

void update(int) {
    if (!pauseAnimation) {
        globalTime += 0.02f;
        for (auto &p : planets) {
            p.orbitAngle += p.orbitSpeed * 0.4f;
            p.rotationAngle += p.rotationSpeed;
            if (p.hasMoon)
                p.moonAngle += p.moonSpeed;
        }
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int) {
    switch(key) {
        case 27: exit(0);
        case ' ': pauseAnimation = !pauseAnimation; break;
        case 'o': showOrbits = !showOrbits; break;
        case 'l': lightingEnabled = !lightingEnabled; break;
        case '+': cameraDistance = std::max(10.0f, cameraDistance-3); break;
        case '-': cameraDistance += 3; break;
        case 'w': cameraHeight += 2; break;
        case 's': cameraHeight -= 2; break;
        case 'a': cameraAngle -= 3; break;
        case 'd': cameraAngle += 3; break;
    }
}

void reshape(int w,int h) {
    windowWidth = w;
    windowHeight = h;
    glViewport(0,0,w,h);
}

int main(int argc,char** argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth,windowHeight);
    glutCreateWindow("Astronomy 3D Simulator");
    init();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    glutMainLoop();
    return 0;
}