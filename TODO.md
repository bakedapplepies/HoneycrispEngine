- better object hierarchy abstraction
- more efficient use of VAOs and shaders and textures:
    + make bind checks
- include shader program use function in draw call?
- don't recompile library files
- runtime vertex attribute change (GL_DYNAMIC_DRAW)
- group render calls by shaders
- batch rendering
- stop reseting unique pointers for objects
- when creating textures, add them to a static vector or sth to easily automatically delete them
    + maybe use glDeleteTextures(numTexture, textureVector.data());
- static VAO in each object type