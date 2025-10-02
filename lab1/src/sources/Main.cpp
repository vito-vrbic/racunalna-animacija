// Local Headers
#include "Mesh.hpp"
#include "Shader.hpp"
// Standard Headers
#include <iostream>
#include <memory>
#include <string>
// External Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace RA;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "[ERROR]: You need to provide 2 arguments: <object_file.obj> <curve_file.crv>\n";
        return 1;
    }

    // Load the mesh from the first argument (OBJ file)
    auto m = Mesh::LoadMesh(argv[1]);

    // Load the B-Spline curve from the second argument (CRV file)

    //

    return 0;
}