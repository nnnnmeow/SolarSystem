#include "Config.h"

const double G = 1.34e-30;

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
        {glm::vec3{0.0f, 0.0f, -3.0f}, 0.6f, 1.989e30},      
        {glm::vec3{1.87f, 0.0f, -3.0f}, 0.0096f, 3.285e23},
        {glm::vec3{2.55f, 0.0f, -3.0f}, 0.0238f, 4.867e24},
        {glm::vec3{3.0f, 0.0f, -3.0f}, 0.025f, 5.9722e24},
        {glm::vec3{3.70f, 0.0f, -3.0f}, 0.0133f, 6.41693e23},
        {glm::vec3{6.84f, 0.0f, -3.0f}, 0.274f, 1.898e27},
        {glm::vec3{9.27f, 0.0f, -3.0f}, 0.229f, 5.683e26},
        {glm::vec3{13.14f, 0.0f, -3.0f}, 0.1f, 8.68103e25},
        {glm::vec3{16.45f, 0.0f, -3.0f}, 0.097f, 1.024e26},
    };

std::vector<Mesh> meshes;

std::vector<Shader> shaders;

double LastXPos = 0;
double LastYPos = 0;

void mouse_cursor_callback(GLFWwindow* window, double XPos, double YPos)
{

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
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
    meshes.push_back(std::move(Mesh{planets[1].combinedVertices, planets[1].indices})); // Mercury
    meshes.push_back(std::move(Mesh{planets[2].combinedVertices, planets[2].indices})); // Venus
    meshes.push_back(std::move(Mesh{planets[3].combinedVertices, planets[3].indices})); // Earth
    meshes.push_back(std::move(Mesh{planets[4].combinedVertices, planets[4].indices})); // Mars
    meshes.push_back(std::move(Mesh{planets[5].combinedVertices, planets[5].indices})); // Jupiter
    meshes.push_back(std::move(Mesh{planets[6].combinedVertices, planets[6].indices})); // Saturn
    meshes.push_back(std::move(Mesh{planets[7].combinedVertices, planets[7].indices})); // Uranus
    meshes.push_back(std::move(Mesh{planets[8].combinedVertices, planets[8].indices})); // Neptune

    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/SunTexture.png"})); // Sun
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/MercuryTexture.png"})); // Mercury
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/VenusTexture.png"})); // Venus
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/EarthTexture.png"})); // Earth
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/MarsTexture.png"})); // Mars
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/JupiterTexture.png"})); // Jupiter
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/SaturnTexture.png"})); // Saturn
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/UranusTexture.png"})); // Uranus
    shaders.push_back(std::move(Shader{"Shaders/Vertex/PlanetVertex.glsl", "Shaders/Fragment/PlanetFragment.glsl", "Shaders/Textures/NeptuneTexture.png"})); // Neptune

    // Initial orbital velocities (perpendicular to Sun direction) for circular orbits
    double M_sun = planets[0].mass;
    for (int i = 1; i < planets.size(); i++) {
        double r = glm::length(planets[i].Position - planets[0].Position);
        double v = sqrt(G * M_sun / r);
        planets[i].Velocity.z = (float)v;
    }

    glfwSetCursorPosCallback(window, mouse_cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float lastTime = glfwGetTime();

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

        float aspectRatio = (float)WWidth / (float)WHeight;

        projection = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

        for (auto& planet : planets) {
            for (auto& other : planets) {
                if (&planet == &other) continue;
                double distance = planet.CalculateDistance(other);

                double dx = other.Position[0] - planet.Position[0];
                double dy = other.Position[1] - planet.Position[1];
                double dz = other.Position[2] - planet.Position[2];

                double acceleration = G * other.mass / (distance * distance);

                planet.UpdateVelocity(
                    (dx / distance) * acceleration * deltaTime,
                    (dy / distance) * acceleration * deltaTime,
                    (dz / distance) * acceleration * deltaTime, '+');
            }
        }

        for (auto& planet : planets) {
            planet.UpdatePosition(deltaTime);
        }


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
