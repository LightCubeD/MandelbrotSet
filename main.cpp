#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <math.h>
#include <malloc.h>
#include <fstream>
#include <string>
#include <sstream>
#include <assert.h>

#include "renderer.h"
#include "vertexBuffer.h"
#include "indexBuffer.h"
#include "shader.h"
#include "vertexArray.h"
#include "Robject.h"

int main(int argc, char** argv)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;


  /* Create a windowed to know whats the highest opengl version supported*/
  GLFWwindow* context = glfwCreateWindow(1, 1, "contextgetter", NULL, NULL);
  if (!context)
  {
    glfwTerminate();
    return -1;
  }
  glfwSetWindowPos(context, 0, 0);

  /* Make the window's context current */
  glfwMakeContextCurrent(context);

  std::string version((char*)glGetString(GL_VERSION));
  std::cout << "highest context: " << version.at(0) << "." << version.at(2) << std::endl;

  glfwDestroyWindow(context);

  const int maj = (const int)version.at(0) - 48;
  const int min = (const int)version.at(2) - 48;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, maj);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, min);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  // 4xMSAA
  glfwWindowHint(GLFW_SAMPLES, 8);


  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(800, 800, "MandelbrotSet", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK)
    std::cout << "Error initialilzing GLEW!" << std::endl;


  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
    -1.0f,  1.0f,
  };
  
  unsigned int indecies[] = {
    0, 1, 2,
    2, 3, 0
  };

  vertexBufferLayout* layout = new vertexBufferLayout;
  layout->push<float>(2);
  Robject* pixels = new Robject(positions, 2*4, indecies, 2*3, layout);

  shader* standard = new shader();
  standard->addShader(GL_VERTEX_SHADER, "./shaders/vertex.shader", SH_FILEPATH);
  standard->addShader(GL_FRAGMENT_SHADER, "./shaders/fragment.shader", SH_FILEPATH);
  standard->compileShader();
  standard->bind();

  glBindVertexArray(0);
  glUseProgram(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  bool once = true;
  float angle = 0;
  float scaler = 8;

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    //if(once) {
      standard->bind();
      standard->setUniformf("angle",3.14159265358*0.9455);
      standard->setUniformf("scaler",scaler);
      angle += 3.141592/(float)2048;
      scaler = scaler / 1.01;
      pixels->draw(GL_TRIANGLES, 6, 0);
      glfwSwapBuffers(window);
      once = false;
    //}
    glfwPollEvents();
  }

  delete pixels;
  delete standard;

  glfwTerminate();
  return 0;
}