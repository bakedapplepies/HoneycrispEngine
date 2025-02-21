# TODO List

- TODO: engine-private library functions are called freely from scenes (prevent this)
- TODO: manage publicity of api functions
- TODO: mipmaps aren't generating (sponza cloths)
- TODO: vcpkg
- TODO: try building dll (HNCRSP_API)
- TODO: batch/instance rendering; prioritize instancing
- TODO: ShaderConfig object to pass into draw calls?
- TODO: LOD (tesselation shader, mesh shader; latter is quite hard)
- TODO: Mark constructors as explicit
- TODO: Quaternion implementation
- TODO: Render - Logic Threads
- TODO: Render graph
- TODO: What the hell is noexcept
- TODO: Look into glNamed... functions
- TODO: cache texture (ktx)
- TODO: SPIR-V
- TODO: Automatic handling of nullptrs that doesn't use weakptrs (somehow)
- TODO: Relative world positions to parents

- TODO: After we're done with models and Renderer architecture, we'll move on to lighting stuff and integrate
that with shaders in scene to process light automatically and easier.

- TODO: OpenGL Disable-type functions don't have to be coupled to their private members, maybe
use non-member functions?

- TODO: Anti-aliasing on the framebuffer, resolve using glBlitFramebuffer

- TODO: Asset Manager to manage resource lifetime (like Images) and stuff like that

- TODO: Rename interfaces with the prefix I-

- TODO: Grayscale textures are stored in texture arrays as GL_RGBA format - very wasteful

- TODO: Z-prepass
