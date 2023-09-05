- better object hierarchy abstraction
- more efficient use of VAOs and shaders and textures:
    + make bind checks
- runtime vertex attribute change (GL_DYNAMIC_DRAW)
- batch rendering (check if object is STATIC, if it moves then batch rendering is impossible)
- instance rendering
- push all draw calls to renderer and sort by shader