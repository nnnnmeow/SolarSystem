#include "Config.h"

const double G = 6.6743e-11;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view;
float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
float deltaTime;

std::vector<Planet> planets{
    
};

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

	float Xoffset = XPos - LastXPos;
	float Yoffset = LastYPos - YPos;
    LastXPos = XPos;
    LastYPos = YPos;

    float sensitivity = 0.1f;
    Xoffset *= sensitivity;
    Yoffset *= sensitivity;

    yaw += Xoffset;
    pitch += Yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
    
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    for (auto& a : shaders)
    {
        a.setMat4("view", view);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    float cameraSpeed = 2.5f * deltaTime;
    switch (key)
    {
        case GLFW_KEY_W:
            cameraPos += cameraSpeed * cameraFront;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            for (auto& a : shaders)
            {
                a.setMat4("view", view);
            }
            break;
        case GLFW_KEY_S:
            cameraPos -= cameraSpeed * cameraFront;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            for (auto& a : shaders)
            {
                a.setMat4("view", view);
            }
            break;
        case GLFW_KEY_A:
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            for (auto& a : shaders)
            {
                a.setMat4("view", view);
            }
            break;
        case GLFW_KEY_D:
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            for (auto& a : shaders)
            {
                a.setMat4("view", view);
            }
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, true);
			break;
        default:
            break;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 45.0f)
    {
        fov -= yoffset;
	}
    if (fov <= 1.0f)
    {
        fov = 1.0f;
    }
    if (fov >= 45.0f)
    {
        fov = 45.0f;
    }
}


int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    GLFWmonitor* primary = glfwGetPrimaryMonitor();
    int WWidth, WHeight;
    glfwGetMonitorWorkarea(primary, NULL, NULL, &WWidth, &WHeight);

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

    Planet ab(std::vector<float>{0.0, 0.0}, std::vector<float>{0.0f, 0.0f}, 0.5f, 5);
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
    Shader PlanetShader("Shaders/Vertex/SunVertex.glsl", "Shaders/Fragment/SunFragment.glsl", "Shaders/Textures/EarthTexture.png");
	shaders.push_back(PlanetShader);

    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    float lastTime = 0;

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

        float aspectRatio = (float)WWidth / (float)WHeight;

        glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

        PlanetShader.use();
        PlanetShader.setMat4("projection", projection);

        PlanetShader.setInt("ourTexture", 0);

        Planet.draw();

        glfwSwapBuffers(window);

        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
