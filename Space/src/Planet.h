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

    Planet(std::vector<double> velocity, std::vector<double> position, float radius, float mass) {
		this->Velocity = velocity;
		this->Position = position;
		this->radius = radius;
        this->mass = mass;
    }

    std::vector<double> Velocity{ 0.0, 0.0 };

	std::vector<double> Position{ 0.0, 0.0 };

    void DrawPlanet() {
		glBegin(GL_TRIANGLES);
        std::vector<float> vertices;
        std::vector<int> indices;
        int sectorCount = 36;
        int stackCount = 18;
        float radius = 1.0f;

        for (int i = 0; i <= stackCount; ++i) {
            float stackAngle = PI / 2 - i * PI / stackCount;
            float xy = radius * cosf(stackAngle);
            float z = radius * sinf(stackAngle);

            for (int j = 0; j <= sectorCount; ++j) {
                float sectorAngle = j * 2 * PI / sectorCount;
                float x = xy * cosf(sectorAngle);
                float y = xy * sinf(sectorAngle);
                vertices.insert(vertices.end(), { x, y, z });
            }
        }

        for (int i = 0; i < stackCount; ++i) {
            int k1 = i * (sectorCount + 1);
            int k2 = k1 + sectorCount + 1;

            for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
                if (i != 0) {
                    indices.insert(indices.end(), { k1, k2, k1 + 1 });
                }
                if (i != (stackCount - 1)) {
                    indices.insert(indices.end(), { k1 + 1, k2, k2 + 1 });
                }
            }
        }
		glEnd();
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