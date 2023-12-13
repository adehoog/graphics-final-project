#ifndef SPHERE_H
#define SPHERE_H

#include <glad/glad.h>

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
class Sphere
{
private:

    GLuint customVBO, customVAO, customEBO;  // OpenGL buffer objects
    float customRadius;  // Sphere radius
    int customSectorCount;  // Number of sectors (longitude slices)
    int customStackCount;   // Number of stacks (latitude slices)
    std::vector<float> customVertices;  // Vertex data (position and texture coordinates)
    std::vector<unsigned int> customIndices;  // Index data for element buffer

public:
    ~Sphere()
    {
        glDeleteVertexArrays(1, &customVAO);
        glDeleteBuffers(1, &customVBO);
        glDeleteBuffers(1, &customEBO);
    }

    Sphere(float sphereRadius, int numSectors, int numStacks)
    {
        customRadius = sphereRadius;
        customSectorCount = numSectors;
        customStackCount = numStacks;

        // Generate Vertex Data
        float x, y, z, xy, lengthInv = 1.0f / customRadius, s, t;
        float sectorStep = (float)(2 * M_PI / customSectorCount);
        float stackStep = (float)(M_PI / customStackCount);
        float sectorAngle, stackAngle;

        int i = 0;
        while (i <= customStackCount)
        {
            stackAngle = (float)(M_PI / 2 - i * stackStep);
            xy = 1.02f * customRadius * cosf(stackAngle);
            z = customRadius * sinf(stackAngle);

            int j = 0;
            while (j <= customSectorCount)
            {
                sectorAngle = j * sectorStep;

                x = xy * cosf(sectorAngle);
                y = xy * sinf(sectorAngle);

                // Push vertex position and texture coordinates to the vectors
                customVertices.push_back(x);
                customVertices.push_back(y);
                customVertices.push_back(z);
                s = (float)j / customSectorCount;
                t = (float)i / customStackCount;
                customVertices.push_back(s);
                customVertices.push_back(t);

                j++;
            }

            i++;
        }

        // Generate Index Data
        int k1, k2;
        int iIndex = 0;
        while (iIndex < customStackCount)
        {
            k1 = iIndex * (customSectorCount + 1);
            k2 = k1 + customSectorCount + 1;

            int jIndex = 0;
            while (jIndex < customSectorCount)
            {
                if (iIndex != 0)
                {
                    customIndices.push_back(k1);
                    customIndices.push_back(k2);
                    customIndices.push_back(k1 + 1);
                }

                if (iIndex != (customStackCount - 1))
                {
                    customIndices.push_back(k1 + 1);
                    customIndices.push_back(k2);
                    customIndices.push_back(k2 + 1);
                }

                jIndex++;
                k1++;
                k2++;
            }

            iIndex++;
        }

        // Generate OpenGL Buffers
        glGenVertexArrays(1, &customVAO);
        glGenBuffers(1, &customVBO);
        glGenBuffers(1, &customEBO);

        glBindVertexArray(customVAO);

        glBindBuffer(GL_ARRAY_BUFFER, customVBO);
        glBufferData(GL_ARRAY_BUFFER, (unsigned int)customVertices.size() * sizeof(float), customVertices.data(), GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, customEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (unsigned int)customIndices.size() * sizeof(unsigned int), customIndices.data(), GL_DYNAMIC_DRAW);

        // Set vertex attribute pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Draw()
    {
        glBindVertexArray(customVAO);
        glDrawElements(GL_TRIANGLES, (unsigned int)customIndices.size(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }
};



#endif
