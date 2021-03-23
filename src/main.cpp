
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <iostream>


static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	
	std::cout<<"Hello "<<std::endl;
}

static void frame_buff_size_callback(GLFWwindow * window,int width,int height){
	glViewport(0,0,width,height);
}



const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShader1Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";
const char *fragmentShader2Source = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "}\n\0";



GLuint compileShader(GLuint type,const char* source){
	GLuint shader=glCreateShader(type);
	glShaderSource(shader,1,&source,nullptr);
	glCompileShader(shader);
	#ifdef DEBUG
	int success;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
	if(!success){
		char infoLog[512];
		glGetShaderInfoLog(shader,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::"<<type<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	#endif
	return shader;
}

GLuint linkProgram(GLuint vertexShader,GLuint fragmentShader){
	GLuint program=glCreateProgram();
	glAttachShader(program,vertexShader);
	glAttachShader(program,fragmentShader);
	glLinkProgram(program);
	#ifdef DEBUG
	int success;
	glGetProgramiv(program,GL_COMPILE_STATUS,&success);
	std::cout<<"hellp"<<std::endl;
	if(!success){
		char infoLog[512];
		glGetProgramInfoLog(program,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::Program::COMPILATION_FAILED\n" << infoLog << std::endl;
		return 0;
	}
	#endif
	return program;
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
	glfwSetKeyCallback(window,key_callback);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
    	return -1;
	}
	glViewport(0,0,800,600);
	glfwSetFramebufferSizeCallback(window,frame_buff_size_callback);

	//着色器相关
	//顶点着色器
	GLuint vertexShader=compileShader(GL_VERTEX_SHADER,vertexShaderSource);
	if(!vertexShader){
		std::cout<<"编译顶点着色器失败"<<std::endl;
		glfwTerminate();
		return -1;
	}
	//片段着色器
	GLuint fragmentShader=compileShader(GL_FRAGMENT_SHADER,fragmentShader1Source);
	if(!fragmentShader){
		glfwTerminate();
		return -1;
	}
	//着色器程序
	GLuint shaderProgram=linkProgram(vertexShader,fragmentShader);
	if(!fragmentShader){
		glfwTerminate();
		return -1;
	}


	GLuint fragmentShader2=compileShader(GL_FRAGMENT_SHADER,fragmentShader2Source);
	GLuint shaderProgram2=linkProgram(vertexShader,fragmentShader2);
	if(!shaderProgram2){
		glfwTerminate();
		return -1;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(fragmentShader2);

	const float firstTriangle[] = {
   // first triangle
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f  // top

	};
		const float secondTriangle[] = {
        // second triangle
         0.0f, -0.5f, 0.0f,  // left
         0.9f, -0.5f, 0.0f,  // right
         0.45f, 0.5f, 0.0f   // top 

	};
	//VAO
	//创建一组vao
	GLuint vao[2];
	glGenVertexArrays(2,vao);
	//新一组建缓冲区
	GLuint vbo[2];
	glGenBuffers(2,vbo);

	//第一个三角形初始化
	//绑定vao0
	glBindVertexArray(vao[0]);
	//绑定到顶点缓冲类型
	glBindBuffer(GL_ARRAY_BUFFER,vbo[0]);
	//配置数据绑定缓冲vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
	//至此已经把顶点vertices数据储存在显卡内存中，使用VBO顶点缓冲对象来管理
	//告诉OpenGL该如何解析顶点数据（应用到逐个顶点属性上）
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	//第二个三角形初始化
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER,vbo[1]);
	glBufferData(GL_ARRAY_BUFFER,sizeof(secondTriangle),secondTriangle,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	//开启线框模式
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	/* 循环直到window关闭 */
	while (!glfwWindowShouldClose(window))
	{
		/* render here */
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(vao[0]);
		glDrawArrays(GL_TRIANGLES,0,3);

		glUseProgram(shaderProgram2);
		//切换vao
		glBindVertexArray(vao[1]);
		glDrawArrays(GL_TRIANGLES,0,3);
		//交换前后缓冲
		glfwSwapBuffers(window);
		/* 论寻处理事件 */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}