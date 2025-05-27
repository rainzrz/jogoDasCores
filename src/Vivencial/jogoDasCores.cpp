#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>

struct Color {
    float r, g, b;
};

struct Rectangle {
    float x, y, width, height;
    Color color;
    bool active;
};

const int GRID_ROWS = 5;
const int GRID_COLS = 5;
const float COLOR_THRESHOLD = 0.3f;

std::vector<Rectangle> rectangles;
int score = 0;
int attempts = 0;

unsigned int shaderProgram, VBO, VAO;

Color randomColor() {
    return {static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX};
}

float colorDistance(Color a, Color b) {
    return sqrt(pow(a.r - b.r, 2) + pow(a.g - b.g, 2) + pow(a.b - b.b, 2));
}

void resetGame() {
    rectangles.clear();
    score = 0;
    attempts = 0;

    float rectWidth = 2.0f / GRID_COLS;
    float rectHeight = 2.0f / GRID_ROWS;

    for (int i = 0; i < GRID_ROWS; i++) {
        for (int j = 0; j < GRID_COLS; j++) {
            Rectangle rect;
            rect.x = -1.0f + j * rectWidth;
            rect.y = -1.0f + i * rectHeight;
            rect.width = rectWidth;
            rect.height = rectHeight;
            rect.color = randomColor();
            rect.active = true;
            rectangles.push_back(rect);
        }
    }

    std::cout << "\nBem-vindo ao Jogo das Cores!\n";
    std::cout << "Clique com o botao esquerdo do mouse para selecionar um retangulo.\n";
    std::cout << "Pressione 'R' para reiniciar o jogo.\n";
}

void drawRectangles() {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    for (const auto& rect : rectangles) {
        if (!rect.active) continue;

        int offsetLoc = glGetUniformLocation(shaderProgram, "offset");
        glUniform2f(offsetLoc, rect.x, rect.y);

        int scaleLoc = glGetUniformLocation(shaderProgram, "scale");
        glUniform2f(scaleLoc, rect.width, rect.height);

        int colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform3f(colorLoc, rect.color.r, rect.color.g, rect.color.b);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        float xNorm = (xpos / width) * 2 - 1;
        float yNorm = 1 - (ypos / height) * 2;

        for (auto& rect : rectangles) {
            if (xNorm >= rect.x && xNorm <= rect.x + rect.width &&
                yNorm >= rect.y && yNorm <= rect.y + rect.height) {

                if (!rect.active) {
                    std::cout << "Este retangulo ja foi removido!\n";
                    return;
                }

                Color chosenColor = rect.color;

                int removed = 0;
                for (auto& r : rectangles) {
                    if (r.active && colorDistance(chosenColor, r.color) < COLOR_THRESHOLD) {
                        r.active = false;
                        removed++;
                    }
                }

                int multiplier = std::max(1, 10 - attempts);
                score += removed * multiplier;
                attempts++;

                std::cout << "Tentativa: " << attempts
                          << ", Removidos: " << removed
                          << ", Pontuacao: " << score << "\n";

                bool allRemoved = true;
                for (const auto& r : rectangles) {
                    if (r.active) {
                        allRemoved = false;
                        break;
                    }
                }
                if (allRemoved) {
                    std::cout << "Fim de jogo! Pontuacao total: " << score << "\n";
                }

                break;
            }
        }
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        resetGame();
        std::cout << "Jogo reiniciado!\n";
    }
}

unsigned int createShaderProgram() {
    const char* vertexShaderSource = R"glsl(
        #version 330 core
        layout (location = 0) in vec2 aPos;

        uniform vec2 offset;
        uniform vec2 scale;

        void main() {
            gl_Position = vec4((aPos * scale) + offset, 0.0, 1.0);
        }
    )glsl";

    const char* fragmentShaderSource = R"glsl(
        #version 330 core
        out vec4 FragColor;

        uniform vec3 ourColor;

        void main() {
            FragColor = vec4(ourColor, 1.0);
        }
    )glsl";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProg = glCreateProgram();
    glAttachShader(shaderProg, vertexShader);
    glAttachShader(shaderProg, fragmentShader);
    glLinkProgram(shaderProg);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProg;
}

void setupBuffers() {
    float vertices[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(600, 600, "Jogo das Cores", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    shaderProgram = createShaderProgram();
    setupBuffers();
    resetGame();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawRectangles();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
