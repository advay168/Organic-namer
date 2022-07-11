#pragma once
#ifdef __cplusplus
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtx/transform.hpp>

#include "imgui.h"

#include "Constants.h"

template<typename T>
inline void DISPLAY_helper(const std::string& varName, T var)
{
  std::stringstream ss;
  ss << var;
  ImGui::Text((varName + ": %s").c_str(), ss.str().c_str());
}

template<glm::length_t num>
inline void DISPLAY_helper(const std::string& varName,
                           const glm::vec<num, float>& var)
{
  auto tmp(var);
  ImGui::DragScalarN(varName.c_str(), ImGuiDataType_Float, &tmp.x, num);
}

#define DISPLAY(var) DISPLAY_helper(#var, var)

#endif
