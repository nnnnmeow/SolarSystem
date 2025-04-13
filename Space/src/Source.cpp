#include "Config.h"
#define WHeight 920
#define WWidth 780

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WWidth, WHeight, "Space", NULL, NULL);
    glViewport(0, 0, WWidth, WHeight);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    Planet sun;
    Planet moon;

	std::vector<Planet> planets;

	planets.push_back(sun);

	planets.push_back(moon);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WWidth, 0, WHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3f(1.0f, 5.0f, 5.0f);

        for (auto& planet : planets)
        {
            planet.DrawPlanet(planet.Position[0], planet.Position[1], 50);

            planet.UpdatePosition(0.0f, planet.Velocity[1]);

            planet.UpdateVelocity(0.0f, (-9.81 / 20.0f), '+');

            if (planet.Position[1] < 0 || planet.Position[1] > WHeight)
            {
                planet.UpdateVelocity(0.0f, -0.95f, '*');
            }
            if (planet.Position[0] < 0 || planet.Position[0] > WWidth)
            {
                planet.UpdateVelocity(-0.95f, 0.0f, '*');
            }
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}