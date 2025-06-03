#pragma once
#include <vector>

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
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(Position[0], Position[1]);
        for (int i = 0; i <= 20; i++) {
            float twicePi = 2.0 * 3.142 * (static_cast<float>(i) / 20);
            float x = Position[0] + cos(twicePi) * radius;
            float y = Position[1] + sin(twicePi) * radius;
            glVertex2d(x, y);
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