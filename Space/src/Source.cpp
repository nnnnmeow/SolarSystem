#include "Config.h"
#define WHeight 920
#define WWidth 780
const double G = 6.6743e-11;

std::vector<Planet> planets{
    
};

Planet ab(std::vector<float>{0.0, 0.0}, std::vector<float>{0.5f, 0.5f}, 1.0f, 5);

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

	std::vector<float> Cube1Vertices = {
        //vertices         
        0.3f,  0.3f, 0.0f,    // top right
        0.3f, -0.3f, 0.0f,    // bottom right
        -0.3f, -0.3f, 0.0f,   // bottom left
        -0.3f,  0.3f, 0.0f,   // top left
        0.3f, 0.3f, 0.5f,     //back top right
		0.3f, -0.3f, 0.5f,    //back bottom right
	    -0.3f, -0.3f, 0.5f,   //back bottom left
	    -0.3f,  0.3f, 0.5f,   //back top left
        //colors
         0.5f, 0.0f, 0.0f, // top right color
         1.0f, 0.0f, 0.0f, // bottom right color
         0.0f, 0.5f, 0.0f, // bottom left color
         0.0f, 1.0f, 0.0f, // top left color
         0.0f, 0.0f, 0.5f, //back top right color
         0.0f, 0.0f, 1.0f, //back bottom right color
         0.5f, 0.5f, 0.0f, //back bottom left color
         1.0f, 1.0f, 0.0f  //back top left color
	};

    std::vector<unsigned int> Cube1Indices = {
        0, 1, 3,   // first triangle
        1, 2, 3,   // second triangle
		4, 5, 7,   // first triangle back
		5, 6, 7,   // second triangle back
		0, 1, 4,   // first triangle right
		1, 5, 4,   // second triangle right
		1, 2, 5,   // first triangle bottom
		2, 6, 5,   // second triangle bottom
		2, 3, 6,   // first triangle left
		3, 7, 6,   // second triangle left
		0, 3, 4,   // first triangle top
		3, 7, 4    // second triangle top
	};

    /*Mesh Cube(Cube1Vertices, Cube1Indices);
    Shader a("Shaders/Vertex/SquareVertex.glsl", "Shaders/Fragment/SquareFragment.glsl");
    shaders.push_back(a);*/

    Mesh Planet(ab.vertices, ab.indices);
    Shader PlanetShader("Shaders/Vertex/SquareVertex.glsl", "Shaders/Fragment/SquareFragment.glsl");

    glfwSetCursorPosCallback(window, mouse_cursor_callback);

while (!glfwWindowShouldClose(window))
    {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
        /*a.use();
        Cube.draw();
        */

        ab.CalculateCoords();
        ab.draw();

        PlanetShader.use();
        Planet.draw();

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
