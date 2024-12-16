// Shree
// header files
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PI 3.1415


// global variable declarations
int bFullScreen = 0;

int resolution_x = 1920;
int resolution_y = 1080;

double coeffs_mtx[3][5];
double solar_zenith, solar_azimuth;


typedef struct {
    double r;
    double g;
    double b;
} RGB;

typedef struct {
    double Yz;
    double xz;
    double yz;
} ZenitalAbsolutes;
ZenitalAbsolutes zen_abs;



typedef struct {
    double A;
    double B;
    double C;
    double D;
    double E;
} Coefficients;

typedef struct {
    Coefficients coeffsY;
    Coefficients coeffsx;
    Coefficients coeffsy;
} CoefficientsResult;


// Entry-point function
int main(int argc, char* argv[])
{
    // local variable declarations
    int width = 1920;
    int height = 1080;
    // local function declarations
    int initialize(void);
    void resize(int, int);
    void display(void);
    void keyboard(unsigned char, int, int);
    void uninitialize(void);
    void renderSky();
    void timer(int);

    ZenitalAbsolutes CalculateZenitalAbsolutes(double turbidity, double solar_zenith);
    RGB Yxy_to_RGB(double Y, double x, double y);
    double Perez(double zenith, double gamma, double coeffs[5]);
    double Gamma(double zenith, double azimuth, double solar_zenith, double solar_azimuth);
    double gamma_correct(double v);
    void setupUserInputs();


    double scale_range(double k, double k_min, double k_max, double v_min, double v_max);

    double deg2rad(double deg);
    RGB Calc_Sky_RGB(double zenith, double azimuth, ZenitalAbsolutes zen_abs, double solar_zenith, double solar_azimuth, double coeffs_mtx[3][5]);

    RGB normalizeRGB(RGB color);
    void print_rgb(RGB color);

    // code
    setupUserInputs();

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sky is blue = 018-NikhilSathe");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);

    glClearColor(0.0, 0.0, 0.0, 1.0);


    glutTimerFunc(25, timer, 0);



    initialize();

    //glutReshapeFunc(resize);
    glutDisplayFunc(renderSky);
    glutKeyboardFunc(keyboard);
    glutCloseFunc(uninitialize);

    glutMainLoop();

    return(0);
}


int initialize(void)
{
    // code
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    return(0);
}

void resize(int width, int height)
{
    // code
    glViewport(0, 0, width, height);
}




ZenitalAbsolutes CalculateZenitalAbsolutes(double turbidity, double solar_zenith) {
    ZenitalAbsolutes ret;

    // Calculate Yz
    double Yz = (4.0453 * turbidity - 4.9710) * tan((4.0 / 9.0 - turbidity / 120.0) * (M_PI - 2.0 * solar_zenith)) - 0.2155 * turbidity + 2.4192;
    double Y0 = 40.0; // Constant value
    ret.Yz = Yz / Y0;

    // Precompute powers of solar_zenith
    double z3 = pow(solar_zenith, 1.3);
    double z2 = pow(solar_zenith, 1);
    double z = solar_zenith;

    // Compute xz
    double x[3] = {
        0.00166 * z3 - 0.00375 * z2 + 0.00209 * z + 0,
        -0.02903 * z3 + 0.06377 * z2 - 0.03202 * z + 0.00394,
        0.11693 * z3 - 0.21196 * z2 + 0.06052 * z + 0.25886
    };
    ret.xz = turbidity * turbidity * x[0] + turbidity * x[1] + x[2];

    // Compute yz
    double y[3] = {
        0.00275 * z3 - 0.00610 * z2 + 0.00317 * z + 0,
        -0.04214 * z3 + 0.08970 * z2 - 0.04153 * z + 0.00516,
        0.15346 * z3 - 0.26756 * z2 + 0.06670 * z + 0.26688
    };
    ret.yz = turbidity * turbidity * y[0] + turbidity * y[1] + y[2];

    return ret;
}


double Gamma(double zenith, double azimuth, double solar_zenith, double solar_azimuth)
{
    return acos(
        sin(solar_zenith)*sin(zenith)*cos(azimuth-solar_azimuth)+cos(solar_zenith)*cos(zenith));
}


double gamma_correct(double v) {
    double corrected = 0.0f;//pow(v, 1.0 / 2.4);
    if (v <= 0.0031308f) {
        corrected = 12.92f * v;
    } else {
        corrected = 1.055f * powf(v, 1.0f / 2.4f) - 0.055f;
    }

    //corrected = fmax(0.0, fmin(1.0, corrected));
    return corrected;
}

/*double gamma_correct(double v) {
    double corrected = pow(v, 1.0 / 1.8);
    if (corrected > 1.0) corrected = 1.0;
    if (corrected < 0.0) corrected = 0.0;
    return corrected;
}*/

RGB Yxy_to_RGB(double Y, double x, double y) {
    double X = (x / y) * Y;
    double Z = ((1.0 - x - y) / y) * Y;
    RGB color;

    //SRGB
    /*color.r = gamma_correct(3.2406 * X - 1.5372 * Y - 0.4986 * Z);
    color.g = gamma_correct(-0.9689 * X + 1.8758 * Y + 0.0415 * Z);
    color.b = gamma_correct(0.0557 * X - 0.2040 * Y + 1.0570 * Z);*/

    //ECI RGB
    /*color.r = gamma_correct(2.3706743 * X - 0.9000405 * Y - 0.4706338 * Z);
    color.g = gamma_correct(-0.5138850 * X + 1.4253036 * Y + 0.0885814 * Z);
    color.b = gamma_correct(0.0052982 * X - 0.0146949 * Y + 1.0093968 * Z);*/

    //Best RGB
    color.r = gamma_correct(1.7552599 * X - 0.4836786 * Y - 0.2530000 * Z);
    color.g = gamma_correct(-0.5441336 * X + 1.5068789 * Y + 0.0215528 * Z);
    color.b = gamma_correct(0.0063467 * X - 0.0175761 * Y + 1.2256959 * Z);

    /*color.r = gamma_correct(2.3638081 * X - 0.8676030 * Y - 0.4988161 * Z);
    color.g = gamma_correct(-0.5005940 * X + 1.3962369 * Y + 0.1047562 * Z);
    color.b = gamma_correct(0.0141712 * X - 0.0306400 * Y + 1.2323842 * Z);*/


    /*1.7552599 -0.4836786 -0.2530000
    -0.5441336  1.5068789  0.0215528
    0.0063467 -0.0175761  1.2256959*/
    /*color.r = floor(color.r*255);
    color.b = floor(color.g*255);
    color.g = floor(color.b*255);*/
    // Clamp values to prevent overflow
    /*color.r = fmax(0.0, fmin(1.0, color.r));
    color.g = fmax(0.0, fmin(1.0, color.g));
    color.b = fmax(0.0, fmin(1.0, color.b));*/

    return color;
}

RGB normalizeRGB(RGB color) {
    color.r = fmax(0.0, fmin(1.0, color.r)); // Clamp r to [0, 1]
    color.g = fmax(0.0, fmin(1.0, color.g)); // Clamp g to [0, 1]
    color.b = fmax(0.0, fmin(1.0, color.b)); // Clamp b to [0, 1]
    return color;
}




CoefficientsResult CalculateCoefficents(double turbidity) {
    CoefficientsResult ret;

    // Calculate coeffsY
    ret.coeffsY.A = 0.1787 * turbidity - 1.4630;
    ret.coeffsY.B = -0.3554 * turbidity + 0.4275;
    ret.coeffsY.C = -0.0227 * turbidity + 5.3251;
    ret.coeffsY.D = 0.1206 * turbidity - 2.5771;
    ret.coeffsY.E = -0.0670 * turbidity + 0.3703;

    // Calculate coeffsx
    ret.coeffsx.A = -0.0193 * turbidity - 0.2592;
    ret.coeffsx.B = -0.0665 * turbidity + 0.0008;
    ret.coeffsx.C = -0.0004 * turbidity + 0.2125;
    ret.coeffsx.D = -0.0641 * turbidity - 0.8989;
    ret.coeffsx.E = -0.0033 * turbidity + 0.0452;

    // Calculate coeffsy
    ret.coeffsy.A = -0.0167 * turbidity - 0.2608;
    ret.coeffsy.B = -0.0950 * turbidity + 0.0092;
    ret.coeffsy.C = -0.0079 * turbidity + 0.2102;
    ret.coeffsy.D = -0.0441 * turbidity - 1.6537;
    ret.coeffsy.E = -0.0109 * turbidity + 0.0529;

    return ret;
}

double Perez(double zenith, double gamma, double coeffs[5]) {
    return (1.0 + coeffs[0] * exp(coeffs[1] / cos(zenith))) *
    (1.0 + coeffs[2] * exp(coeffs[3] * gamma) + coeffs[4] * cos(gamma) * cos(gamma));
}


double scale_range(double k, double k_min, double k_max, double v_min, double v_max) {
    return ((k - k_min) / (k_max - k_min)) * (v_max - v_min) + v_min;
}

double deg2rad(double deg) {
    return deg * (M_PI / 180.0);
}

RGB Calc_Sky_RGB(double zenith, double azimuth, ZenitalAbsolutes zen_abs, double solar_zenith, double solar_azimuth, double coeffs_mtx[3][5]) {
    double gamma = Gamma(zenith, azimuth, solar_zenith, solar_azimuth);
    zenith = fmin(zenith, M_PI / 2.0);

    double Yp = zen_abs.Yz * Perez(zenith, gamma, coeffs_mtx[0]) / Perez(0.0, solar_zenith, coeffs_mtx[0]);
    double xp = zen_abs.xz * Perez(zenith, gamma, coeffs_mtx[1]) / Perez(0.0, solar_zenith, coeffs_mtx[1]);
    double yp = zen_abs.yz * Perez(zenith, gamma, coeffs_mtx[2]) / Perez(0.0, solar_zenith, coeffs_mtx[2]);

    return Yxy_to_RGB(Yp, xp, yp);
    /*RGB color;
    color.r = Yp;
    color.g = xp;
    color.b = yp;

    return color;*/
}


void print_rgb(RGB color) {
    printf("RGB: (%.3f, %.3f, %.3f)\n", color.r, color.g, color.b);
}

void setupUserInputs() {
    printf("Enter turbidity: ");
    double turbidity;
    scanf("%lf", &turbidity);

    printf("Enter longitude (degrees): ");
    double longitude;
    scanf("%lf", &longitude);

    printf("Enter latitude (degrees): ");
    double latitude;
    scanf("%lf", &latitude);

    longitude = deg2rad(longitude);
    latitude = deg2rad(latitude);

    printf("Enter solar zenith (degrees): ");
    scanf("%lf", &solar_zenith);
    solar_zenith = deg2rad(solar_zenith);

    printf("Enter solar azimuth (degrees): ");
    scanf("%lf", &solar_azimuth);
    solar_azimuth = deg2rad(solar_azimuth);

    // Initialize zen_abs and coeffs_mtx with dummy values for now
    CoefficientsResult coeffs = CalculateCoefficents(turbidity);
    zen_abs = CalculateZenitalAbsolutes(turbidity, solar_zenith);
    //zen_abs.Yz = 1.0;
    //zen_abs.xz = 0.3;
    //zen_abs.yz = 0.3;

    coeffs_mtx[0][0] = coeffs.coeffsY.A;
    coeffs_mtx[0][1] = coeffs.coeffsY.B;
    coeffs_mtx[0][2] = coeffs.coeffsY.C;
    coeffs_mtx[0][3] = coeffs.coeffsY.D;
    coeffs_mtx[0][4] = coeffs.coeffsY.E;

    coeffs_mtx[1][0] = coeffs.coeffsx.A;
    coeffs_mtx[1][1] = coeffs.coeffsx.B;
    coeffs_mtx[1][2] = coeffs.coeffsx.C;
    coeffs_mtx[1][3] = coeffs.coeffsx.D;
    coeffs_mtx[1][4] = coeffs.coeffsx.E;

    coeffs_mtx[2][0] = coeffs.coeffsy.A;
    coeffs_mtx[2][1] = coeffs.coeffsy.B;
    coeffs_mtx[2][2] = coeffs.coeffsy.C;
    coeffs_mtx[2][3] = coeffs.coeffsy.D;
    coeffs_mtx[2][4] = coeffs.coeffsy.E;

}




void renderSky() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_FRAMEBUFFER_SRGB);

    for (int row = 0; row < resolution_y; ++row) {
        for (int col = 0; col < resolution_x; ++col) {
            double azimuth = deg2rad(scale_range(col, 0, resolution_x, -180, 180));
            double zenith = deg2rad(scale_range(row, 0, resolution_y, 0, 90));

            RGB rgb = Calc_Sky_RGB(zenith, azimuth, zen_abs, solar_zenith, solar_azimuth, coeffs_mtx);
            //rgb = normalizeRGB(rgb);

            glColor3f(rgb.r, rgb.g, rgb.b);
            //printf("%f,%f,%f", rgb.r, rgb.g, rgb.b);

            double x = (double)col / resolution_x * 2.0 - 1.0;
            double y = (double)row / resolution_y * 2.0 - 1.0;

            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + 2.0 / resolution_x, y);
            glVertex2f(x + 2.0 / resolution_x, y + 2.0 / resolution_y);
            glVertex2f(x, y + 2.0 / resolution_y);
            glEnd();
        }
    }

    glutSwapBuffers();
}

void timer(int value) {
    solar_zenith += 0.05;
    if (solar_zenith > M_PI / 2) solar_zenith = 0;

    glutPostRedisplay();
    glutTimerFunc(25, timer, 0);
}



void keyboard(unsigned char key, int x, int y)
{
    // code
    switch (key)
    {
        case 27:
            glutLeaveMainLoop();
            break;
        case 'F':
        case 'f':
            if (bFullScreen == 0)
            {
                glutFullScreen();
                bFullScreen = 1;
            }
            else
            {
                glutLeaveFullScreen();
                bFullScreen = 0;
            }
            break;
        default:
            break;
    }
}


void uninitialize(void)
{
    // code
}
