#include "Renderer.h"

#include <cassert>

HNCRSP_NAMESPACE_START

bool Renderer::m_rendererSetup = false;

Renderer::Renderer()
{
    assert(!m_rendererSetup && "A renderer has already existed.");
    m_rendererSetup = true;
}

Renderer::~Renderer()
{
    m_rendererSetup = false;
}

HNCRSP_NAMESPACE_END