#include "Config.h"
#define WHeight 920
#define WWidth 780
const double G = 6.6743e-11;

std::vector<Planet> planets{
    
};

glm::mat4 globalTransform = glm::mat4(1.0f);

std::vector<Shader> shaders;

double LastXPos = 0;
double LastYPos = 0;

void mouse_cursor_callback(GLFWwindow* window, double XPos, double YPos)
{

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        LastXPos = XPos;
        LastYPos = YPos;
        return;
    }

    //std::cout << XPos << " " << YPos << std::endl;

    double deltaX = XPos - LastXPos;
    double deltaY = YPos - LastYPos;

    if (LastXPos != XPos && LastYPos != YPos)
    {
        if (XPos > LastXPos && YPos > LastYPos)
        {
            deltaX = -(XPos - LastXPos);
            deltaY = -(YPos - LastYPos);
        }
        else if (XPos < LastXPos && YPos < LastYPos)
        {
            deltaX = LastXPos - XPos;
            deltaY = LastYPos - YPos;
        }
        else if (XPos > LastXPos && YPos < LastYPos)
        {
            deltaX = -(XPos - LastXPos);
            deltaY = LastYPos - YPos;
        }
        else if (XPos < LastXPos && YPos > LastYPos)
        {
            deltaX = LastXPos - XPos;
            deltaY = LastYPos - YPos;
        }
    }
    else if (LastXPos != XPos && LastYPos == YPos)
    {
        if (XPos > LastXPos)
        {
            deltaX = -(XPos - LastXPos);
            deltaY = 0;
        }
        else if (XPos < LastXPos)
        {
            deltaX = LastXPos - XPos;
            deltaY = 0;
        }
    }
    else if (LastXPos == XPos && LastYPos != YPos)
    {
        if (YPos > LastYPos)
        {
            deltaX = 0;
            deltaY = -(YPos - LastYPos);
        }
        else if (YPos < LastYPos)
        {
            deltaX = 0;
            deltaY = LastYPos - YPos;
        }
    }

    if (fabs(deltaX) < 0.001 && fabs(deltaY) < 0.001)
        return;

    float sensitivity = 0.005f;

    glm::vec3 axis = glm::normalize(glm::vec3((float)deltaY, (float)deltaX, 0.0f));
    float angle = sqrt(deltaX * deltaX + deltaY * deltaY) * sensitivity;

    globalTransform = glm::rotate(globalTransform, angle, axis);

    for (auto& a : shaders)
    {
        a.setMat4("transform", globalTransform);
    }

    LastXPos = XPos;
    LastYPos = YPos;
}

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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WWidth, 0, WHeight, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    Planet ab(std::vector<float>{0.0, 0.0}, std::vector<float>{WWidth / 2, WHeight / 2}, 100.0f, 5);
    ab.CalculateCoords();
    /*for (size_t i = 0; i < ab.vertices.size(); i++)
    {
        std::cout << "Vertices: " << ab.vertices[i] << std::endl;
    }
    for (size_t i = 0; i < ab.indices.size(); i++)
    {
        std::cout << "Indices: " << ab.indices[i] << std::endl;
    }*/

    Mesh Planet(ab.vertices, ab.indices);
    Shader PlanetShader("Shaders/Vertex/SquareVertex.glsl", "Shaders/Fragment/SquareFragment.glsl");
	shaders.push_back(PlanetShader);

    glfwSetCursorPosCallback(window, mouse_cursor_callback);

while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        /*for (auto& planet : planets)
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

				//planet.UpdateVelocity(acc[0] / 96, acc[1] / 96, '+');

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

			shaders[0].use();

            planet.DrawPlanet();
        }*/

        glm::mat4 projection = glm::ortho(0.0f, (float)WWidth, 0.0f, (float)WHeight);
        glm::mat4 view = glm::mat4(1.0f);

        PlanetShader.use();
        PlanetShader.setMat4("projection", projection);
        PlanetShader.setMat4("view", view);

        Planet.draw();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
