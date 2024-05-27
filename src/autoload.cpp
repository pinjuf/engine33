#include "autoload.h"
#include "main.h"

#include <cstring>
#include <fstream>

AutoLoader::AutoLoader() {}

AutoLoader::~AutoLoader() {
    for (auto [id, mesh] : objects) {
        if (mesh->shader)
            delete mesh->shader;
        delete mesh;
    }
}

void AutoLoader::load_loadfile(const char * path) {
    std::ifstream file(path, std::ios::in);
    if (!file) {
        std::cerr << "Could not open load file " << path << std::endl;
        return;
    }

    Mesh * current_mesh = NULL;

    while (!file.eof()) {
        std::string token;
        file >> token;

        if (token == "mesh") {
            std::string id;
            file >> id;

            current_mesh = new Mesh();
            objects[id] = current_mesh;

            current_mesh->init_glbufs();
        } else if (token == "wfobj") {
            std::string objfile;
            file >> objfile;

            current_mesh->load_wfobj(objfile.c_str());

            if (current_mesh->shader) // This updates both the GL buffers themselves as well as the vertex attributes (in the shader)
                current_mesh->update_mesh_bufs();

        } else if (token == "shader") {
            std::string vert, frag; 
            file >> vert >> frag;

            ShaderProgram * shaderprog = new ShaderProgram(
                shadermanager.link_program(
                    shadermanager.load_shader(GL_VERTEX_SHADER, vert.c_str()),
                    shadermanager.load_shader(GL_FRAGMENT_SHADER, frag.c_str())
                )
            );

            current_mesh->shader = shaderprog;

        } else if (token == "texture") {
            std::string n;
            std::string texpath;
            file >> n >> texpath;

            current_mesh->load_texture(std::stoi(n), texpath.c_str());
        } else if (token == "parent") {
            std::string parentid;
            file >> parentid;

            current_mesh->p.parent = &(objects[parentid]->p);
        } else if (token == "potition") {
            std::string x, y, z;
            file >> x >> y >> z;

            current_mesh->p.position = glm::vec3(std::stof(x), std::stof(y), std::stof(z));
        } else if (token == "meshoffset") { // Assumes a mesh is already loaded!
            std::string dx, dy, dz;
            file >> dx >> dy >> dz;

            for (size_t i = 0; i < current_mesh->vertices.size(); i += 3) {
                current_mesh->vertices[i + 0] += std::stof(dx);
                current_mesh->vertices[i + 1] += std::stof(dy);
                current_mesh->vertices[i + 2] += std::stof(dz);
            }

            if (current_mesh->shader)
                current_mesh->update_mesh_bufs();
        }
    }
}

void AutoLoader::render() {
    for (auto [id, mesh]: objects) {
        mesh->render();
    }
}
