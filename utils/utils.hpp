#ifndef UTILS_HPP
#define UTILS_HPP

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb/stb_image.h> // Only include here, no implementation
#include <iostream>

GLFWwindow* init_glfw_window(int x, int y);
GLuint loadTexture(const char* path);
void setupVertices(GLuint& VAO, GLuint& VBO, GLuint& EBO);
GLuint  init_shader();
void renderTexture(unsigned int shaderProgram, unsigned int texture, unsigned int VAO, float x, float y, float rotation,
					int width, int height, float scale);


#endif