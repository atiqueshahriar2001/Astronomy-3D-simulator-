#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

const int WINDOW_WIDTH = 1260;
const int WINDOW_HEIGHT = 800;
const float PI = 3.14159265358979323846f;
const float GUIDE_FRAME_WIDTH = 350.0f;
const float GUIDE_FRAME_HEIGHT = 185.0f;
const float SCENE_PADDING = 40.0f;

struct Point2D {
    float x;
    float y;
};

struct Matrix3 {
    float m[3][3];
};

struct Planet {
    std::string name;
    float baseOrbitRadius;
    float orbitRadius;
    float baseRadius;
    float radius;
    float orbitSpeed;
    float angle;
    float r;
    float g;
    float b;
};

std::vector<Point2D> stars;
std::vector<Planet> planets;

bool paused = false;
bool shearShipX = false;
bool shearShipY = false;
float shipRotation = 0.0f;
float shipScale = 1.0f;
float shipOffsetX = 0.0f;
float shipOffsetY = 0.0f;
float shootingStarX = -120.0f;
float shootingStarY = 720.0f;
float moonAngle = 0.0f;
float sunPulse = 0.0f;
int currentWindowWidth = WINDOW_WIDTH;
int currentWindowHeight = WINDOW_HEIGHT;

void plotPoint(int x, int y) {
    glVertex2i(x, y);
}

void beginPointDrawing(float size = 2.0f) {
    glPointSize(size);
    glBegin(GL_POINTS);
}

void endPointDrawing() {
    glEnd();
}

Matrix3 identityMatrix() {
    Matrix3 result = {};
    result.m[0][0] = 1.0f;
    result.m[1][1] = 1.0f;
    result.m[2][2] = 1.0f;
    return result;
}

Matrix3 multiplyMatrix(const Matrix3& a, const Matrix3& b) {
    Matrix3 result = {};
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            for (int k = 0; k < 3; ++k) {
                result.m[row][col] += a.m[row][k] * b.m[k][col];
            }
        }
    }
    return result;
}

Point2D transformPoint(const Point2D& p, const Matrix3& m) {
    Point2D result;
    result.x = (p.x * m.m[0][0]) + (p.y * m.m[0][1]) + m.m[0][2];
    result.y = (p.x * m.m[1][0]) + (p.y * m.m[1][1]) + m.m[1][2];
    return result;
}

Matrix3 translationMatrix(float tx, float ty) {
    Matrix3 result = identityMatrix();
    result.m[0][2] = tx;
    result.m[1][2] = ty;
    return result;
}

Matrix3 rotationMatrix(float angleDegrees) {
    Matrix3 result = identityMatrix();
    float rad = angleDegrees * PI / 180.0f;
    float c = std::cos(rad);
    float s = std::sin(rad);
    result.m[0][0] = c;
    result.m[0][1] = -s;
    result.m[1][0] = s;
    result.m[1][1] = c;
    return result;
}

Matrix3 scalingMatrix(float sx, float sy) {
    Matrix3 result = identityMatrix();
    result.m[0][0] = sx;
    result.m[1][1] = sy;
    return result;
}

Matrix3 shearMatrix(float shx, float shy) {
    Matrix3 result = identityMatrix();
    result.m[0][1] = shx;
    result.m[1][0] = shy;
    return result;
}

void drawLineDDA(int x1, int y1, int x2, int y2, float size = 2.0f) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));

    float xIncrement = steps == 0 ? 0.0f : static_cast<float>(dx) / steps;
    float yIncrement = steps == 0 ? 0.0f : static_cast<float>(dy) / steps;

    float x = static_cast<float>(x1);
    float y = static_cast<float>(y1);

    beginPointDrawing(size);
    for (int i = 0; i <= steps; ++i) {
        plotPoint(static_cast<int>(std::round(x)), static_cast<int>(std::round(y)));
        x += xIncrement;
        y += yIncrement;
    }
    endPointDrawing();
}

void drawLineBresenham(int x1, int y1, int x2, int y2, float size = 2.0f) {
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    beginPointDrawing(size);
    while (true) {
        plotPoint(x1, y1);
        if (x1 == x2 && y1 == y2) {
            break;
        }
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
    endPointDrawing();
}

void plotCircleSymmetry(int xc, int yc, int x, int y) {
    plotPoint(xc + x, yc + y);
    plotPoint(xc - x, yc + y);
    plotPoint(xc + x, yc - y);
    plotPoint(xc - x, yc - y);
    plotPoint(xc + y, yc + x);
    plotPoint(xc - y, yc + x);
    plotPoint(xc + y, yc - x);
    plotPoint(xc - y, yc - x);
}

void drawCircleMidpoint(int xc, int yc, int radius, float size = 2.0f) {
    int x = 0;
    int y = radius;
    int d = 1 - radius;

    beginPointDrawing(size);
    plotCircleSymmetry(xc, yc, x, y);
    while (x < y) {
        ++x;
        if (d < 0) {
            d += 2 * x + 1;
        } else {
            --y;
            d += 2 * (x - y) + 1;
        }
        plotCircleSymmetry(xc, yc, x, y);
    }
    endPointDrawing();
}

void drawFilledCircle(int xc, int yc, int radius) {
    for (int r = radius; r >= 1; --r) {
        drawCircleMidpoint(xc, yc, r, 1.5f);
    }
}

void drawPolygonOutline(const std::vector<Point2D>& points, bool useDDA) {
    if (points.size() < 2) {
        return;
    }

    for (size_t i = 0; i < points.size(); ++i) {
        Point2D a = points[i];
        Point2D b = points[(i + 1) % points.size()];
        if (useDDA) {
            drawLineDDA(static_cast<int>(std::round(a.x)), static_cast<int>(std::round(a.y)),
                        static_cast<int>(std::round(b.x)), static_cast<int>(std::round(b.y)));
        } else {
            drawLineBresenham(static_cast<int>(std::round(a.x)), static_cast<int>(std::round(a.y)),
                              static_cast<int>(std::round(b.x)), static_cast<int>(std::round(b.y)));
        }
    }
}

void drawFilledPolygon(const std::vector<Point2D>& points) {
    glBegin(GL_POLYGON);
    for (const Point2D& point : points) {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void drawRoundedPolygonBlob(float centerX, float centerY,
                            float radiusX, float radiusY,
                            float r, float g, float b, float alpha,
                            float angleOffset = 0.0f) {
    const int segments = 28;
    glColor4f(r, g, b, alpha);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i) {
        float angle = angleOffset + (2.0f * PI * static_cast<float>(i)) / static_cast<float>(segments);
        float wave = 0.88f + 0.14f * std::sin(angle * 3.0f + angleOffset * 1.7f)
                           + 0.08f * std::cos(angle * 5.0f - angleOffset);
        glVertex2f(centerX + std::cos(angle) * radiusX * wave,
                   centerY + std::sin(angle) * radiusY * wave);
    }
    glEnd();
}

void renderBitmapText(float x, float y, void* font, const std::string& text) {
    glRasterPos2f(x, y);
    for (char ch : text) {
        glutBitmapCharacter(font, ch);
    }
}

Point2D getOrbitPosition(float centerX, float centerY, float radius, float angleDegrees) {
    float rad = angleDegrees * PI / 180.0f;
    return {centerX + radius * std::cos(rad), centerY + radius * std::sin(rad)};
}

float clampValue(float value, float minValue, float maxValue) {
    if (value < minValue) {
        return minValue;
    }
    if (value > maxValue) {
        return maxValue;
    }
    return value;
}

float getMaxOrbitRadius() {
    float maxOrbitRadius = 0.0f;
    for (const Planet& planet : planets) {
        maxOrbitRadius = std::max(maxOrbitRadius, planet.baseOrbitRadius);
    }
    return maxOrbitRadius;
}

float getOrbitScaleFactor() {
    float availableHalfWidth = (static_cast<float>(currentWindowWidth) - GUIDE_FRAME_WIDTH - 2.0f * SCENE_PADDING) * 0.5f;
    float availableHalfHeight = (static_cast<float>(currentWindowHeight) - 2.0f * SCENE_PADDING) * 0.5f;
    float maxAvailableRadius = std::max(120.0f, std::min(availableHalfWidth, availableHalfHeight));
    float maxBaseRadius = getMaxOrbitRadius();

    if (maxBaseRadius <= 0.0f) {
        return 1.0f;
    }

    return std::min(1.0f, maxAvailableRadius / maxBaseRadius);
}

void updatePlanetLayout() {
    float orbitScale = getOrbitScaleFactor();

    for (Planet& planet : planets) {
        planet.orbitRadius = planet.baseOrbitRadius * orbitScale;
        planet.radius = std::max(4.0f, planet.baseRadius * (0.85f + 0.15f * orbitScale));
    }
}

Point2D getSolarSystemCenter() {
    float maxOrbitRadius = getMaxOrbitRadius();
    float minX = SCENE_PADDING + maxOrbitRadius;
    float minY = SCENE_PADDING + maxOrbitRadius;
    float maxX = static_cast<float>(currentWindowWidth) - SCENE_PADDING - GUIDE_FRAME_WIDTH - maxOrbitRadius;
    float maxY = static_cast<float>(currentWindowHeight) - SCENE_PADDING - maxOrbitRadius;

    Point2D center = {
        static_cast<float>(currentWindowWidth) * 0.5f,
        static_cast<float>(currentWindowHeight) * 0.5f
    };

    if (maxX >= minX) {
        center.x = clampValue(center.x, minX, maxX);
    } else {
        center.x = static_cast<float>(currentWindowWidth) * 0.5f;
    }

    if (maxY >= minY) {
        center.y = clampValue(center.y, minY, maxY);
    } else {
        center.y = static_cast<float>(currentWindowHeight) * 0.5f;
    }

    return center;
}

void initializeStars() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    stars.clear();
    for (int i = 0; i < 220; ++i) {
        stars.push_back({
            static_cast<float>(std::rand() % currentWindowWidth),
            static_cast<float>(std::rand() % currentWindowHeight)
        });
    }
}

void initializePlanets() {
    planets = {
        {"Mercury", 110.0f, 110.0f, 5.0f, 5.0f, 1.6f, 10.0f, 0.70f, 0.70f, 0.72f},
        {"Venus",   165.0f, 165.0f, 9.0f, 9.0f, 1.2f, 80.0f, 0.92f, 0.72f, 0.35f},
        {"Earth",   230.0f, 230.0f, 9.5f, 9.5f, 0.95f, 145.0f, 0.25f, 0.55f, 0.95f},
        {"Mars",    300.0f, 300.0f, 6.5f, 6.5f, 0.78f, 220.0f, 0.88f, 0.32f, 0.22f},
        {"Jupiter", 385.0f, 385.0f, 18.0f, 18.0f, 0.42f, 300.0f, 0.78f, 0.58f, 0.35f},
        {"Saturn",  470.0f, 470.0f, 15.0f, 15.0f, 0.32f, 20.0f, 0.85f, 0.78f, 0.45f},
        {"Uranus",  550.0f, 550.0f, 11.0f, 11.0f, 0.24f, 110.0f, 0.52f, 0.86f, 0.92f},
        {"Neptune", 620.0f, 620.0f, 10.5f, 10.5f, 0.18f, 250.0f, 0.24f, 0.42f, 0.92f},
        {"Pluto",   685.0f, 685.0f, 4.0f, 4.0f, 0.12f, 335.0f, 0.82f, 0.78f, 0.72f}
    };

    updatePlanetLayout();
}

void drawBackgroundStars() {
    beginPointDrawing(2.5f);
    for (size_t i = 0; i < stars.size(); ++i) {
        float twinkle = 0.5f + 0.5f * std::sin(sunPulse + static_cast<float>(i));
        glColor3f(0.7f + 0.3f * twinkle, 0.7f + 0.3f * twinkle, 1.0f);
        plotPoint(static_cast<int>(stars[i].x), static_cast<int>(stars[i].y));
    }
    endPointDrawing();
}

void drawNebula() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawRoundedPolygonBlob(250.0f, 620.0f, 210.0f, 120.0f, 0.26f, 0.16f, 0.40f, 0.10f, 0.2f);
    drawRoundedPolygonBlob(360.0f, 575.0f, 145.0f, 90.0f, 0.46f, 0.22f, 0.58f, 0.07f, 1.1f);

    drawRoundedPolygonBlob(910.0f, 610.0f, 220.0f, 125.0f, 0.10f, 0.40f, 0.52f, 0.09f, 0.45f);
    drawRoundedPolygonBlob(1040.0f, 565.0f, 160.0f, 95.0f, 0.18f, 0.56f, 0.68f, 0.06f, 1.6f);

    glDisable(GL_BLEND);
}

void drawAsteroidBelt(float centerX, float centerY) {
    float orbitScale = getOrbitScaleFactor();
    for (int i = 0; i < 90; ++i) {
        float angle = i * 4.0f + sunPulse * 8.0f;
        float radius = (285.0f + static_cast<float>(i % 5) * 7.0f) * orbitScale;
        Point2D rock = getOrbitPosition(centerX, centerY, radius, angle);
        glColor3f(0.48f, 0.42f, 0.38f);
        drawCircleMidpoint(static_cast<int>(rock.x), static_cast<int>(rock.y), 1, 1.0f);
    }
}

void drawOrbitRings(float centerX, float centerY) {
    for (const Planet& planet : planets) {
        glColor3f(0.25f, 0.30f, 0.42f);
        drawCircleMidpoint(static_cast<int>(centerX), static_cast<int>(centerY),
                           static_cast<int>(planet.orbitRadius), 1.0f);
    }
}

void drawSun(float centerX, float centerY) {
    int baseRadius = 22;
    int glowRadius = baseRadius + static_cast<int>(2.0f * std::sin(sunPulse));

    glColor3f(1.0f, 0.78f, 0.18f);
    drawFilledCircle(static_cast<int>(centerX), static_cast<int>(centerY), glowRadius);

    glColor3f(1.0f, 0.95f, 0.55f);
    drawFilledCircle(static_cast<int>(centerX), static_cast<int>(centerY), baseRadius - 8);

    glColor3f(1.0f, 0.65f, 0.10f);
    for (int i = 0; i < 12; ++i) {
        float angle = i * 30.0f + sunPulse * 20.0f;
        Point2D inner = getOrbitPosition(centerX, centerY, 28.0f, angle);
        Point2D outer = getOrbitPosition(centerX, centerY, 41.0f + 3.0f * std::sin(sunPulse + i), angle);
        drawLineDDA(static_cast<int>(inner.x), static_cast<int>(inner.y),
                    static_cast<int>(outer.x), static_cast<int>(outer.y), 2.0f);
    }
}

void drawPlanet(const Planet& planet, float centerX, float centerY) {
    Point2D pos = getOrbitPosition(centerX, centerY, planet.orbitRadius, planet.angle);

    glColor3f(planet.r, planet.g, planet.b);
    drawFilledCircle(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(planet.radius));

    glColor3f(1.0f, 1.0f, 1.0f);
    drawCircleMidpoint(static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(planet.radius), 1.0f);

    if (planet.name == "Earth") {
        Point2D moon = getOrbitPosition(pos.x, pos.y, 28.0f, moonAngle);
        glColor3f(0.82f, 0.82f, 0.86f);
        drawFilledCircle(static_cast<int>(moon.x), static_cast<int>(moon.y), 4);
    }

    if (planet.name == "Saturn") {
        glColor3f(0.92f, 0.86f, 0.62f);
        drawLineBresenham(static_cast<int>(pos.x - 30), static_cast<int>(pos.y - 5),
                          static_cast<int>(pos.x + 30), static_cast<int>(pos.y + 5), 1.5f);
        drawLineBresenham(static_cast<int>(pos.x - 30), static_cast<int>(pos.y + 5),
                          static_cast<int>(pos.x + 30), static_cast<int>(pos.y - 5), 1.5f);
    }

    glColor3f(0.92f, 0.95f, 1.0f);
    renderBitmapText(pos.x - planet.radius, pos.y + planet.radius + 12.0f,
                     GLUT_BITMAP_HELVETICA_10, planet.name);
}

std::vector<Point2D> buildShipModel() {
    return {
        {0.0f, 18.0f},
        {14.0f, 0.0f},
        {8.0f, -6.0f},
        {8.0f, -18.0f},
        {-8.0f, -18.0f},
        {-8.0f, -6.0f},
        {-14.0f, 0.0f}
    };
}

std::vector<Point2D> buildLeftPanelModel() {
    return {
        {-36.0f, 8.0f},
        {-16.0f, 8.0f},
        {-16.0f, -8.0f},
        {-36.0f, -8.0f}
    };
}

std::vector<Point2D> buildRightPanelModel() {
    return {
        {16.0f, 8.0f},
        {36.0f, 8.0f},
        {36.0f, -8.0f},
        {16.0f, -8.0f}
    };
}

std::vector<Point2D> applyTransform(const std::vector<Point2D>& model, const Matrix3& matrix) {
    std::vector<Point2D> transformed;
    transformed.reserve(model.size());
    for (const Point2D& point : model) {
        transformed.push_back(transformPoint(point, matrix));
    }
    return transformed;
}

void drawTransformableSatellite() {
    Matrix3 transform = identityMatrix();
    transform = multiplyMatrix(translationMatrix(shipOffsetX, shipOffsetY), transform);
    transform = multiplyMatrix(rotationMatrix(shipRotation), transform);
    transform = multiplyMatrix(scalingMatrix(shipScale, shipScale), transform);

    if (shearShipX) {
        transform = multiplyMatrix(shearMatrix(0.45f, 0.0f), transform);
    }
    if (shearShipY) {
        transform = multiplyMatrix(shearMatrix(0.0f, 0.35f), transform);
    }

    transform = multiplyMatrix(translationMatrix(930.0f, 150.0f), transform);

    std::vector<Point2D> body = applyTransform(buildShipModel(), transform);
    std::vector<Point2D> leftPanel = applyTransform(buildLeftPanelModel(), transform);
    std::vector<Point2D> rightPanel = applyTransform(buildRightPanelModel(), transform);

    glColor3f(0.85f, 0.90f, 0.98f);
    drawFilledPolygon(body);
    glColor3f(0.25f, 0.55f, 0.95f);
    drawFilledPolygon(leftPanel);
    drawFilledPolygon(rightPanel);

    glColor3f(1.0f, 1.0f, 1.0f);
    drawPolygonOutline(body, false);
    drawPolygonOutline(leftPanel, true);
    drawPolygonOutline(rightPanel, true);

    Point2D center = transformPoint({0.0f, 0.0f}, transform);
    glColor3f(1.0f, 0.65f, 0.18f);
    drawCircleMidpoint(static_cast<int>(center.x), static_cast<int>(center.y), 5, 1.5f);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    drawNebula();
    drawBackgroundStars();

    Point2D solarCenter = getSolarSystemCenter();
    float centerX = solarCenter.x;
    float centerY = solarCenter.y;

    drawOrbitRings(centerX, centerY);
    drawAsteroidBelt(centerX, centerY);
    drawSun(centerX, centerY);

    for (const Planet& planet : planets) {
        drawPlanet(planet, centerX, centerY);
    }

    drawTransformableSatellite();

    glutSwapBuffers();
}

void update(int value) {
    if (!paused) {
        for (Planet& planet : planets) {
            planet.angle += planet.orbitSpeed;
            if (planet.angle >= 360.0f) {
                planet.angle -= 360.0f;
            }
        }

        moonAngle += 4.0f;
        sunPulse += 0.06f;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
        case 'r':
        case 'R':
            shipRotation += 8.0f;
            break;
        case 't':
        case 'T':
            shipRotation -= 8.0f;
            break;
        case 'z':
        case 'Z':
            shipScale += 0.1f;
            break;
        case 'x':
        case 'X':
            shipScale = std::max(0.4f, shipScale - 0.1f);
            break;
        case 'h':
        case 'H':
            shearShipX = !shearShipX;
            break;
        case 'j':
        case 'J':
            shearShipY = !shearShipY;
            break;
        case ' ':
            paused = !paused;
            break;
        case 27:
            std::exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    const float step = 10.0f;
    switch (key) {
        case GLUT_KEY_LEFT:
            shipOffsetX -= step;
            break;
        case GLUT_KEY_RIGHT:
            shipOffsetX += step;
            break;
        case GLUT_KEY_UP:
            shipOffsetY += step;
            break;
        case GLUT_KEY_DOWN:
            shipOffsetY -= step;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void initScene() {
    glClearColor(0.02f, 0.03f, 0.08f, 1.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, WINDOW_WIDTH, 0.0, WINDOW_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initializeStars();
    initializePlanets();
}

void reshape(int width, int height) {
    currentWindowWidth = std::max(1, width);
    currentWindowHeight = std::max(1, height);
    updatePlanetLayout();

    glViewport(0, 0, currentWindowWidth, currentWindowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, currentWindowWidth, 0.0, currentWindowHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutInitWindowPosition(80, 40);
    glutCreateWindow("Astronomy 2D Simulator");

    initScene();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
