// XJLearnOpengl2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>

#include "GraphSystem.h"
#include "Camera.h"
#include "Shader.h"
#include "Model.h"
#include <iostream>
using namespace xjg;
using namespace std;
GraphSystem graphSystem;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

Camera camera(glm::vec3(0, 0, 3), glm::vec3(0, -90, 0));
int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	graphSystem.setScreenSize(1280, 720);
	GLFWwindow *window = glfwCreateWindow(graphSystem.screenWidth, graphSystem.screenHeight, "LearnOPENGL", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, graphSystem.screenWidth, graphSystem.screenHeight);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	graphSystem.setCurrentCamera(&camera);
	Model nanosuit("nanosuit.obj");
	graphSystem.addModel(&nanosuit);
	//==========Test Plane=======================
	float testVert[]{ -0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f };
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(testVert), testVert, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	Shader shader;
	shader.loadShader();
	//===================================
	//unsigned int fbo;
	//glGenFramebuffers(1, &fbo);
	//glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		cout << "fbo is completed" << endl;
	}
	else
	{
		cout << "fbo is not completed" << endl;
	}
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//==========================================
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		//=========TEST===============
		//shader.use();
		//glm::mat4 model(glm::vec4(1, 0, 0, 0), glm::vec4(0, 1, 0, 0), glm::vec4(0, 0, 1, 0), glm::vec4(0, 0, 0, 1));
		//shader.setMatrix4("app.model", model);
		//shader.setMatrix4("app.view", camera.getViewMatrix());
		//shader.setMatrix4("app.projection", graphSystem.getProjMat());
		//glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//===============================
		graphSystem.drawModels();
		//centerMesh.draw(shader, nanoModel.getModelMatrix());
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

    return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.mouse_callback(window, xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	camera.processInput(window);
	//camera.processInput(window);
}