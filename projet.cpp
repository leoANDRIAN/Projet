#include <iostream>
#include <cmath>
#include <gl/glut.h>
#include "geometry.h"
#include "forms.h"
#include "arrow.h"
#include "Target.h"
#include "wind.h"
#include "wall.h"
#include "speedIndicator.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

/***************************************************************************/
/* Constants and functions declarations                                    */

// Screen dimension constants
int const SCREEN_WIDTH = 1000;
int const SCREEN_HEIGHT = 600;

// Max number of forms : static allocation
const int MAX_FORMS_NUMBER = 50;

// Animation actualization delay (in ms) => 100 updates per second
const Uint32 ANIM_DELAY = 10;

GLuint TexIDSkyBox[7];


// Starts up SDL, creates window, and initializes OpenGL
bool init(SDL_Window** window, SDL_GLContext* context);

// Initializes matrices and clear color
bool initGL();

// Updating forms for animation
void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t);

// Renders scene to the screen
void render(Form* formlist[MAX_FORMS_NUMBER], const Point& cam_pos, int angleVue);

// Frees media and shuts down SDL
void close(SDL_Window** window);


/***************************************************************************/
/* Functions implementations                                               */
/***************************************************************************/
bool init(SDL_Window** window, SDL_GLContext* context)
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else
    {
        // Use OpenGL 2.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        // Create window
        *window = SDL_CreateWindow("TP intro OpenGL / SDL 2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
        if (*window == NULL)
        {
            std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
            success = false;
        }
        else
        {
            // Create context
            *context = SDL_GL_CreateContext(*window);
            if (*context == NULL)
            {
                std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else
            {
                // Use Vsync
                if (SDL_GL_SetSwapInterval(1) < 0)
                {
                    std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
                }

                // Initialize OpenGL
                if (!initGL())
                {
                    std::cout << "Unable to initialize OpenGL!" << std::endl;
                    success = false;
                }
            }
        }
    }

    return success;
}


bool initGL()
{
    bool success = true;
    GLenum error = GL_NO_ERROR;

    // Initialize Projection Matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the viewport : use all the window to display the rendered scene
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Fix aspect ratio and depth clipping planes
    gluPerspective(40.0, (GLdouble)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0, 100.0);


    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Initialize clear color : black with no transparency
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Activate Z-Buffer
    glEnable(GL_DEPTH_TEST);
    //*glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    // Lighting basic configuration and activation
    const GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    const GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

    const GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    const GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    const GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    const GLfloat high_shininess[] = { 100.0f };

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);


    // Check for error
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cout << "Error initializing OpenGL!  " << gluErrorString(error) << std::endl;
        success = false;
    }

    return success;
}

void update(Form* formlist[MAX_FORMS_NUMBER], double delta_t)
{
    // Update the list of forms
    unsigned short i = 0;
    while (formlist[i] != NULL)
    {
        formlist[i]->update(delta_t);
        i++;
    }
    i = 0;
}

void render(Form* formlist[MAX_FORMS_NUMBER], Vector vec, int score)
{
    // Clear color buffer and Z-Buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Initialize Modelview Matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Set the camera position and parameters
    gluLookAt(0, 0, 0, vec.x, vec.y, vec.z, 0.0, 1.0, 0.0);
    // Isometric view
    // glRotated(angleVue, 0, 1, 0);
    // glRotated(30, 1, 0, -1);

    // X, Y and Z axis
    glPushMatrix(); // Preserve the camera viewing point for further forms
    // Render the coordinates system
    glBegin(GL_LINES);
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(1, 0, 0);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 1, 0);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3i(0, 0, 0);
        glVertex3i(0, 0, 1);
    }
    glEnd();
    glPopMatrix(); // Restore the camera viewing point for next object

    // Render the list of forms
    unsigned short i = 0;
    while (formlist[i] != NULL)
    {
        glPushMatrix(); // Preserve the camera viewing point for further forms
        formlist[i]->render();
        glPopMatrix(); // Restore the camera viewing point for next object
        i++;
    }
    ////// Vitesse //////
    glPushMatrix();
    auto vent = static_cast<Wind*>(formlist[1]); // J'assure que c'est du vent dans cette case du tableau
    if (vent->direction) {
        glTranslatef(vent->getAnim().getPos().x - 0.5, vent->getAnim().getPos().y, vent->getAnim().getPos().z);
    }
    else {
        glTranslatef(vent->getAnim().getPos().x + 0.5, vent->getAnim().getPos().y, vent->getAnim().getPos().z);
    }
    string stri = to_string(vent->speed);
    stri += " km/h";
    glColor3f(1.0, 1.0, 1.0);
    // set position to text    
    glRasterPos2f(0.0, 1.0);
    for (int i = 0; i < stri.length(); i++)
    {  
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stri[i]);
    }
    glPopMatrix();
    ////// Score //////
    glPushMatrix();
    auto cib = static_cast<Target*>(formlist[0]); // J'assure que c'est la cible dans cette case du tableau
    glTranslatef(-1, 3.2, -10);
    stri = "Score : ";
    stri += to_string(cib->points);
    glColor3f(1.0, 1.0, 1.0);
    // set position to text    
    glRasterPos2f(0.0, 1.0);
    for (int i = 0; i < stri.length(); i++)
    { 
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, stri[i]);
    }
    glPopMatrix();
    i = 0;
}

void close(SDL_Window** window)
{
    //Destroy window
    SDL_DestroyWindow(*window);
    *window = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

void updateRegard(float& angleVert, float& angleHor, Vector& vec, float mouseSpeed, int x, int y, SDL_Window* win) {
    angleHor += mouseSpeed * float(SCREEN_WIDTH/2 - x);
    angleVert += mouseSpeed * float(SCREEN_HEIGHT/2 - y);
   if (angleVert < -1) {
        angleVert = -1;
    }
    else if (angleVert > 1) {
        angleVert = 1;
    }
    vec.x = cos(angleVert) * sin(angleHor);
    vec.y = sin(angleVert);
    vec.z = cos(angleVert) * cos(angleHor);
    SDL_WarpMouseInWindow(win, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void creatTexture(void)
{
    const char* img[7];
    img[0] = "skybox/back.jpg";
    img[1] = "skybox/front.jpg";
    img[2] = "skybox/bottom.jpg";
    img[3] = "skybox/top.jpg";
    img[4] = "skybox/left.jpg";
    img[5] = "skybox/right.jpg";
    img[6] = "sol.jpg";

    int i = 1;

    for (i = 0; i <= 6; ++i)
    {
        SDL_Surface* imgsurf = IMG_Load(img[i]);
        if (imgsurf == NULL) {
            printf("Failed to load texture image!\n");
        }
        int mode;
        if (imgsurf->format->BytesPerPixel == 3) { // RGB 24bit
            mode = GL_RGB;
        }
        else if (imgsurf->format->BytesPerPixel == 4) { // RGBA 32bit
            mode = GL_RGBA;
        }
        else {
            SDL_FreeSurface(imgsurf);
            return;
        }
        glGenTextures(1, &TexIDSkyBox[i]);
        glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, mode, imgsurf->w, imgsurf->h, 0, mode, GL_UNSIGNED_BYTE, imgsurf->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
        SDL_FreeSurface(imgsurf);
    }
}

void SkyBox_Draw(float x, float y, float z, float width, float height, float length)
{
    x = x - width / 2;  // Calcul l'emplacement d'un coin du cube
    y = y - height / 2;
    z = z - length / 2;
    glPushMatrix();
    //glDisable(GL_DEPTH_TEST);

    //GLint OldCullFaceMode;
    //glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
    //GLint OldDepthFuncMode;
    //glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

    //glCullFace(GL_FRONT);
    //glDepthFunc(GL_LEQUAL);
    glEnable(GL_TEXTURE_2D);

    glDepthMask(GL_FALSE);

    glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[0]);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[1]);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[2]);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z + length);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[3]);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1, 0);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y + height, z + length);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
    glEnd();


    glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[5]);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1, 0);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, TexIDSkyBox[4]);
    glBegin(GL_TRIANGLE_STRIP);
    glColor4f(1, 1, 1, 0);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
    glEnd();

    glDepthMask(GL_TRUE);

    //glCullFace(OldCullFaceMode);
    //glDepthFunc(OldDepthFuncMode);
    //glEnable(GL_DEPTH_TEST);
    glPopMatrix();

    glDisable(GL_TEXTURE_2D);
}

/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    glutInit(&argc, args);
    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;

    // initialize random seed
    srand(time(NULL));

    int windSpeed = rand() % 10 + 1;
    bool windDirection = true;
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float mouseSpeed = 0.005f;
    Vector regard = Vector(0, 0, -1);
    Vector speed1 = Vector(5, 2, 0);
    Vector acc1 = Vector(windSpeed, -10, 0);

    // Start up SDL and create window
    if (!init(&gWindow, &gContext))
    {
        std::cout << "Failed to initialize!" << std::endl;
    }
    else
    {
        // Main loop flag
        bool quit = false;
        Uint32 current_time, previous_time, elapsed_time;
        // Event handler
        SDL_Event event;
        bool mouseClic = true;

        // Camera position
        // Point camera_position(0, 0.0, 5.0);

        // Textures
        creatTexture();
        
        // The forms to render
        Form* forms_list[MAX_FORMS_NUMBER];
        unsigned short number_of_forms = 0, i;
        for (i = 0; i < MAX_FORMS_NUMBER; i++)
        {
            forms_list[i] = NULL;
        }
        Target* t = NULL;
        t = new Target(1);
        forms_list[number_of_forms] = t;
        number_of_forms++;
        t->getAnim().setPos(Point(0.0, 1.0, -20.0));

        Wind* w = NULL;
        w = new Wind(windDirection, abs(windSpeed));
        forms_list[number_of_forms] = w;
        number_of_forms++;

        // Si besoin de s'orienter pour debug :

        /*Sphere* s1 = NULL;
        s1 = new Sphere(0.1, Point(1, 0, 0), WHITE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1->setTexture(TexIDSkyBox[6]);
        s1 = new Sphere(0.1, Point(1, 1, 0), WHITE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1->setTexture(TexIDSkyBox[6]);
        s1 = new Sphere(0.1, Point(0, 1, 0), WHITE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1->setTexture(TexIDSkyBox[6]);
        s1 = new Sphere(0.1, Point(0, -1, 0), WHITE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1->setTexture(TexIDSkyBox[6]);
        s1 = new Sphere(0.1, Point(0, 0, 2), WHITE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1->setTexture(TexIDSkyBox[6]);*/
        /*s1 = new Sphere(0.1, Point(0, 0, -2), ORANGE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;*/

        Wall* bottom = NULL;
        bottom = new Wall(Vector(1, 0, 0), Vector(0, 0, 1), Point(-50, -2, -50), 100, 100, WHITE);
        bottom->setTexture(TexIDSkyBox[6]);
        forms_list[number_of_forms] = bottom;
        number_of_forms++;

        Arrow* a = NULL;

        SDL_ShowCursor(SDL_DISABLE);
        // Get first "current time"
        previous_time = SDL_GetTicks();
        // While application is running
        while (!quit)
        {
            // Handle events on queue
            while (SDL_PollEvent(&event) != 0)
            {
                int x = 0, y = 0;
                SDL_Keycode key_pressed = event.key.keysym.sym;

                switch (event.type)
                {
                    // User requests quit
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    mouseClic = true;
                    a = new Arrow(100, 2, regard, t);
                    a->getAnim().setSpeed(30 * regard);
                    a->getAnim().setAccel(acc1);
                    forms_list[number_of_forms] = a;
                    number_of_forms++;
                    break;
                case SDL_KEYDOWN:
                    switch (key_pressed)
                    {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_q:
                        break;
                    case SDLK_d:
                        break;
                    case SDLK_z:
                        break;
                    case SDLK_s:
                        break;
                    default:
                        break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    
                    break;
                default:
                    break;
                }
                SDL_GetMouseState(&x, &y);
                updateRegard(verticalAngle, horizontalAngle, regard, mouseSpeed, x, y, gWindow);
            }
            // Update the scene
            current_time = SDL_GetTicks(); // get the elapsed time from SDL initialization (ms)
            elapsed_time = current_time - previous_time;
            if (elapsed_time > ANIM_DELAY)
            {
                previous_time = current_time;
                update(forms_list, 1e-3 * elapsed_time); // International system units : seconds
            }

            // Render the scene
            render(forms_list, regard, 4);
            SkyBox_Draw(-0, -0, -0, 100, 100, 100);
            if (mouseClic) {
                
                windSpeed = rand() % 10 + 1;
                windDirection = true;
                if (rand() % 2 == 0) {
                    windDirection = false;
                    windSpeed = -windSpeed;
                }
                w->direction = windDirection;
                w->speed = abs(windSpeed);
                acc1.x = windSpeed;
                mouseClic = false;
            }
            SDL_GL_SwapWindow(gWindow);
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
