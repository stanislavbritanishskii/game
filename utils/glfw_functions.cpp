#include "utils.hpp"

#define STB_IMAGE_IMPLEMENTATION // Define this here
#include <stb/stb_image.h> // Include after defining the implementation


// Function to load texture from file
GLuint loadTexture(const char *path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	int width, height, nrChannels;
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	} else
	{
		std::cout << "Failed to load texture: " << path << std::endl;
	}
	stbi_image_free(data);
	return textureID;
}

// Function to set up the vertex data
void setupVertices(GLuint &VAO, GLuint &VBO, GLuint &EBO)
{
	float vertices[] = {
		// Positions        // Texture Coords
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom Left
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom Right
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f, // Top Right
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f // Top Left
	};

	unsigned int indices[] = {
		0, 1, 2, // First Triangle
		2, 3, 0 // Second Triangle
	};

	// Generate and bind the Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate and bind the Vertex Buffer Object
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Generate and bind the Element Buffer Object
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Define the vertex attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbind the buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


GLFWwindow *init_glfw_window(int x, int y)
{
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);


	// Create a window
	GLFWwindow *window = glfwCreateWindow(x, y, "game", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glewInit();

	// Set viewport
	glViewport(0, 0, x, y);

	return window;
}

const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(position, 1.0);
	TexCoord = texCoord;
}
)";

// Fragment Shader Source Code
const char *fragmentShaderSource = R"(
#version 330 core
in vec2 TexCoord;
out vec4 color;

uniform sampler2D texture1;

void main() {
	color = texture(texture1, TexCoord);
}
)";


GLuint init_shader()
{
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return shaderProgram;
}


void renderTexture(unsigned int shaderProgram, unsigned int texture, unsigned int VAO, float x, float y, float rotation,
					int width, int height, float scale)
{
	// Use the shader program
	glUseProgram(shaderProgram);
	glBindTexture(GL_TEXTURE_2D, texture);
	x /= (width / 2);
	y /= (height / 2);
	float scale_x = (scale / width * 2);
	float scale_y = (scale / height * 2);

	// Define the transformation matrix
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(x, y, 0.0f)); // Position
	transform = glm::scale(transform, glm::vec3(scale_x, scale_y, 1.0f)); // Scale
	transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation

	// Set the transformation uniform
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	// Render the texture
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}