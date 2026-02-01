#pragma once
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define PI 3.14159265358979323846

class Planet {
public:
    float radius;
    double mass;
    int sectorCount = 32;
    int stackCount = 32;

    Planet(glm::vec3 position, float radius, double mass) {
		this->Position = position;
		this->radius = radius;
        this->mass = mass;
		CalculateCoords();
    }

    glm::mat4 model;

    glm::vec3 Velocity{ 0.0, 0.0, 0.0 };

    glm::vec3 Position{ 0.0, 0.0, 0.0 };

    std::vector<float> combinedVertices;

    std::vector<float> vertices;

    std::vector<float> texCoords;

    std::vector<unsigned int> indices;

    std::vector<unsigned int> lineIndices;

    void CalculateCoords() {
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(texCoords);

        float x, y, z, xy;
        float s, t;

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            y = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                z = xy * sinf(sectorAngle);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                combinedVertices.push_back(x);
                combinedVertices.push_back(y);
                combinedVertices.push_back(z);

                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                texCoords.push_back(s);
                texCoords.push_back(t);

                combinedVertices.push_back(s);
                combinedVertices.push_back(t);
            }
        }

        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount + 1);
            k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
            {
                
                if (i != 0)
                {
                    indices.push_back(k1);
                    indices.push_back(k2);
                    indices.push_back(k1 + 1);
                }

                
                if (i != (stackCount - 1))
                {
                    indices.push_back(k1 + 1);
                    indices.push_back(k2);
                    indices.push_back(k2 + 1);
                }

                lineIndices.push_back(k1);
                lineIndices.push_back(k2);
                if (i != 0)
                {
                    lineIndices.push_back(k1);
                    lineIndices.push_back(k1 + 1);
                }
            }
        }
    }

    double CalculateDistance(Planet& other) {
        double distance = sqrt(pow(this->Position[0] - other.Position[0], 2) + pow(this->Position[1] - other.Position[1], 2) + pow(this->Position[2] - other.Position[2], 2));
        return distance;
    }

	void UpdatePosition(float dt) {
		Position[0] += this->Velocity[0] * dt;
        Position[1] += this->Velocity[1] * dt;
		Position[2] += this->Velocity[2] * dt;
	}

    void UpdateVelocity(float velX, float velY, float velZ, char ch) {
        switch (ch)
        {
        case '+':
            Velocity[0] += velX;
			Velocity[1] += velY;
            Velocity[2] += velZ;
			break;
        case '*':
            Velocity[0] *= velX;
            Velocity[1] *= velY;
			Velocity[2] *= velZ;
            break;
        }
       
    }

    double CheckCollision(Planet& other) {
        double distance = CalculateDistance(other);

        if (other.radius + this->radius > distance) {
            return -0.2f;
        }
        return 1.0f;
    }
};