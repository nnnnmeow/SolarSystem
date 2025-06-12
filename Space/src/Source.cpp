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
    std::vector<Shader> shaders{
        Shader("Shaders/Vertex/SunVertex.glsl", "Shaders/Fragment/SunFragment.glsl")
    };

	std::vector<float> CubeVertices = {
        //vertices         
        0.5f,  0.5f, 0.0f,    // top right
        0.5f, -0.5f, 0.0f,    // bottom right
        -0.5f, -0.5f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   // top left
        0.5f, 0.5f, 1.0f,     //back top right
		0.5f, -0.5f, 1.0f,    //back bottom right
	    -0.5f, -0.5f, 1.0f,   //back bottom left
	    -0.5f,  0.5f, 1.0f,   //back top left
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

    std::vector<unsigned int> CubeIndices = {
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

    Mesh Cube(CubeVertices, CubeIndices);
	Shader CubeShader("Shaders/Vertex/SquareVertex.glsl", "Shaders/Fragment/SquareFragment.glsl");


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
        CubeShader.use();
        Cube.draw();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.2f, 0.0f, 0.2f));

		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        unsigned int transformLoc = glGetUniformLocation(CubeShader.getID(), "transform");
        CubeShader.setMat4("transform", trans);
		CubeShader.setMat4("view", view);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
