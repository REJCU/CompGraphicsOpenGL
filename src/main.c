#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include <../textures/stb_image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stdbool.h>

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
// float vertices[] = {
 //    0.5f,  0.5f, 0.0f,
   //  0.5f, -0.5f, 0.0f,
   // -0.5f, -0.5f, 0.0f,
    //-0.5f,  0.5f, 0.0f,
//};

// makes the rectangle 
//unsigned int indices[] = {
  //  0 , 1, 3,
  //  1,  2 ,3
//};

//float vertices[] = {
 //    1.0f,  1.0f, 0.0f,  // Top Right
 //    1.0f, -1.0f, 0.0f,  // Bottom Right
 //   -1.0f, -1.0f, 0.0f,  // Bottom Left
 //   -1.0f,  1.0f, 0.0f   // Top Left
//};

// for textures
float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

};

unsigned int indices[] = {
    0, 1, 3,   // First Triangle
    1, 2, 3    // Second Triangle
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

// for shader
//glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//glEnableVertexAttribArray(0);

// for textures
// 3 floats (x,y,z), Stride is 8 (total floats per row), Offset is 0
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
glEnableVertexAttribArray(0);

// 2. Texture Coordinate Attribute (Location 2 - matching your code)
// 2 floats (u,v), Stride is 8, Offset is 6 (skips x,y,z and r,g,b)
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
glEnableVertexAttribArray(2);
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

char* vertexSource = readShaderSource("../shaders/texture.vert");
char* fragmentSource = readShaderSource("../shaders/sampler.frag");

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

// define features - took out of while loop as it called it every single frame
    int resLoc = glGetUniformLocation(shaderProgram, "u_resolution");
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    float timeValue = (float)glfwGetTime();
    int timeLoc = glGetUniformLocation(shaderProgram, "u_time");

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    int mouseLoc = glGetUniformLocation(shaderProgram, "u_mouse");


stbi_set_flip_vertically_on_load(true);
int texWidth, texHeight, nrChannels;
unsigned char *data1 = stbi_load("../textures/tux.jpg", &texWidth, &texHeight, &nrChannels, 0); 

if (!data1) {
    printf("Failed to load texture\n");
}

unsigned int tex1;
    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data1);

// Second texture
int tw2, th2, ch2; 
unsigned char *data2 = stbi_load("../textures/PNG.png", &tw2, &th2, &ch2, 4);

if (!data2) {
    printf("Failed to load texture\n");
};

unsigned int tex2;
    glGenTextures(1, &tex2);
    glBindTexture(GL_TEXTURE_2D, tex2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw2, th2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data2);


glActiveTexture(GL_TEXTURE0);
glBindTexture(GL_TEXTURE_2D, tex1);
glActiveTexture(GL_TEXTURE1);
glBindTexture(GL_TEXTURE_2D, tex2);

glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

int image1 = glGetUniformLocation(shaderProgram, "image1"); 
int image2 = glGetUniformLocation(shaderProgram, "image2");

glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);



// 2. Create the shader objects
while (!glfwWindowShouldClose(window)) {
    proccessInput(window);

        glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT); 

        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex1);
        glActiveTexture(GL_TEXTURE1); 
        glBindTexture(GL_TEXTURE_2D, tex2);

        // Second texture

        glBindVertexArray(VAO);

        glUniform1i(glGetUniformLocation(shaderProgram, "inputBuffer"), 0);
        glUniform2f(resLoc, (float)width, (float)height);
        glUniform1f(timeLoc, timeValue);
        glUniform2f(mouseLoc, (float)xpos, (float)ypos);

        glUniform1i(image1, 0); 
        glUniform1i(image2, 1);
    
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6 ,GL_UNSIGNED_INT, 0);

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

