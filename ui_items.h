#pragma once
#ifndef UI_ITEMS_H
#define UI_ITEMS_H
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <functional>

#include "terrainGenerator.h"
#include "terrainModel.h"

/*#ifndef STB_PERLIN_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION
#include "stb_perlin.h"
#endif */

//std::function<float(float, float, float, int, int, int, int)> seedFunction = stb_perlin_noise3_seed;
//std::function<float(float, float, float, float, float, float, float)> genFunction = stb_perlin_ridge_noise3;
void drawGenerationWindowUI(float *rotationVal, terrainInstance *terInst, terrainModel *terModel);

#endif // !UI_ITEMS_H
