#pragma once

#include <string>
#include <map>

#include "mesh.h"

class AutoLoader {
    public:
        AutoLoader();
        ~AutoLoader();

        std::map<std::string, Mesh *> objects;

        void load_loadfile(const char * path);
        void render();
};
