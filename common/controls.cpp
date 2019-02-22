
#include "controls.h"

#include <glm/glm.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include <SDL2/SDL.h>


#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768


using namespace std;

const float _PI = (float) 3.141592;

unsigned char cameracontrols[3];

float mouseHorDivergence = 0.0f;
float mouseVerDivergence = 0.0f;

glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;
glm::vec3 CameraPosition;
glm::vec3 SphereCenter;
glm::vec3 UpVector;

// some moving parameters
float rho;   // distance to spherical origin
float theta; // horizontal spherical coordinate
float phi;   // vertical spherical coordinate
float hor_speed;
float ver_speed;

int prevwheel_x = 0;
int prevwheel_y = 0;

void InitCamera()
{
    rho = 3.0f;
    // initial spherical coordinates
    phi = _PI/2.0;
    theta = 0.0f;

    // Look to the origin
    SphereCenter = glm::vec3(0, 0, 0);
}

void UpdateCamera(SDL_Window* window)
{
    // Get window size
    int window_w;
    int window_h;
    SDL_GetWindowSize(window, &window_w, &window_h);

    if(IsMousePressed(MS_LEFT))
    {
        int wheel_x, wheel_y;
        SDL_GetMouseState(&wheel_x, &wheel_y);
        if(wheel_x != prevwheel_x || wheel_y != prevwheel_y)
        {
            mouseHorDivergence = (float)(prevwheel_x-wheel_x);
            mouseVerDivergence = (float)(prevwheel_y-wheel_y);
        }
        prevwheel_x = wheel_x;
        prevwheel_y = wheel_y;
    }
    else
    {
        SDL_GetMouseState(&prevwheel_x, &prevwheel_y);
    }

    // calc speed depending on different mouse divergences
    float abshor = std::abs(mouseHorDivergence);
    float absver = std::abs(mouseVerDivergence);
    hor_speed = 0.001 + 0.0001f * abshor;
    ver_speed = 0.001f + 0.0001f * absver;

    float horizontal_step = mouseHorDivergence * hor_speed;
    float vertical_step   = mouseVerDivergence * ver_speed;

    theta += horizontal_step;
    phi   += vertical_step;

    // avoid upside-down
    if(phi > _PI ) phi = _PI;
    if(phi < 0.0f) phi = 0.0001f;

    // note: in OpenGL, there is a right handed coordinate system
    CameraPosition.x = SphereCenter.x + rho * sin(phi) * sin(theta);
    CameraPosition.y = SphereCenter.y + rho * cos(phi);
    CameraPosition.z = SphereCenter.z + rho * sin(phi) * cos(theta);

    // update projection matrix
    ProjectionMatrix = glm::perspective(80.0f, float(WINDOW_WIDTH / WINDOW_HEIGHT), 0.1f, 100.0f);

    // Camera matrix
    ViewMatrix = glm::lookAt(
                     CameraPosition,        // Camera is here
                     SphereCenter,          // and looks here : at the same position, plus "direction"
                     glm::vec3(0, 1, 0)     // Head is up (set to 0,-1,0 to look upside-down)
                 );

    mouseHorDivergence = 0.0;
    mouseVerDivergence = 0.0;
}

glm::mat4 GetViewMatrix()
{
    return ViewMatrix;
}

glm::mat4 GetProjectionMatrix()
{
    return ProjectionMatrix;
}

float movespeed = 0.1;

void MoveForward()
{
    // vector CameraPosition->SphereCenter
    SphereCenter = SphereCenter + ( glm::normalize(SphereCenter - CameraPosition) * movespeed );
}

void MoveBackward()
{
    SphereCenter = SphereCenter - ( glm::normalize(SphereCenter - CameraPosition) * movespeed );
}

// input handler

SDL_Event current_event;
bool keystates[KS_LAST_CONSTANT];
bool mousestates[MS_LAST_CONSTANT];
bool keyevent_pressed[KP_LAST_CONSTANT];
bool keyevent_released[KR_LAST_CONSTANT];
bool mouseevent_pressed[MP_LAST_CONSTANT];
bool mouseevent_released[MR_LAST_CONSTANT];
bool windowevent[WE_LAST_CONSTANT];


void InitEventListener()
{
    for(unsigned int i=0; i<KS_LAST_CONSTANT; i++)
        keystates[i] = false;
    for(unsigned int i=0; i<MS_LAST_CONSTANT; i++)
        mousestates[i] = false;
    for(unsigned int i=0; i<WE_LAST_CONSTANT; i++)
        windowevent[i] = false;
}

void ReactToEvents()
{
    if(IsKeyPressed(KS_W)) MoveForward();
    if(IsKeyPressed(KS_S)) MoveBackward();
}

bool IsKeyPressed(KEYSTATES kcode)
{
    return keystates[kcode];
}

bool IsKeyPressTriggered(KEYEVENT_PRESSED ptkey)
{
    return keyevent_pressed[ptkey];
}

bool IsKeyReleaseTriggered(KEYEVENT_RELEASED rtkey)
{
    return keyevent_released[rtkey];
}

bool IsMousePressed(MOUSESTATES mcode)
{
    return mousestates[mcode];
}

bool IsMousePressTriggered(MOUSEEVENT_PRESSED ptmouse)
{
    return mouseevent_pressed[ptmouse];
}

bool IsMouseReleaseTriggered(MOUSEEVENT_RELEASED rtmouse)
{
    return mouseevent_released[rtmouse];
}

bool IsWindowEventTriggered(WINDOWEVENT wecode)
{
    return windowevent[wecode];
}

void AssignKeyboardAndMouseEvents()
{
    // only short events like pressed or released
    ResetTriggeredEvents();

    while(SDL_PollEvent(&current_event))
    {
        switch(current_event.type)
        {
        case SDL_KEYDOWN:
            switch(current_event.key.keysym.sym)
            {
            case SDLK_0:
                keystates[KS_0] = true;
                keyevent_pressed[KP_0] = true;
                break;
            case SDLK_1:
                keystates[KS_1] = true;
                keyevent_pressed[KP_1] = true;
                break;
            case SDLK_2:
                keystates[KS_2] = true;
                keyevent_pressed[KP_2] = true;
                break;
            case SDLK_3:
                keystates[KS_3] = true;
                keyevent_pressed[KP_3] = true;
                break;
            case SDLK_4:
                keystates[KS_4] = true;
                keyevent_pressed[KP_4] = true;
                break;
            case SDLK_5:
                keystates[KS_5] = true;
                keyevent_pressed[KP_5] = true;
                break;
            case SDLK_6:
                keystates[KS_6] = true;
                keyevent_pressed[KP_6] = true;
                break;
            case SDLK_7:
                keystates[KS_7] = true;
                keyevent_pressed[KP_7] = true;
                break;
            case SDLK_8:
                keystates[KS_8] = true;
                keyevent_pressed[KP_8] = true;
                break;
            case SDLK_9:
                keystates[KS_9] = true;
                keyevent_pressed[KP_9] = true;
                break;

            case SDLK_a:
                keystates[KS_A] = true;
                keyevent_pressed[KP_A] = true;
                break;
            case SDLK_b:
                keystates[KS_B] = true;
                keyevent_pressed[KP_B] = true;
                break;
            case SDLK_c:
                keystates[KS_C] = true;
                keyevent_pressed[KP_C] = true;
                break;
            case SDLK_d:
                keystates[KS_D] = true;
                keyevent_pressed[KP_D] = true;
                break;
            case SDLK_e:
                keystates[KS_E] = true;
                keyevent_pressed[KP_E] = true;
                break;
            case SDLK_f:
                keystates[KS_F] = true;
                keyevent_pressed[KP_F] = true;
                break;
            case SDLK_g:
                keystates[KS_G] = true;
                keyevent_pressed[KP_G] = true;
                break;
            case SDLK_h:
                keystates[KS_H] = true;
                keyevent_pressed[KP_H] = true;
                break;
            case SDLK_i:
                keystates[KS_I] = true;
                keyevent_pressed[KP_I] = true;
                break;
            case SDLK_j:
                keystates[KS_J] = true;
                keyevent_pressed[KP_J] = true;
                break;
            case SDLK_k:
                keystates[KS_K] = true;
                keyevent_pressed[KP_K] = true;
                break;
            case SDLK_l:
                keystates[KS_L] = true;
                keyevent_pressed[KP_L] = true;
                break;
            case SDLK_m:
                keystates[KS_M] = true;
                keyevent_pressed[KP_M] = true;
                break;
            case SDLK_n:
                keystates[KS_N] = true;
                keyevent_pressed[KP_N] = true;
                break;
            case SDLK_o:
                keystates[KS_O] = true;
                keyevent_pressed[KP_O] = true;
                break;
            case SDLK_p:
                keystates[KS_P] = true;
                keyevent_pressed[KP_P] = true;
                break;
            case SDLK_q:
                keystates[KS_Q] = true;
                keyevent_pressed[KP_Q] = true;
                break;
            case SDLK_r:
                keystates[KS_R] = true;
                keyevent_pressed[KP_R] = true;
                break;
            case SDLK_s:
                keystates[KS_S] = true;
                keyevent_pressed[KP_S] = true;
                break;
            case SDLK_t:
                keystates[KS_T] = true;
                keyevent_pressed[KP_T] = true;
                break;
            case SDLK_u:
                keystates[KS_U] = true;
                keyevent_pressed[KP_U] = true;
                break;
            case SDLK_v:
                keystates[KS_V] = true;
                keyevent_pressed[KP_V] = true;
                break;
            case SDLK_w:
                keystates[KS_W] = true;
                keyevent_pressed[KP_W] = true;
                break;
            case SDLK_x:
                keystates[KS_X] = true;
                keyevent_pressed[KP_X] = true;
                break;
            case SDLK_y:
                keystates[KS_Y] = true;
                keyevent_pressed[KP_Y] = true;
                break;
            case SDLK_z:
                keystates[KS_Z] = true;
                keyevent_pressed[KP_Z] = true;
                break;

            // Funktionstasten
            case SDLK_RETURN:
                keystates[KS_RETURN] = true;
                keyevent_pressed[KP_RETURN] = true;
                break;
            case SDLK_UP:
                keystates[KS_UP] = true;
                keyevent_pressed[KP_UP] = true;
                break;
            case SDLK_DOWN:
                keystates[KS_DOWN] = true;
                keyevent_pressed[KP_DOWN] = true;
                break;
            }
            break;
        case SDL_KEYUP:
            switch(current_event.key.keysym.sym)
            {
            case SDLK_0:
                keystates[KS_0] = false;
                keyevent_released[KP_0] = true;
                break;
            case SDLK_1:
                keystates[KS_1] = false;
                keyevent_released[KP_1] = true;
                break;
            case SDLK_2:
                keystates[KS_2] = false;
                keyevent_released[KP_2] = true;
                break;
            case SDLK_3:
                keystates[KS_3] = false;
                keyevent_released[KP_3] = true;
                break;
            case SDLK_4:
                keystates[KS_4] = false;
                keyevent_released[KP_4] = true;
                break;
            case SDLK_5:
                keystates[KS_5] = false;
                keyevent_released[KP_5] = true;
                break;
            case SDLK_6:
                keystates[KS_6] = false;
                keyevent_released[KP_6] = true;
                break;
            case SDLK_7:
                keystates[KS_7] = false;
                keyevent_released[KP_7] = true;
                break;
            case SDLK_8:
                keystates[KS_8] = false;
                keyevent_released[KP_8] = true;
                break;
            case SDLK_9:
                keystates[KS_9] = false;
                keyevent_released[KP_9] = true;
                break;

            case SDLK_a:
                keystates[KS_A] = false;
                keyevent_released[KP_A] = true;
                break;
            case SDLK_b:
                keystates[KS_B] = false;
                keyevent_released[KP_B] = true;
                break;
            case SDLK_c:
                keystates[KS_C] = false;
                keyevent_released[KP_C] = true;
                break;
            case SDLK_d:
                keystates[KS_D] = false;
                keyevent_released[KP_D] = true;
                break;
            case SDLK_e:
                keystates[KS_E] = false;
                keyevent_released[KP_E] = true;
                break;
            case SDLK_f:
                keystates[KS_F] = false;
                keyevent_released[KP_F] = true;
                break;
            case SDLK_g:
                keystates[KS_G] = false;
                keyevent_released[KP_G] = true;
                break;
            case SDLK_h:
                keystates[KS_H] = false;
                keyevent_released[KP_H] = true;
                break;
            case SDLK_i:
                keystates[KS_I] = false;
                keyevent_released[KP_I] = true;
                break;
            case SDLK_j:
                keystates[KS_J] = false;
                keyevent_released[KP_J] = true;
                break;
            case SDLK_k:
                keystates[KS_K] = false;
                keyevent_released[KP_K] = true;
                break;
            case SDLK_l:
                keystates[KS_L] = false;
                keyevent_released[KP_L] = true;
                break;
            case SDLK_m:
                keystates[KS_M] = false;
                keyevent_released[KP_M] = true;
                break;
            case SDLK_n:
                keystates[KS_N] = false;
                keyevent_released[KP_N] = true;
                break;
            case SDLK_o:
                keystates[KS_O] = false;
                keyevent_released[KP_O] = true;
                break;
            case SDLK_p:
                keystates[KS_P] = false;
                keyevent_released[KP_P] = true;
                break;
            case SDLK_q:
                keystates[KS_Q] = false;
                keyevent_released[KP_Q] = true;
                break;
            case SDLK_r:
                keystates[KS_R] = false;
                keyevent_released[KP_R] = true;
                break;
            case SDLK_s:
                keystates[KS_S] = false;
                keyevent_released[KP_S] = true;
                break;
            case SDLK_t:
                keystates[KS_T] = false;
                keyevent_released[KP_T] = true;
                break;
            case SDLK_u:
                keystates[KS_U] = false;
                keyevent_released[KP_U] = true;
                break;
            case SDLK_v:
                keystates[KS_V] = false;
                keyevent_released[KP_V] = true;
                break;
            case SDLK_w:
                keystates[KS_W] = false;
                keyevent_released[KP_W] = true;
                break;
            case SDLK_x:
                keystates[KS_X] = false;
                keyevent_released[KP_X] = true;
                break;
            case SDLK_y:
                keystates[KS_Y] = false;
                keyevent_released[KP_Y] = true;
                break;
            case SDLK_z:
                keystates[KS_Z] = false;
                keyevent_released[KP_Z] = true;
                break;

            // Funktionstasten
            case SDLK_RETURN:
                keystates[KS_RETURN] = false;
                keyevent_released[KP_RETURN] = true;
                break;
            case SDLK_UP:
                keystates[KS_UP] = false;
                keyevent_released[KP_UP] = true;
                break;
            case SDLK_DOWN:
                keystates[KS_DOWN] = false;
                keyevent_released[KP_DOWN] = true;
                break;
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch(current_event.button.button)
            {
            case SDL_BUTTON_LEFT:
                mousestates[MS_LEFT] = true;
                mouseevent_pressed[MP_LEFT] = true;
                break;
            case SDL_BUTTON_MIDDLE:
                mousestates[MS_WHEEL] = true;
                mouseevent_pressed[MP_WHEEL] = true;
                break;
            case SDL_BUTTON_RIGHT:
                mousestates[MS_RIGHT] = true;
                mouseevent_pressed[MP_RIGHT] = true;
                break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch(current_event.button.button)
            {
            case SDL_BUTTON_LEFT:
                mousestates[MS_LEFT] = false;
                mouseevent_released[MP_LEFT] = true;
                break;
            case SDL_BUTTON_MIDDLE:
                mousestates[MS_WHEEL] = false;
                mouseevent_released[MP_WHEEL] = true;
                break;
            case SDL_BUTTON_RIGHT:
                mousestates[MS_RIGHT] = false;
                mouseevent_released[MP_RIGHT] = true;
                break;
            }
            break;
        case SDL_WINDOWEVENT:
            switch(current_event.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                windowevent[WE_CLOSE] = true;
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                windowevent[WE_MAXIMIZED] = true;
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                windowevent[WE_MINIMIZED] = true;
                break;
            }
            break;
        }
    }
}

void ResetTriggeredEvents()
{
    // keyboard
    for(unsigned int i=0; i<KP_LAST_CONSTANT; i++)
        keyevent_pressed[i] = false;
    for(unsigned int i=0; i<KR_LAST_CONSTANT; i++)
        keyevent_released[i] = false;
    // mouse
    for(unsigned int i=0; i<MP_LAST_CONSTANT; i++)
        mouseevent_pressed[i] = false;
    for(unsigned int i=0; i<MR_LAST_CONSTANT; i++)
        mouseevent_released[i] = false;
    // window
    for(unsigned int i=0; i<WE_LAST_CONSTANT; i++)
        windowevent[i] = false;
}
