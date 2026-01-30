#include "Config.h"

const double G = 6.6743e-11;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
glm::mat4 projection;

float yaw = -90.0f;
float pitch = 0.0f;
float fov = 45.0f;
float deltaTime;

std::vector<Planet> planets{
    {glm::vec3{0.0f, 0.0f, -3.0f}, 0.5f, 5},
    {glm::vec3{2.0f, 0.0f, -3.0f}, 0.5f, 5},


};

std::vector<Mesh> meshes;

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

    meshes.push_back(std::move(Mesh{planets[0].combinedVertices, planets[0].indices})); // Sun
    meshes.push_back(std::move(Mesh{planets[1].combinedVertices, planets[1].indices})); // Earth

    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/SunTexture.png"})); // Sun
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/EarthTexture.png"})); // Earth

    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float lastTime = 0;

while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraPos += cameraSpeed * cameraFront;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraPos -= cameraSpeed * cameraFront;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cameraPos.y += cameraSpeed;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cameraPos.y -= cameraSpeed;
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


    //    for (auto& planet : planets)
    //    {
    //        for (auto& other : planets)
    //        {
    //            if (&planet == &other) continue;

    //            float distance = sqrt(pow(planet.Position[0] - other.Position[0], 2) + (planet.Position[1] - other.Position[1], 2));

    //            std::vector<double> direction = { other.Position[0] / distance, other.Position[1] / distance};
    //            distance *= 1000;

    //            double Gforce = (G * planet.mass * other.mass) / (distance * distance);
    //            float acceleration = Gforce / planet.mass;

    //            std::vector<double> acc = { direction[0] * acceleration, direction[1] * acceleration};

				////planet.UpdateVelocity(acc[0] / 96, acc[1] / 96, '+');

    //            if (planet.Position[1] < 0 || planet.Position[1] > WHeight)
    //            {
    //                planet.UpdateVelocity(0.0f, -0.95f, 0.0f, '*');
    //            }
    //            if (planet.Position[0] < 0 || planet.Position[0] > WWidth)
    //            {
    //                planet.UpdateVelocity(-0.95f, 0.0f, 0.0f, '*');
    //            }
    //        }

    //        planet.UpdatePosition();

    //        model = glm::translate(model, planet.Position);
    //    }

        float aspectRatio = (float)WWidth / (float)WHeight;

        projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

        for (auto& planet : planets)
        {
			planet.model = glm::mat4(1.0f);
            planet.model = glm::translate(planet.model, planet.Position);
		}

        for (int i = 0; i < planets.size(); i++)
        {
            shaders[i].use();
            shaders[i].setMat4("model", planets[i].model);
            shaders[i].setMat4("view", view);
            shaders[i].setMat4("projection", projection);
            shaders[i].setInt("ourTexture", 0);
            meshes[i].draw();
		}

        glfwSwapBuffers(window);

        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
