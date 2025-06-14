#pragma once
#include <vector>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#define PI 3.14159265358979323846

class Planet {
public:
    float radius;
    float mass;
    int sectorCount = 20;
    int stackCount = 20;

    Planet(std::vector<float> velocity, std::vector<float> position, float radius, float mass) {
		this->Velocity = velocity;
		this->Position = position;
		this->radius = radius;
        this->mass = mass;
    }

    std::vector<float> Velocity{ 0.0, 0.0 };

	std::vector<float> Position{ 0.0, 0.0 };

    std::vector<float> vertices;

    std::vector<float> interleavedVertices;

    std::vector<float> normals;

    std::vector<float> texCoords;

    std::vector<unsigned int> indices;

    std::vector<unsigned int> lineIndices;

    void CalculateCoords() {
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(normals);
        std::vector<float>().swap(texCoords);

        float x, y, z, xy;
        float nx, ny, nz, lengthInv = 1.0f / radius;
        float s, t;

        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = PI / 2 - i * stackStep;
            xy = radius * cosf(stackAngle);
            z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j)
            {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);
                vertices.push_back(x);
                vertices.push_back(y);
                vertices.push_back(z);

                nx = x * lengthInv;
                ny = y * lengthInv;
                nz = z * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                texCoords.push_back(s);
                texCoords.push_back(t);
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

	void UpdatePosition() {
		Position[0] += this->Velocity[0];
        Position[1] += this->Velocity[1];
	}

    void UpdateVelocity(float velX, float velY, char ch) {
        switch (ch)
        {
        case '+':
            Velocity[0] += velX;
			Velocity[1] += velY;
			break;
        case '*':
            Velocity[0] *= velX;
            Velocity[1] *= velY;
        }
       
    }

	float CheckCollision(Planet& other) {
		if (this == &other) return 1.0f;

        float distance = sqrt(pow(Position[0] - other.Position[0], 2) + (Position[1] - other.Position[1], 2));
		if (distance < radius + other.radius) {
			return -0.2f;
		}
		return 1.0f;
	}
};