#pragma once

// OPENGL SETUP
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 3RD-PARTY LIBRARIES
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imconfig.h>
#include <imgui/imstb_rectpack.h>
#include <imgui/imstb_textedit.h>
#include <imgui/imstb_truetype.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <fmt/format.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <nlohmann/json.hpp>

// STANDARD LIBRARIES
#include <iostream>
#include <string>   
#include <fstream>
#include <sstream>
#include <cmath>
#include <memory>
#include <vector>
#include <unordered_map>
#include <random>
#include <thread>
#include <cassert>
// #include <sys/stat.h>
#include <source_location>
#include <filesystem>
#include <chrono>
#include <queue>
#include <type_traits>
#include <array>

// COMMON HEADERS
#include "src/utils/Debug.h"
#include "src/utils/Assert.h"
#include "src/utils/Terminate.h"
#include "src/utils/Logging.h"
#include "src/utils/utils.h"
#include "src/utils/FileSystem.h"