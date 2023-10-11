#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>

#include "defines.hpp"
#include "shader.hpp"

#define Vao unsigned int

struct Window {
    int width;
    int height;
    GLFWwindow* handle;
};

static Window globalWindow;
static Vao squareVao;
static glm::mat4 projection;

static void updateViewport(int width, int height) {
    glViewport(0, 0, width, height);
}

static void updateProjection() {
    projection = glm::ortho(
        (float) (-globalWindow.width / 2),
        (float) (globalWindow.width / 2),
        (float) (-globalWindow.height / 2),
        (float) (globalWindow.height / 2),
        0.1f,
        100.0f
    );
}

static void frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
    updateViewport(width, height);
    globalWindow.width = width;
    globalWindow.height = height;
    updateProjection();
}

static void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    globalWindow.width = 1280;
    globalWindow.height = 720;
    globalWindow.handle = 
        glfwCreateWindow(
            globalWindow.width, 
            globalWindow.height,
            "Physics",
            NULL,
            NULL
        );
    if (!globalWindow.handle) {
        printf("Failed to create window\n");
    }
    glfwSetFramebufferSizeCallback(globalWindow.handle, frameBufferResizeCallback);
    glfwMakeContextCurrent(globalWindow.handle);
    glfwSwapInterval(0);
}

static void setupSquareVao() {
    float vertices[] = {
        -1,  1,
         1,  1,
        -1, -1,
         1, -1,
    };

    unsigned int indices[] = {
        1, 3, 2,
        0, 1, 2,
    };

    GL(glGenVertexArrays(1, &squareVao));
    GL(glBindVertexArray(squareVao));

    unsigned int buffers[2];
    GL(glGenBuffers(2, buffers));
    unsigned int vertexBuffer = buffers[0];
    unsigned int indexBuffer = buffers[1];

    GL(glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer));
    GL(glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(float) * 8,
        vertices,
        GL_STATIC_DRAW
    ));
    GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer));
    GL(glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * 6,
        indices,
        GL_STATIC_DRAW
    ));

    GL(glEnableVertexAttribArray(0));
    GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
}

int main() {
    initWindow();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("failed to load required extensions\n");
    }

    GL(glEnable(GL_BLEND));
    GL(glEnable(GL_MULTISAMPLE));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    updateProjection();
    setupSquareVao();

    glm::mat4 view = 
        glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0), glm::vec3(0, 1, 0));

    GridShader gridShader = createGridShader(
        "../shader/gridVert.glsl",
        "../shader/gridFrag.glsl"
    );

    float delta = 0.0f;
    float lastFrame = 0.0f;
    GL(glClearColor(0.1, 0.1, 0.1, 1));

    while (!glfwWindowShouldClose(globalWindow.handle)) {
        if (glfwGetKey(globalWindow.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(globalWindow.handle, true);
        }

        float currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        GL(glClear(GL_COLOR_BUFFER_BIT));

        GL(glUseProgram(gridShader.id));
        GL(glBindVertexArray(squareVao));
        glm::vec2 screenDimensions(globalWindow.width, globalWindow.height);
        setUniformVec2(gridShader.uScreenDimensions, &screenDimensions);
        GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        glfwSwapBuffers(globalWindow.handle);
        glfwPollEvents();
    }
}
