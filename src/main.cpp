#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float2.hpp>

#include "defines.hpp"
#include "shader.hpp"

// IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define Vao unsigned int

struct Window {
    int width;
    int height;
    GLFWwindow* handle;
};

static Window globalWindow;
static Vao squareVao;

static void updateViewport(int width, int height) {
    glViewport(0, 0, width, height);
}

static void frameBufferResizeCallback(GLFWwindow *window, int width, int height) {
    updateViewport(width, height);
    globalWindow.width = width;
    globalWindow.height = height;
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

    // IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(globalWindow.handle, true);
    ImGui_ImplOpenGL3_Init();
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

static void renderMenu() {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_NoTitleBar;
    // if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    windowFlags |= ImGuiWindowFlags_MenuBar;
    windowFlags |= ImGuiWindowFlags_NoMove;
    windowFlags |= ImGuiWindowFlags_NoResize;
    // if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    // if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    // if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    // if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    // if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

    ImVec2 pivot = ImVec2(1, 0);
    ImVec2 pos = ImVec2(globalWindow.width, 0);
    ImGui::SetNextWindowPos(pos, 0, pivot);

    if (!ImGui::Begin("Some gui goes here", NULL, windowFlags)) {
        ImGui::End();
        return;
    }
    if (ImGui::CollapsingHeader("Animations")) {

    }

    ImGui::End();
}

int main() {
    initWindow();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("failed to load required extensions\n");
    }

    GL(glEnable(GL_BLEND));
    GL(glEnable(GL_MULTISAMPLE));
    GL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    setupSquareVao();

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

        // IMGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        renderMenu();

        float currentFrame = glfwGetTime();
        delta = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        GL(glClear(GL_COLOR_BUFFER_BIT));

        GL(glUseProgram(gridShader.id));
        GL(glBindVertexArray(squareVao));
        glm::vec2 screenDimensions(globalWindow.width, globalWindow.height);
        setUniformVec2(gridShader.uScreenDimensions, &screenDimensions);
        GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

        // IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(globalWindow.handle);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
}
