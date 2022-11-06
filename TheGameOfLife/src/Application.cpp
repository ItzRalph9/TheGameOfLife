#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <vector>

// TODO:
// Make all the arrays, unique- or shared pointers (prob shared, cause you need a temp backup of the array)

void DrawSquare(int xPos, int yPos)
{
    glColor3f(0.0f, 1.0f, 0.0f);
    float x = xPos / 20.0f - 1;
    float y = ((40.0f - yPos) / 20.0f) - 1;
    float size = 0.05f;

    glVertex2f(x, y); // vertex 1
    glVertex2f(x, y - size); // vertex 2
    glVertex2f(x + size, y - size); // vertex 3
    glVertex2f(x + size, y); // vertex 4
}

void DrawLines()
{
    glColor3f(0.25f, 0.25f, 0.25f);
    float border = 0.004f;

    glBegin(GL_QUADS);
    for (int col = 1; col < 40; col++)
    {
        float x = col / 20.0f - 1;
        glVertex2f(x, 1.0f); // vertex 1
        glVertex2f(x, -1.0f); // vertex 2
        glVertex2f(x + border, -1.0f); // vertex 3
        glVertex2f(x + border, 1.0f); // vertex 4
    }

    for (int row = 1; row < 40; row++)
    {
        float y = ((40.0f - row) / 20.0f) - 1;
        glVertex2f(-1.0f, y); // vertex 1
        glVertex2f(-1.0f, y - border); // vertex 2
        glVertex2f(1.0f, y - border); // vertex 3
        glVertex2f(1.0f, y); // vertex 4
    }
    glEnd();
}

std::vector<int> CalculateFrame(const std::vector<int> data)
{
    // rules:
    // if cell is alive, it stays alive when it has 2 or 3 alive neighbours, else it dies
    // if cell is dead, it springs to life when it has exactly 3 alive neighbours

    std::vector<int> temp(1600);
    for (int i = 0; i < 1600; i++) { temp[i] = data[i]; }

    int pos[8] = { -41,-40,-39,-1,1,39,40,41 };

    for (int i = 0; i < 1600; i++)
    {
        bool isAlive = false;
        if (data[i] == 1) isAlive = true;

        int neighbours = 0;
        for (const int cellOfsset : pos)
        {
            if (i - cellOfsset >= 0 && i - cellOfsset < 1600 && data[i - cellOfsset] == 1)
            {
                if (i % 40 == 0 && (i - cellOfsset) % 40 != 1)
                    continue;

                if (i % 40 == 39 && (i - cellOfsset) % 40 != 38)
                    continue;
                 
                neighbours++;
            }
        }

        if (isAlive && neighbours != 2 && neighbours != 3) temp[i] = 0;
        if (!isAlive && neighbours == 3) temp[i] = 1;
    }

    return temp;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "The Game Of Life", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::vector<int> pixelData(1600);
    pixelData[215] = 1;
    pixelData[255] = 1;
    pixelData[295] = 1;
    pixelData[294] = 1;
    pixelData[253] = 1;

    pixelData[415] = 1;
    pixelData[455] = 1;
    pixelData[495] = 1;
    pixelData[496] = 1;
    pixelData[457] = 1;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_QUADS);
        for (int i = 0; i < 40; i++)
        {
            for (int j = 0; j < 40; j++)
            {
                if (pixelData[j * 40 + i] == 1)
                {
                    DrawSquare(i, j);
                }
            }
        }
        glEnd();


        DrawLines();
        pixelData = CalculateFrame(pixelData);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        glfwSwapInterval(5);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}