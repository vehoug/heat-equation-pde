#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <heat_solver.hpp>
#include <renderer.hpp>

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main()
{
  //=================================================//
  //---------------GLFW Initialization---------------//
  //=================================================//

  if (!glfwInit())
  {
    std::cerr << "Failed to initialize GLFW" << '\n';
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Heat Equation Simulation", NULL, NULL);
  if (!window)
  {
    std::cerr << "Failed to create GLFW window" << '\n';
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cerr << "Failed to initialize GLAD" << '\n';
    return -1;
  }

  //=================================================//
  //-------------------Heat Solver-------------------//
  //=================================================//

  const int gridWidth = 100;
  const int gridHeight = 100;

  heat_equation::HeatSolver heatSolver(gridWidth, gridHeight);

  heatSolver.setHeatSource(gridWidth / 2, gridHeight / 2, 100.0f);
  heatSolver.setHeatSource(gridWidth / 4, gridHeight / 4, 75.0f);
  heatSolver.setHeatSource(3 * gridWidth / 4, 3 * gridHeight / 4, 75.0f);

  //=================================================//
  //-------------------Renderer----------------------//
  //=================================================//

  rendering::Renderer renderer(gridWidth, gridHeight);

  float lastTime = glfwGetTime();
  float timestep = 0.01f;

  while (!glfwWindowShouldClose(window))
  {
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    processInput(window);

    heatSolver.update(timestep);
    renderer.render(heatSolver.getTemperatureGrid());

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();

  return 0;
}
