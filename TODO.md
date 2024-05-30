# TODO List

- TODO: batch/instance rendering; prioritize instancing
- TODO: ShaderConfig object to pass into draw calls?
- TODO: LOD (tesselation shader, mesh shader; latter is quite hard)
- TODO: Mark constructors as explicit
- TODO: Quaternion implementation
- TODO: Render - Logic Threads
- TODO: Render graph
- TODO: What the hell is noexcept
- TODO: Look into glNamed... functions
- TODO: Bind light and uniforms and stuff together
- TODO: Make settings/inspector retractable width-wise
- TODO: cache texture (ktx)
- TODO: SPIR-V
- TODO: .clangd format feature seems useful
- TODO: Ticking bomb-style error (doesn't crash program, but will crash if you use the erroneous data)
- TODO: Automatic handling of nullptrs that doesn't use weakptrs (somehow)
- TODO: Relative world positions to parents

- TODO: Minimize the number of glViewport calls

- TODO: After we're done with models and Renderer architecture, we'll move on to lighting stuff and integrate
that with shaders in scene to process light automatically and easier.

- TODO: Assertions of manager state during related methods execution

- TODO: OpenGL Disable-type functions don't have to be coupled to their private members, maybe
use non-member functions?

- TODO: Anti-aliasing on the framebuffer, resolve using glBlitFramebuffer

- TODO: Make it so that each CMakeLists.txt sublevel is easier to edit altogether

- TODO: Sort draw objects by distance to camera for blending

- TODO: Asset Manager to manage resource lifetime (like Images) and stuff like that

- TODO: Maybe make Object class non-copyable and non-movable

- TODO: Rename interfaces with the prefix I-

- TODO: Grayscale textures are stored in texture arrays as GL_RGBA format; may be wasteful????
