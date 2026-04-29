const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

char* readShaderSource(const char* filePath) {
    FILE* file = fopen(filePath, "rb");
    if (file == NULL) {
        printf("Could not open file: %s\n", filePath);
        return NULL;
    }

    // Move pointer to end of file to find the size
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate memory and read
    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);
    buffer[length] = '\0'; // OpenGL needs a null-terminated string

    fclose(file);
    return buffer;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0,0,width,height);
}

void proccessInput(GLFWwindow *window) 
{
    if (glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1); 
}

int main() {

    glfwInit(); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "SimpleShader", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

// vertex input - normalised device coordinates
float vertices[] = {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f,  0.0f
};
unsigned int indices[] = {
    0 , 1, 3,
    1,  2 ,3
};


// puts the data from ram into gpu 
unsigned int VBO, VAO, EBO;
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);

glBindBuffer(GL_ARRAY_BUFFER, VBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices ), vertices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

glBindVertexArray(0);

// for square
// 1. bind Vertex Array Object

//unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

//glShaderSource(vertexShader,1,&vertexShaderSource, NULL);
//glCompileShader(vertexShader);

//int success;
//char infoLog[512];
//glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
//if (!success) {
 //   glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
   // printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
//}

//unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
//glCompileShader(fragmentShader);

// --- LOAD FROM FILES INSTEAD OF STRINGS ---
char* vertexSource = readShaderSource("../shaders/shader.vert");
char* fragmentSource = readShaderSource("../shaders/shader.frag");

if (vertexSource == NULL || fragmentSource == NULL) {
    printf("Failed to load shader files!\n");
    return -1;
}

// Vertex Shader
unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, (const char**)&vertexSource, NULL); // Cast to const char**
glCompileShader(vertexShader);


// Fragment Shader
unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentShader, 1, (const char**)&fragmentSource, NULL);
glCompileShader(fragmentShader);


free(vertexSource);
free(fragmentSource);


unsigned int shaderProgram;
shaderProgram = glCreateProgram();

glAttachShader(shaderProgram,vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

glUseProgram(shaderProgram);



glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);




// 2. Create the shader objects
    while (!glfwWindowShouldClose(window)) {
        
        proccessInput(window);

        //pink
        //glClearColor(0.9f,0.2f,0.4,1.0f);
        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        glUseProgram(shaderProgram);


        int resLoc = glGetUniformLocation(shaderProgram, "u_resolution");
        glUniform2f(resLoc, 800.0f, 600.0f);

        float timeValue = (float)glfwGetTime();
        int timeLoc = glGetUniformLocation(shaderProgram, "u_time");
        glUniform1f(timeLoc, timeValue);
    
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT, 0);
        //glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    
    // memory leaks 
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    return 0; 
    }

