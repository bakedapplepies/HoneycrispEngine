- TODO: batch/instance rendering; prioritize instancing
- TODO: ShaderConfig object to pass into draw calls?
- TODO: optimize shader system
- TODO: cache texture (ktx) & store model in reuseable binary format
- TODO: check if shared_ptr in SceneManager is necessary
- TODO: LOD (mesh subdivision via geometry shader for now, mesh shader later)
- TODO: Make model loading asynchronous
- TODO: Mark constructors as explicit
- TODO: Quaternion implementation
- TODO: Separate the program in 2 threads: Rendering and Everything else. The only input the Rendering thread
gets is the Draw-calls-queue.
- TODO: rename protected members with p_ prefixes
- TODO: Render priority
- TODO: What the hell is noexcept
- TODO: Continuous maps
- TODO: Add type-checking for UBOs
- TODO: Texture2D Min/Mag filter (nearest/linear), Wrap (repeat/clamp/...)
- TODO: Look into glNamed... functions
- TODO: Use asynchronous operations to speed up model loading (nodes)
- TODO: replace all smart-pointer arguments to raw pointers -> faster speed
        only use smart pointers to manage the lifetime of objects
- TODO: Entity-Component-System for Renderable objects (SceneRenderObj - Renderable Component - Renderer)
- TODO: Bind light and uniforms and stuff together
- TODO: Add FBX support
- TODO: Make settings/inspector extendable/retractable
- TODO: Should ambient and diffuse have r-g-b components different from each other?
- TODO: Manage texture units (including GL_TEXTURE_CUBE_MAP)
- TODO: Texture2DManager
- TODO: Use Texture2DManager when using stuff that has textures
- TODO: ShaderManager: returns a shared_ptr<Shader>
- TODO: SPIR-V & Shader cache
- TODO: .clangd format feature seems useful
- TODO: Ticking bomb-style error (doesn't crash program, but will crash if you use the erroneous data)
- TODO: Automatic handling of nullptrs that doesn't use weakptrs (somehow)

- TODO: Systems that actually need ECS:
    + Renderer
    + Physics

- TODO: Everything doesn't have to be not static cuz between the Logic & Render thread we could transfer data
over 2 threads all at once, and only the Logic thread needs the static class.