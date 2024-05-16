#include "autoload.h"
#include "main.h"

#include <cstring>

AutoLoader::AutoLoader() {}

AutoLoader::~AutoLoader() {
    for (auto [id, mesh] : objects) {
        if (mesh->shader)
            delete mesh->shader;
        delete mesh;
    }
}

void AutoLoader::load_loadfile(const char * path) {
    FILE * file = fopen(path, "r");
    if (!file) {
        std::cerr << "Could not open load file " << path << std::endl;
        return;
    }

    Mesh * current_mesh = NULL;

    // TODO: This is C++, no place for fscanf!
    while (true) {
        char token[256];

        if (fscanf(file, "%255s ", token) == EOF)
            break;

        if (!strcmp(token, "mesh")) {
            char id[256];
            fscanf(file, "%255s\n", id);

            current_mesh = new Mesh();

            objects.insert(std::make_pair(std::string(id), current_mesh));

            current_mesh->init_glbufs();
        } else if (!strcmp(token, "wfobj")) {
            char objfile[256];
            fscanf(file, "%255s\n", objfile);

            current_mesh->load_wfobj(objfile);

            if (current_mesh->shader) // This updates both the GL buffers themselves as well as the vertex attributes (in the shader)
                current_mesh->update_mesh_bufs();

        } else if (!strcmp(token, "shader")) {
            char vert[256], frag[256]; 
            fscanf(file, "%255s %255s\n", vert, frag);

            ShaderProgram * shaderprog = new ShaderProgram(
                shadermanager.link_program(
                    shadermanager.load_shader(GL_VERTEX_SHADER, vert),
                    shadermanager.load_shader(GL_FRAGMENT_SHADER, frag)
                )
            );

            current_mesh->shader = shaderprog;

        } else if (!strcmp(token, "texture")) {
            int n;
            char texpath[256];
            fscanf(file, "%d %255s\n", &n, texpath);

            current_mesh->load_texture(n, texpath);
        } else if (!strcmp(token, "parent")) {
            char parentid[256];
            fscanf(file, "%255s\n", parentid);

            current_mesh->p.parent = &(objects[parentid]->p);
        } else if (!strcmp(token, "position")) {
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z);

            current_mesh->p.position = glm::vec3(x, y, z);
        } else if (!strcmp(token, "meshoffset")) { // Assumes a mesh is already loaded!
            float dx, dy, dz;
            fscanf(file, "%f %f %f\n", &dx, &dy, &dz);

            for (size_t i = 0; i < current_mesh->vertices.size(); i += 3) {
                current_mesh->vertices[i + 0] += dx;
                current_mesh->vertices[i + 1] += dy;
                current_mesh->vertices[i + 2] += dz;
            }

            if (current_mesh->shader)
                current_mesh->update_mesh_bufs();

        } else {
            char ch;
            fscanf(file, "%[^\n]", &ch);
        }
    }
}

void AutoLoader::render() {
    for (auto [id, mesh]: objects) {
        mesh->render();
    }
}
