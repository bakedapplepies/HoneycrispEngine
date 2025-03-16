# TODO List

- TODO: mipmaps aren't generating (sponza cloths)
- TODO: vcpkg
- TODO: try building dll (HNCRSP_API)
- TODO: ShaderConfig object to pass into draw calls?
- TODO: LOD (tesselation shader, mesh shader; latter is quite hard)
- TODO: Mark constructors as explicit
- TODO: Quaternion implementation
- TODO: Render - Logic Threads
- TODO: Render graph
- TODO: What the hell is noexcept
- TODO: Look into glNamed... functions (DSA)
- TODO: cache texture (ktx)
- TODO: SPIR-V
- TODO: Automatic handling of nullptrs that doesn't use weakptrs (somehow)
- TODO: Relative world positions to parents

- TODO: OpenGL Disable-type functions don't have to be coupled to their private members, maybe
use non-member functions?

- TODO: Anti-aliasing on the framebuffer, resolve using glBlitFramebuffer

- TODO: Asset Manager to manage resource lifetime (like Images) and stuff like that

- TODO: Grayscale textures are stored in texture arrays as GL_RGBA format - very wasteful

- TODO: Z-prepass

- TODO: glMultiDrawElementsIndirect

Object data are currently managed by ECS system.
Because objects' world positions may be changed at any time, the so will the model matrices.
-> Each model will have n meshes (uses the same model matrices)
    Each mesh will have an material ID.
    There are k materials.
-> Uniform array of k materials.
    Each material has maximum of 5 textures
    If there are only 3 materials, there will be a maximum of 15 textures which is A LOT
    Each mesh can have an instanced attribute in the VAO for storing an index to the material array
-> Render a complex model in one draw call (glDrawMultiIndirect)
    Filter out opaque objects and draw later

Also see glBindTextures
