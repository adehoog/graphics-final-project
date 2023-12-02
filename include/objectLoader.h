#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <glm/glm.hpp>
#include <cstdlib>
#include <ctime>


bool loadObject(
    const char *path,
    std::vector<glm::vec3> &out_vertices,
    std::vector<glm::vec3>& out_normals)
{
    printf("Loading OBJ file %s...\n", path);

    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector< glm::vec3 > temp_normals;


    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        printf("Impossible to open the file ! Are you in the right path ? See Tutorial 1 for details\n");
        getchar();
        return false;
    }

    while (1)
    {

        char lineHeader[1024];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // else : parse lineHeader

        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            int c = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            // std::cout<< " Herer11 "<< c <<"\n";
            temp_vertices.push_back(vertex);
        }
        else if (strcmp(lineHeader, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        else if (strcmp(lineHeader, "f") == 0)
        {
            
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
            //std::cout<< " Herer "<< matches <<"\n";
            if (matches != 6)
            {
                printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                fclose(file);
                return false;
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else
        {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }
  
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    // For each vertex of each triangle
    glm::vec3 color;
    float colx, coly, colz;
    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        // unsigned int uvIndex = uvIndices[i];
        //unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        // std::cout<< " reached index " << i <<" vertex index = "<< vertexIndex<<"\n";
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];
        // glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        // glm::vec3 normal = temp_normals[ normalIndex-1 ];
        

        // Generate a random float between 0 and 1

        if(i%6 == 0)
        {
        colx = static_cast<float>(std::rand()) / RAND_MAX;
        coly = static_cast<float>(std::rand()) / RAND_MAX;
        coly = static_cast<float>(std::rand()) / RAND_MAX;
        //std::cout << i <<" " << colx <<"\n";
        }
        
        color.x = colx, color.y = coly, color.z = colz;
        // color.x = static_cast<float>(std::rand()) / RAND_MAX;
        // color.y = static_cast<float>(std::rand()) / RAND_MAX;
        // color.z = static_cast<float>(std::rand()) / RAND_MAX;

        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_vertices.push_back(color);
        // out_uvs.push_back(uv);
        // out_normals .push_back(normal);
    }
    for (unsigned int i = 0; i < normalIndices.size(); i++) {
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[normalIndex - 1];
        out_normals.push_back(normal);
    }
 
    fclose(file);
    return true;
}
