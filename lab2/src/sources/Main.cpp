// Local
#include "Application.hpp"
#include "Assets.hpp"
// Standard
#include <iostream>
#include <memory>
// External

using namespace RA;

int main(int argc, char *argv[])
{
    Application::Initialize();
    Assets::Load();

    Application::Run();
}