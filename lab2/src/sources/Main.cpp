// Local Headers
#include "Application.hpp"
// Standard Headers
#include <iostream>
#include <memory>
// External Headers

using namespace RA;

int main(int argc, char *argv[])
{
    Application::Instance = std::make_shared<Application>();
    Application::Instance->Run();

    return 0;
}