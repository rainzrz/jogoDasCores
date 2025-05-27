#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>  // Para std::max

// Estrutura de cor RGB
struct Color {
    float r, g, b;
};

// Estrutura de retângulo
struct Rectangle {
    float x, y, width, height;
    Color color;
    bool active;  // Se foi removido ou não
};

// Variáveis globais
const int GRID_ROWS = 5;
const int GRID_COLS = 5;
const float COLOR_THRESHOLD = 0.3f;  // Tolerância de similaridade

std::vector<Rectangle> rectangles;
int score = 0;
int attempts = 0;

// Função para gerar cor aleatória
Color randomColor() {
    return {static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX,
            static_cast<float>(rand()) / RAND_MAX};
}

// Similaridade de cor (distância Euclidiana)
float colorDistance(Color a, Color b) {
    return sqrt(pow(a.r - b.r, 2) + pow(a.g - b.g, 2) + pow(a.b - b.b, 2));
}

// Reiniciar o jogo
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

// Renderizar todos os retângulos
void drawRectangles() {
    for (const auto& rect : rectangles) {
        if (!rect.active) continue;

        glColor3f(rect.color.r, rect.color.g, rect.color.b);
        glBegin(GL_QUADS);
        glVertex2f(rect.x, rect.y);
        glVertex2f(rect.x + rect.width, rect.y);
        glVertex2f(rect.x + rect.width, rect.y + rect.height);
        glVertex2f(rect.x, rect.y + rect.height);
        glEnd();
    }
}

// Callback de clique do mouse
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int width, height;
        glfwGetWindowSize(window, &width, &height);

        // Converter posição do clique para coordenadas OpenGL
        float xNorm = (xpos / width) * 2 - 1;
        float yNorm = 1 - (ypos / height) * 2;

        // Identificar o retângulo clicado
        for (auto& rect : rectangles) {
            if (xNorm >= rect.x && xNorm <= rect.x + rect.width &&
                yNorm >= rect.y && yNorm <= rect.y + rect.height) {

                if (!rect.active) {
                    std::cout << "Este retangulo ja foi removido!\n";
                    return;
                }

                Color chosenColor = rect.color;

                int removed = 0;
                // Remover retângulos com cor similar
                for (auto& r : rectangles) {
                    if (r.active && colorDistance(chosenColor, r.color) < COLOR_THRESHOLD) {
                        r.active = false;
                        removed++;
                    }
                }

                // Atualizar pontuação
                int multiplier = std::max(1, 10 - attempts);
                score += removed * multiplier;
                attempts++;

                std::cout << "Tentativa: " << attempts 
                          << ", Removidos: " << removed 
                          << ", Pontuacao: " << score << "\n";

                // Verificar se todos foram removidos
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

                break;  // Para não processar múltiplos cliques
            }
        }
    }
}

// Callback de tecla
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        resetGame();
        std::cout << "Jogo reiniciado!\n";
    }
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(600, 600, "Jogo das Cores", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetKeyCallback(window, keyCallback);

    resetGame();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawRectangles();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
