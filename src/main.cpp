
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>
#include <cmath>
#include "shader.h"

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	std::cout << "Hello " << std::endl;
}

static void frame_buff_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int main(int argc, char *argv[])
{
	GLFWwindow *window;

	if (!glfwInit())
	{
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "demo window", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// glfwSwapInterval(1);
	glfwSetWindowCloseCallback(window, [](GLFWwindow *window) {
		std::cout << "window is closed" << std::endl;
	});
	glfwSetKeyCallback(window, key_callback);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, frame_buff_size_callback);

	//着色器相关
	Shader shader("shader/vertex.vs", "shader/fragment.fs");

	const float firstTriangle[] = {
		// first triangle        //颜色
		-0.9f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // left
		-0.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // right
		-0.45f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // top

	};
	// const float secondTriangle[] = {
	// 	// second triangle
	// 	0.0f, -0.5f, 0.0f, // left
	// 	0.9f, -0.5f, 0.0f, // right
	// 	0.45f, 0.5f, 0.0f  // top

	// };
	//VAO
	//创建一组vao
	GLuint vao[2];
	glGenVertexArrays(2, vao);
	//新一组建缓冲区
	GLuint vbo[2];
	glGenBuffers(2, vbo);

	//第一个三角形初始化
	//绑定vao0
	glBindVertexArray(vao[0]);
	//绑定到顶点缓冲类型
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	//配置数据绑定缓冲vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	//至此已经把顶点vertices数据储存在显卡内存中，使用VBO顶点缓冲对象来管理
	//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	//位置属性(layout location=0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	//颜色属性(layout location=1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//第二个三角形初始化
	// glBindVertexArray(vao[1]);
	// glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	// glEnableVertexAttribArray(0);

	//开启线框模式
	// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	/* 循环直到window关闭 */
	while (!glfwWindowShouldClose(window))
	{
		/* render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader.use();
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// float timeValue = glfwGetTime();
		// float green = (sin(timeValue) / 2.0f) + 0.5f;
		// int vertexColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
		// //切换vao
		// glBindVertexArray(vao[1]);
		// glUniform4f(vertexColorLocation, 0.0f, green, 0.0f, 1.0f);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		//交换前后缓冲
		glfwSwapBuffers(window);
		/* 论寻处理事件 */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}