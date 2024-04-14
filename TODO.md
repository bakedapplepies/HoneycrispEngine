- TODO: batch/instance rendering; prioritize instancing
- TODO: ShaderConfig object to pass into draw calls?
- TODO: cache texture (ktx) & store model in reuseable binary format
- TODO: LOD (tesselation shader, mesh shader; latter is quite hard)
- TODO: Mark constructors as explicit
- TODO: Quaternion implementation
- TODO: Separate the program in 2 threads: Rendering and Everything else. The only input the Rendering thread
gets is the Draw-calls-queue.
- TODO: rename protected members with p_ prefixes
- TODO: Render priority
- TODO: What the hell is noexcept
- TODO: Texture2D Min/Mag filter (nearest/linear), Wrap (repeat/clamp/...)
- TODO: Look into glNamed... functions
- TODO: Bind light and uniforms and stuff together
- TODO: Make settings/inspector extendable/retractable
- TODO: Should ambient and diffuse have r-g-b components different from each other?
- TODO: Manage texture units (including GL_TEXTURE_CUBE_MAP)
- TODO: SPIR-V
- TODO: .clangd format feature seems useful
- TODO: Ticking bomb-style error (doesn't crash program, but will crash if you use the erroneous data)
- TODO: Automatic handling of nullptrs that doesn't use weakptrs (somehow)
- TODO: Relative world positions to parents

- TODO: Everything doesn't have to be not static cuz between the Logic & Render thread we could transfer data
over 2 threads all at once, and only the Logic thread needs the static class.