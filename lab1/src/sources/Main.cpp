// Local Headers
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Application.hpp"
// Standard Headers
#include <iostream>
#include <memory>
#include <string>
// External Headers

using namespace RA;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "[ERROR]: You need to provide 2 arguments: <object_file.obj> <curve_file.crv>\n";
        return 1;
    }

    std::shared_ptr<Application> app = std::make_shared<Application>(argv[1], argv[2]);
    Application::Instance = app;

    Application::Instance->Run();

    return 0;
}