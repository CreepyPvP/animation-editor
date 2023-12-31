#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>

#include "defines.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "geometry_generator.hpp"
#include "font.hpp"
#include "nine_slice.hpp"
#include "texture_atlas.hpp"
#include "ui.hpp"
#include "render_queue.hpp"

#define Vao unsigned int

struct Window {
    int width;
    int height;
    GLFWwindow* handle;
};

static Window globalWindow;
static Vao squareVao;

static glm::mat4 projection;

static UiContext context;

static void updateViewport(int width, int height) {
    glViewport(0, 0, width, height);
}

static void updateProjection() {
    projection = glm::ortho(
        0.0f, 
        (float) globalWindow.width, 
        (float) globalWindow.height, 
        0.0f, 
        .1f,
        1000.0f
    );
}

static void frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
    updateViewport(width, height);
    globalWindow.width = width;
    globalWindow.height = height;

    updateProjection();
}

static void mouseCallback(GLFWwindow* window, double posX, double posY) {
    context.mouseX = posX;
    context.mouseY = posY;
}

static void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    globalWindow.width = 1280;
    globalWindow.height = 720;
    globalWindow.handle = glfwCreateWindow(
        globalWindow.width, 
        globalWindow.height,
        "Animation Editor",
        NULL,
        NULL
    );
    if (!globalWindow.handle) {
        printf("Failed to create window\n");
    }

    glfwSetFramebufferSizeCallback(globalWindow.handle, frameBufferResizeCallback);
    glfwSetCursorPosCallback(globalWindow.handle, mouseCallback);
    glfwMakeContextCurrent(globalWindow.handle);

    // int fbW, fbH;
    // glfwGetFramebufferSize(globalWindow.handle, &fbW, &fbH);
    // printf("fbW: %d, fbH: %d\n", fbW, fbH);

    updateProjection();
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
    // GL(glEnable(GL_MULTISAMPLE));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    setupSquareVao();
    unsigned int uiVao = setupUiVao();

    GridShader gridShader = createGridShader(
        "../shader/gridVert.glsl",
        "../shader/gridFrag.glsl"
    );

    UiShader uiShader = createUiShader(
        "../shader/uiVert.glsl",
        "../shader/uiFrag.glsl"
    );

    FontShader fontShader = createFontShader(
        "../shader/fontVert.glsl", 
        "../shader/fontFrag.glsl"
    );

    unsigned int texture = loadTexture("../assets/ui.png");

    unsigned int fontAtlas;
    Font font;
    setupFontAtlas(&fontAtlas, &font, "../assets/fonts/EndFont.ttf");

    GeometryGenerator geometryGenerator;
    geometryGenerator.init(4000, 4000);
    context.geometry = &geometryGenerator;
    context.active = -1;

    RenderQueue queue;
    queue.fontShader = fontShader.id;
    queue.fontAtlas = fontAtlas;
    queue.uiShader = uiShader.id;
    queue.uiAtlas = texture;
    queue.init(1000);
    context.queue = &queue;

    float delta = 0.0f;
    float lastFrame = 0.0f;
    int mouseState = GLFW_RELEASE;
    GL(glClearColor(0.1, 0.1, 0.1, 1));


    while (!glfwWindowShouldClose(globalWindow.handle)) {
        if (glfwGetKey(globalWindow.handle, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(globalWindow.handle, true);
        }

        float currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // update ui input
        context.hot = -1;
        int newMouseState = glfwGetMouseButton(globalWindow.handle, GLFW_MOUSE_BUTTON_LEFT);
        context.mouseFlags = 0;
        if (mouseState != newMouseState) {
            mouseState = newMouseState;
            if (mouseState == GLFW_PRESS) {
                context.mouseFlags |= MOUSE_JUST_PRESSED;
            } else {
                context.mouseFlags |= MOUSE_JUST_RELEASED;
            }
        }

        // update projection uniforms + clear screen
        GL(glUseProgram(uiShader.id));
        GL(setUniformMat4(uiShader.uProjection, &projection));
        GL(glUseProgram(fontShader.id));
        GL(setUniformMat4(fontShader.uProjection, &projection));
        GL(glClear(GL_COLOR_BUFFER_BIT));

        // draw grid
        GL(glUseProgram(gridShader.id));
        GL(glBindVertexArray(squareVao));
        glm::vec2 screenDimensions(globalWindow.width, globalWindow.height);
        setUniformVec2(gridShader.uScreenDimensions, &screenDimensions);
        GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        geometryGenerator.startBatch();
        geometryGenerator.drawString(100, 100, "Test test.. !! #", &font, 1);
        Batch fontBatch = geometryGenerator.endBatch();

        bool clicked = button(200, 200, 150, 100, 1, &context);
        button(400, 200, 150, 100, 2, &context);
        button(600, 200, 150, 100, 3, &context);
        if (clicked) {
            printf("clicked button\n");
        }

        geometryGenerator.updateUiBuffers(uiVao);
        geometryGenerator.reset();

        queue.renderFont(fontBatch);
        queue.execute();

        glfwSwapBuffers(globalWindow.handle);
        glfwPollEvents();
    }

    glfwTerminate();
}
