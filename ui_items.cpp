#include "ui_items.h"

void drawGenerationWindowUI(float *rotationVal, terrainInstance* terInst, terrainModel* terModel)
{
    static int seedUI = 1441212;
    ImGui::SetNextWindowPos(ImVec2{ 0.0,0.0 }, 0, ImVec2{ 0.0,0.0 });
    //ImGui::ShowDemoWindow(&demoWindow);
    ImGui::Begin("Generation options");
    
    //std::string drawingTime = "drawing time:" + std::to_string(deltaTime);
    //ImGui::Text(drawingTime.c_str());
    //ImGui::Checkbox("Demo Window", &demoWindow);
    ImGui::SliderFloat("Slider demo", rotationVal, 0.0, 1.0);
    bool t = ImGui::Button("Generate new", ImVec2{ 60.0, 30.0 });
    if (t)
    {
        //std::function<float(float, float, float, int, int, int, int)> func1 = stb_perlin_noise3_seed;
        //std::function<float(float, float, float, float, float, float, float)> func2 = stb_perlin_ridge_noise3;
       // terInst->generateYCoords(seedUI, 0.9, 0.4, seedFunction, genFunction);
        terInst->genNormales();
        terModel->update();
    }
    ImGui::End();
}