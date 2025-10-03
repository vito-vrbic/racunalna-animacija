// Local Headers
#include "Application.hpp"
// Standard Headers
#include <iostream>
#include <memory>
// External Headers

using namespace RA;
using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        cout << "[ERROR]: You need to provide 2 arguments: <object_file.obj> <curve_file.crv>\n";
        return 1;
    }

    Application::Instance = make_shared<Application>(argv[1], argv[2]);
    Application::Instance->Run();

    return 0;
}