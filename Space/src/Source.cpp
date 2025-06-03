#include "Config.h"
#define WHeight 920
#define WWidth 780
const double G = 6.6743e-11;

int main(void)
{

    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    window = glfwCreateWindow(WWidth, WHeight, "Space", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    gladLoadGL();

    std::vector<Planet> planets{
        Planet(std::vector<double>{0.0, 0.0}, std::vector<double>{WWidth / 2, WHeight / 2}, 50.0f, 7.35 * pow(10, 22)),
        Planet(std::vector<double>{0.0, 0.0}, std::vector<double>{(WWidth / 2) + 300, WHeight / 2}, 25.0f, 7.35 * pow(10, 22))
    };

    glViewport(0, 0, WWidth, WHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WWidth, 0, WHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

while (!glfwWindowShouldClose(window))
    {

        glClear(GL_COLOR_BUFFER_BIT);

        for (auto& planet : planets)
        {
            for (auto& other : planets)
            {
                if (&planet == &other) continue;

                float distance = sqrt(pow(planet.Position[0] - other.Position[0], 2) + (planet.Position[1] - other.Position[1], 2));

                std::vector<double> direction = { other.Position[0] / distance, other.Position[1] / distance};
                distance *= 1000;

                double Gforce = (G * planet.mass * other.mass) / (distance * distance);
                float acceleration = Gforce / planet.mass;

                std::vector<double> acc = { direction[0] * acceleration, direction[1] * acceleration};

				planet.UpdateVelocity(acc[0] / 96, acc[1] / 96, '+');

                if (planet.Position[1] < 0 || planet.Position[1] > WHeight)
                {
                    planet.UpdateVelocity(0.0f, -0.95f, '*');
                }
                if (planet.Position[0] < 0 || planet.Position[0] > WWidth)
                {
                    planet.UpdateVelocity(-0.95f, 0.0f, '*');
                }
            }

            planet.UpdatePosition();


            planet.DrawPlanet();
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
