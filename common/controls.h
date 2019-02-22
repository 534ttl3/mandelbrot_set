#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// mandelbrot set ranges from
// x: -2.5 to 1 => width of 3.5 units => 300*3.5 = 1050
// i: -1 to 1 => height of 2 units => 300*2 = 600
#define WINDOW_WIDTH 1050
#define WINDOW_HEIGHT 600


#include <glm/glm.hpp>
#include <SDL2/SDL.h>



enum KEYSTATES
{
    KS_0 = 0, KS_1, KS_2, KS_3, KS_4, KS_5, KS_6, KS_7, KS_8, KS_9,
    KS_A, KS_B, KS_C, KS_D, KS_E, KS_F, KS_G, KS_H, KS_I, KS_J,
    KS_K, KS_L, KS_M, KS_N, KS_O, KS_P, KS_Q, KS_R, KS_S, KS_T,
    KS_U, KS_V, KS_W, KS_X, KS_Y, KS_Z, KS_SHIFT, KS_STRG, KS_SPACE, KS_RETURN,
    KS_UP, KS_DOWN, KS_LEFT, KS_RIGHT, KS_KP_PLUS, KS_KP_MINUS, KS_LAST_CONSTANT
};

enum KEYEVENT_PRESSED
{
    KP_0 = 0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9,
    KP_A, KP_B, KP_C, KP_D, KP_E, KP_F, KP_G, KP_H, KP_I, KP_J,
    KP_K, KP_L, KP_M, KP_N, KP_O, KP_P, KP_Q, KP_R, KP_S, KP_T,
    KP_U, KP_V, KP_W, KP_X, KP_Y, KP_Z, KP_SHIFT, KP_STRG, KP_SPACE, KP_RETURN,
    KP_UP, KP_DOWN, KP_LEFT, KP_RIGHT, KP_KP_PLUS, KP_KP_MINUS, KP_LAST_CONSTANT
};

enum KEYEVENT_RELEASED
{
    KR_0 = 0, KR_1, KR_2, KR_3, KR_4, KR_5, KR_6, KR_7, KR_8, KR_9,
    KR_A, KR_B, KR_C, KR_D, KR_E, KR_F, KR_G, KR_H, KR_I, KR_J,
    KR_K, KR_L, KR_M, KR_N, KR_O, KR_P, KR_Q, KR_R, KR_S, KR_T,
    KR_U, KR_V, KR_W, KR_X, KR_Y, KR_Z, KR_SHIFT, KR_STRG, KR_SPACE, KR_RETURN,
    KR_UP, KR_DOWN, KR_LEFT, KR_RIGHT, KR_KR_PLUS, KR_KR_MINUS, KR_LAST_CONSTANT
};

enum MOUSESTATES
{
    MS_LEFT = 0, MS_WHEEL, MS_RIGHT, MS_LAST_CONSTANT
};

enum MOUSEEVENT_PRESSED
{
    MP_LEFT = 0, MP_WHEEL, MP_RIGHT, MP_LAST_CONSTANT
};

enum MOUSEEVENT_RELEASED
{
    MR_LEFT = 0, MR_WHEEL, MR_RIGHT, MR_LAST_CONSTANT
};

enum WINDOWEVENT
{
    WE_CLOSE = 0, WE_MINIMIZED, WE_MAXIMIZED, WE_RESIZED, WE_LAST_CONSTANT
};

void InitCamera();
void UpdateCamera(SDL_Window* window);
glm::mat4 GetViewMatrix();
glm::mat4 GetProjectionMatrix();

void MoveForward();
void MoveBackward();


// input handler
void InitEventListener();
bool IsKeyPressed(KEYSTATES kcode);
bool IsKeyPressTriggered(KEYEVENT_PRESSED ptkey);
bool IsKeyReleaseTriggered(KEYEVENT_RELEASED rtkey);
bool IsMousePressed(MOUSESTATES mcode);
bool IsMousePressTriggered(MOUSEEVENT_PRESSED ptmouse);
bool IsMouseReleaseTriggered(MOUSEEVENT_RELEASED rtmouse);
bool IsWindowEventTriggered(WINDOWEVENT wecode);

void AssignKeyboardAndMouseEvents();
void ResetTriggeredEvents();

void ReactToEvents();

#endif
