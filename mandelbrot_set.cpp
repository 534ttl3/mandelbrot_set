// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <SDL2/SDL.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include <iostream>
using namespace std;

#include "common/shader.h"
#include "common/controls.h"

#include "common/datastructures.h"
#include "common/CObjectImp.h"


void Screenshot(int x, int y, int w, int h, const char * filename)
{
    unsigned char * pixels = new unsigned char[w*h*4]; // 4 bytes for RGBA
    glReadPixels(x,y,w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    SDL_Surface * surf = SDL_CreateRGBSurfaceFrom(pixels, w, h, 8*4, w*4, 0,0,0,0);
    SDL_SaveBMP(surf, filename);

    SDL_FreeSurface(surf);
    delete [] pixels;
}

struct cpl_num
{
    cpl_num(){ }
    cpl_num(float _r, float _i)
    {
        r = _r; i = _i;
    }
    float r;
    float i;
};


struct texture
{
    texture(unsigned int _width, unsigned int _height, unsigned int _numof_colors)
    {
        width = _width; height = _height; numof_colors = _numof_colors;
        arraylength = height*width*numof_colors;
        pixels = new char[arraylength];

        for(int i=0; i<width*height*numof_colors; i++)
            pixels[i] = 255;
    }
    void SetPixel(unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b)
    {
        pixels[y*width*numof_colors+x*numof_colors] = r;
        pixels[y*width*numof_colors+x*numof_colors+1] = g;
        pixels[y*width*numof_colors+x*numof_colors+2] = b;
    }
    void SetPixel(unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b, unsigned int a)
    {
        // cout << y << " " << width << " " << numof_colors << " + " << numof_colors << " " << endl;
        int idx = y*width*numof_colors+x*numof_colors;
        if( idx > arraylength-5) {            return;
        }
        // cout << "accessing idx " << idx << " of " << height*width*numof_colors << endl;
        pixels[idx] = r;
        pixels[idx+1] = g;
        pixels[idx+2] = b;
        pixels[idx+3] = a;
        // cout <<  "pixel set" << endl;
    }

    char* pixels;
    unsigned int width, height, numof_colors;
    unsigned int arraylength;
};

const int MAX_ITERATION = 100;

unsigned int IsInMandelbrotSet(cpl_num& cn)
{
    float x0 = cn.r;
    float y0 = cn.i;

    float x = 0.0;
    float y = 0.0;
    int iteration = 0;
    while ( x*x + y*y < 2*2  && iteration < MAX_ITERATION)
    {
        float xtemp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtemp;
        iteration++;
    }

    return iteration;
}

int main(int argc, char* argv[])
{
    // Initialise GLFW
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        cout << "Error SDL_Init(): not correctly initialized" << endl;
    }
    else
    {
        cout << "SDL_Init(SDL_INIT_EVERYTHING) successful" << endl;
    }

    // create window and OpenGL context
    SDL_Window* current_window = SDL_CreateWindow("OpenGL render to texture", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GL_CreateContext(current_window);
    // Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "Error glewInit: " << glewGetErrorString(err) << endl;
    }
    else
    {
        cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
    }

    SDL_WarpMouseInWindow(current_window, WINDOW_WIDTH/2, WINDOW_HEIGHT/2);

    // Dark blue background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Create and compile our GLSL program from the shaders
    GLuint texturequad_shader = LoadShaders( "texturedquad.vs", "texturedquad.fs" );

    GLuint renderedTexture;
    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);

    texture *mytex = new texture(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

    cout << "computing the pixels ..." << endl;

    float cp_r_left = -2.5;
    float cp_r_right = 1.0;
    float cp_i_down = -1.0;
    float cp_i_up = 1.0;

    float cp_r_length = (float)(abs((double)cp_r_left) + abs((double)cp_r_right));

    // how many pixels are to be calculated?
    // calculate x float step

    int counter = 0;

    float pixelstep = cp_r_length/(float)WINDOW_WIDTH;
    // aspect ratio always stays the same, if not, calculate steps for x and y seperately

    float si; // scale i (imaginary part of complex number/plane)
    unsigned int pcy; // pixel counter for x pixels
    for(si = cp_i_down, pcy = 0;
        si <= cp_i_up && pcy <= WINDOW_HEIGHT;
        si += pixelstep, pcy++)
    {
        float sr; // scale r (real part of complex number/plane)
        unsigned int pcx;  // pixel counter for y pixels
        for(sr = cp_r_left, pcx = 0;
            sr <= cp_r_right && pcx <= WINDOW_WIDTH;
            sr += pixelstep, pcx++)
        {
            cpl_num cn(sr, si);
            unsigned int iterations = IsInMandelbrotSet(cn);

            mytex->SetPixel(pcx, pcy, (char)((255/MAX_ITERATION) * iterations),  (char)((255/MAX_ITERATION) * iterations),  (char)((255/MAX_ITERATION) * iterations), 255);
            counter++;
        }
    }
    cout << "DONE computing the pixels" << endl;

    cout << counter << endl;

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, mytex->pixels);

    // Poor filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // The fullscreen quad's FBO
    std::vector<vec3> quad_vpositions = { vec3(-1.0f, -1.0f, 0.0f),
                                          vec3( 1.0f, -1.0f, 0.0f),
                                          vec3(-1.0f,  1.0f, 0.0f),
                                          vec3( 1.0f,  1.0f, 0.0f) };

    std::vector<unsigned int> quad_indices = { 0, 1, 2,
                                               2, 1, 3 };

    std::vector<vec2> quad_vtexcoords = { vec2(0, 0),
                                          vec2(1, 0),
                                          vec2(0, 1),
                                          vec2(1, 1) };

    GLuint quad_VAO;
    glGenVertexArrays(1, &quad_VAO);
    glBindVertexArray(quad_VAO);

    GLuint quad_vpositions_buf;
    glGenBuffers(1, &quad_vpositions_buf);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vpositions_buf);
    glBufferData(GL_ARRAY_BUFFER, quad_vpositions.size() * sizeof(glm::vec3), &quad_vpositions[0], GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(texturequad_shader, "vPosition"), 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint quad_vtexcoords_buf;
    glGenBuffers(1, &quad_vtexcoords_buf);
    glBindBuffer(GL_ARRAY_BUFFER, quad_vtexcoords_buf);
    glBufferData(GL_ARRAY_BUFFER, quad_vtexcoords.size() * sizeof(vec2), &quad_vtexcoords[0], GL_STATIC_DRAW);
    glVertexAttribPointer(glGetAttribLocation(texturequad_shader, "vTexCoords"), 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint quad_indices_buf;
    glGenBuffers(1, &quad_indices_buf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quad_indices_buf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quad_indices.size() * sizeof(unsigned int), &quad_indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);

    InitEventListener();
    do
    {
        // do the event listening
        AssignKeyboardAndMouseEvents();
        ReactToEvents();

        // Render to screen
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(texturequad_shader);

        // Bind framebuffer texture in Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderedTexture);
        glUniform1i(glGetUniformLocation(texturequad_shader, "renderedTexture"), 0);

        glBindVertexArray(quad_VAO);
        glEnableVertexAttribArray(glGetAttribLocation(texturequad_shader, "vPosition"));
        glEnableVertexAttribArray(glGetAttribLocation(texturequad_shader, "vTexCoords"));
        glDrawElements(GL_TRIANGLES, quad_indices.size(), GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray(glGetAttribLocation(texturequad_shader, "vPosition"));
        glDisableVertexAttribArray(glGetAttribLocation(texturequad_shader, "vTexCoords"));
        glBindVertexArray(0);

        // Swap buffers
        SDL_GL_SwapWindow(current_window);
    } // Check if the X key was pressed
    while(!(IsKeyPressTriggered(KP_X) || IsWindowEventTriggered(WE_CLOSE)));

    // Screenshot(int x, int y, int w, int h, const char * filename)
    Screenshot(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, "image.bmp");
    // Close OpenGL window and terminate SDL
    SDL_Quit();

    return 0;
}

