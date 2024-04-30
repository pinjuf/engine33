#include "mesh.h"

#include <cstring>

Mesh::Mesh() {}

void Mesh::load_wfobj(const char * path) {
    // "Inspired" (copied) from https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

    // TODO: Indexing!

    FILE * file = fopen(path, "r");
    if (!file) {
        std::cerr << "Could not open model file " << path << std::endl;
        return;
    }

    vertices.clear();
    uvs.clear();
    normals.clear();

    std::vector<glm::vec3> v{{0.0f, 0.0f, 0.0f}};
    std::vector<glm::vec2> vt{{0.0f, 0.0f}};
    std::vector<glm::vec3> vn{{0.0f, 0.0f, 0.0f}};

    while (true) {
        char token[128];
        // TODO: Buffer overflow possible here!
        if (fscanf(file, "%s ", token) == EOF)
            break;

        if (!strcmp(token, "v")) { // Coords
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            v.push_back(vertex);
        } else if (!strcmp(token, "vt")) { // UVs
            glm::vec2 texcoord;
            fscanf(file, "%f %f\n", &texcoord.x, &texcoord.y);
            vt.push_back(texcoord);
        } else if (!strcmp(token, "vn")) { // Normals
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            vn.push_back(normal);
        } else if (!strcmp(token, "f")) { // Indices
            // Note how we assume we already have the points referred to by the indices in our temporary v, vt & vn

            unsigned int indices0[3], indices1[3], indices2[3];
            fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", 
                    &indices0[0],
                    &indices0[1],
                    &indices0[2],
                    &indices1[0],
                    &indices1[1],
                    &indices1[2],
                    &indices2[0],
                    &indices2[1],
                    &indices2[2]
            );

            vertices.push_back(v[indices0[0]].x);
            vertices.push_back(v[indices0[0]].y);
            vertices.push_back(v[indices0[0]].z);
            vertices.push_back(v[indices1[0]].x);
            vertices.push_back(v[indices1[0]].y);
            vertices.push_back(v[indices1[0]].z);
            vertices.push_back(v[indices2[0]].x);
            vertices.push_back(v[indices2[0]].y);
            vertices.push_back(v[indices2[0]].z);

            uvs.push_back(vt[indices0[1]].x);
            uvs.push_back(vt[indices0[1]].y);
            uvs.push_back(vt[indices1[1]].x);
            uvs.push_back(vt[indices1[1]].y);
            uvs.push_back(vt[indices2[1]].x);
            uvs.push_back(vt[indices2[1]].y);

            /*
            normals.push_back(vn[indices0[2]].x);
            normals.push_back(vn[indices0[2]].y);
            normals.push_back(vn[indices0[2]].z);
            normals.push_back(vn[indices1[2]].x);
            normals.push_back(vn[indices1[2]].y);
            normals.push_back(vn[indices1[2]].z);
            normals.push_back(vn[indices2[2]].x);
            normals.push_back(vn[indices2[2]].y);
            normals.push_back(vn[indices2[2]].z);
            */
            for (size_t i = 0; i < 9; i++) {
                normals.push_back((rand() % 1000)/1000.0f);
            }
        }
    }
}
