#include <iostream>
#include <cmath>
#include <gl/glut.h>
#include "geometry.h"
#include "forms.h"
#include "arrow.h"
#include "Target.h"
#include "wind.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_opengl.h>
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

void render(Form* formlist[MAX_FORMS_NUMBER], Vector vec)
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
    cout << angleHor << endl;
    cout << angleVert << endl;
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
    // SetCursorPos(600, 300);
}

/***************************************************************************/
/* MAIN Function                                                           */
/***************************************************************************/
int main(int argc, char* args[])
{
    // The window we'll be rendering to
    SDL_Window* gWindow = NULL;

    // OpenGL context
    SDL_GLContext gContext;
<<<<<<< HEAD

    // initialize random seed
    srand(time(NULL));

    // generate number between 1 and 10
    int windSpeed = rand() % 10 + 1;

    bool windDirection = true;
    if (rand() % 2 == 0) {
        windDirection = false;
        windSpeed = -windSpeed;
    }

    // int angleVue = -45;
    // int hVue = 1;
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

        // Camera position
        // Point camera_position(0, 0.0, 5.0);

<<<<<<< HEAD
        // Texture //////////////////////////////////////////////////////////
        GLuint textureid_1;
        // Le fichier devant servir de texture. A mettre dans le dossier du projet
        SDL_Surface* imgsurf = IMG_Load("earth_texture.jpg");
        if (imgsurf == NULL) {
            printf("Failed to load texture image!\n");
        }
        // work out what format to tell glTexImage2D to use...
        int mode;
        if (imgsurf->format->BytesPerPixel == 3) { // RGB 24bit
            mode = GL_RGB;
        }
        else if (imgsurf->format->BytesPerPixel == 4) { // RGBA 32bit
            mode = GL_RGBA;
        }
        else {
            SDL_FreeSurface(imgsurf);
            return 0;
        }
        // create one texture name
        glGenTextures(1, &textureid_1);

        // tell opengl to use the generated texture name
        glBindTexture(GL_TEXTURE_2D, textureid_1);

        // this reads from the sdl imgsurf and puts it into an opengl texture
        glTexImage2D(GL_TEXTURE_2D, 0, mode, imgsurf->w, imgsurf->h, 0, mode, GL_UNSIGNED_BYTE, imgsurf->pixels);

        // these affect how this texture is drawn later on...
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // clean up
        SDL_FreeSurface(imgsurf);

        // Texture //////////////////////////////////////////////////////////
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

        // Si besoin de s'orienter pour debug :

        Sphere* s1 = NULL;
        s1 = new Sphere(0.1, Point(2, 0, 0), RED);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1 = new Sphere(0.1, Point(2, 1, 0), GREEN);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1 = new Sphere(0.1, Point(0, 1, 0), BLUE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1 = new Sphere(0.1, Point(0, -1, 0), WHITE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1 = new Sphere(0.1, Point(0, 0, 2), YELLOW);
        forms_list[number_of_forms] = s1;
        number_of_forms++;
        s1 = new Sphere(0.1, Point(0, 0, -2), ORANGE);
        forms_list[number_of_forms] = s1;
        number_of_forms++;


        Target* t = NULL;
        t = new Target(1);
        forms_list[number_of_forms] = t;
        number_of_forms++;
        t->getAnim().setPos(Point(0.0, 1.0, -25.0));

        Arrow* a = NULL;

        Wind* w = NULL;
        w = new Wind(windDirection, abs(windSpeed));
        forms_list[number_of_forms] = w;
        number_of_forms++;

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
                case SDL_KEYDOWN:
                    switch (key_pressed)
                    {
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_q:
                        a = new Arrow(100, 2, regard, t);
                        a->getAnim().setSpeed(30 * regard);
                        a->getAnim().setAccel(acc1);
                        forms_list[number_of_forms] = a;
                        number_of_forms++;
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
                    SDL_GetMouseState(&x, &y);
                    updateRegard(verticalAngle, horizontalAngle, regard, mouseSpeed, x, y, gWindow);
                    break;
                default:
                    break;
                }
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
            render(forms_list, regard);

            // Update window screen
            SDL_GL_SwapWindow(gWindow);
        }
    }

    // Free resources and close SDL
    close(&gWindow);

    return 0;
}
