// Local Headers
#include "Renderable.hpp"

namespace RA
{
    Renderable::Renderable()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    }

    Renderable::~Renderable()
    {
        glDeleteVertexArrays(1, &VAO);
    }
}