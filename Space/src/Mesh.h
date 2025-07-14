#pragma once
#include <vector>
#include <glad/glad.h>

class Mesh {
    unsigned int VAO, VBO, EBO;
    bool useEBO;
    size_t indexCount;
    size_t vertexCount;
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices = {}, int stride = 3)
        : useEBO(!indices.empty()), indexCount(indices.size()), vertexCount(vertices.size() / stride)
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        if (useEBO)
            glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        if (useEBO) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, stride, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    void draw() const {
        glBindVertexArray(VAO);
        if (useEBO) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));
        }
        glBindVertexArray(0);
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        if (useEBO)
            glDeleteBuffers(1, &EBO);
    }
};