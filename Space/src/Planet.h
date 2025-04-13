#pragma once
#include <vector>

class Planet {
public:

    std::vector<float> Velocity{ 0.0f, 0.0f };

	std::vector<float> Position{ 390.0f, 460.0f };

    void DrawPlanet(float cx, float cy, float r) {
        glClear(GL_COLOR_BUFFER_BIT);
        glBegin(GL_TRIANGLE_FAN);
        glVertex2d(cx, cy);
        for (int i = 0; i <= 20; i++) {
            float twicePi = 2.0 * 3.142 * (static_cast<float>(i) / 20);
            float x = cx + cos(twicePi) * r;
            float y = cy + sin(twicePi) * r;
            glVertex2d(x, y);
        }
        glEnd();
    }

	void UpdatePosition(float posX, float posY) {
		Position[0] += posX;
        Position[1] += posY;
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
};