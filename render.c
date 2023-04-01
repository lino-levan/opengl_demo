#include <stdio.h>

#include "render.h"

unsigned int bufferVertices(void) {
  // Generate a VAO
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  // Bind it to our current session
  glBindVertexArray(VAO);

  // Get some dummy vertices
  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  // Get ID for buffer
  unsigned int VBO;
  glGenBuffers(1, &VBO);

  // Bind the buffer as the current buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Buffer an array with a length 9 and put it at a location with slow writes and fast reads
  glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);

  // Tell OpenGL how to interpret our vertex data
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  // TODO: Understand what this does
  glEnableVertexAttribArray(0);

  return VAO;
}

// The HLSL code for the vertex shader
const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";

// The HLSL code for the fragment shader
const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\0";

// Returns ID of shader
unsigned int createShader(const char *shaderString, GLenum type) {
  // Get ID for shader
  unsigned int shader = glCreateShader(type);

  // Attach and compile the shader
  glShaderSource(shader, 1, &shaderString, NULL);
  glCompileShader(shader);

  // Print any errors from the process to do error handling
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if(!success)
  {
    char error[512];
    glGetShaderInfoLog(shader, 512, NULL, error);
    fprintf(stderr, "Shader failed to compile: %s\n", error);
  }

  return shader;
}

// Returns ID of shader program
unsigned int createShaderProgram(unsigned int vertexShader, unsigned int fragmentShader) {
  // Get ID
  unsigned int shaderProgram = glCreateProgram();

  // Attach shaders
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  // Link the shader to openGL
  glLinkProgram(shaderProgram);

  // Print any error messages from shader linking process
  int success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if(!success) {
    char error[512];
    glGetProgramInfoLog(shaderProgram, 512, NULL, error);
    fprintf(stderr, "Shader Program failed to compile: %s\n", error);
  }

  return shaderProgram;
}

void render(GLFWwindow *window) {
  // Clear last render
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // Buffer the vertices in the GPU
  unsigned int VAO = bufferVertices();

  // Generate the shader program
  unsigned int vertexShader = createShader(vertexShaderSource, GL_VERTEX_SHADER);
  unsigned int fragmentShader = createShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
  unsigned int shaderProgram = createShaderProgram(vertexShader, fragmentShader);

  // Enable using the shader
  glUseProgram(shaderProgram);

  // Clean up unnecessary shader stuff
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Bind our vertices
  glBindVertexArray(VAO);

  // RENDER OUR TRIANGLE 🎉 🎉 🎉
  glDrawArrays(GL_TRIANGLES, 0, 3);
}