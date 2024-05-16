#include "autoload.h"
#include "main.h"

#include <cstring>

AutoLoader::AutoLoader() {}

AutoLoader::~AutoLoader() {}

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
            std::cout << "Defining new mesh" << std::endl;

            char id[256];
            fscanf(file, "%255s\n", id);

            current_mesh = new Mesh();

            objects.insert(std::make_pair(std::string(id), current_mesh));

            std::cout << objects["plane"] << std::endl;
            for(auto [k, v] : objects) {
                std::cout<<k<<std::endl;
            }

            current_mesh->init_glbufs();
        } else if (!strcmp(token, "wfobj")) {
            std::cout << "Loading object file" << std::endl;

            char objfile[256];
            fscanf(file, "%255s\n", objfile);

            current_mesh->load_wfobj(objfile);

            if (current_mesh->shader) // This updates both the GL buffers themselves as well as the vertex attributes (in the shader)
                current_mesh->update_mesh_bufs();

        } else if (!strcmp(token, "shader")) {
            std::cout << "Loading shader" << std::endl;

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
            std::cout << "Loading texture" << std::endl;

            int n;
            char texpath[256];
            fscanf(file, "%d %255s\n", &n, texpath);

            current_mesh->load_texture(n, texpath);
        } else if (!strcmp(token, "parent")) {
            std::cout << "Setting parent" << std::endl;

            char parentid[256];
            fscanf(file, "%255s\n", parentid);

            current_mesh->p.parent = &(objects[parentid]->p);
        } else {
            char ch;
            fscanf(file, "%[^\n]", &ch);
        }
    }
}

void AutoLoader::render() {
    for (auto it : objects) {
        it.second->render();
    }
}
