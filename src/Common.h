#ifdef __cplusplus
#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/transform.hpp>

#ifdef EMSCRIPTEN
#include<emscripten/emscripten.h>
#define GLFW_INCLUDE_ES3
#endif
#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include "Constants.h"
#include "WindowData.h"
#endif
