#include "example_gui.h"

#include <engine_gui/factory/engine_gui_factory.h>
#include <engine_gui/engine_gui.h>
#include <gui/imgui/imgui.h>
#include <physics/bullet_extensions/btFractureDynamicsWorld.h>
#include <physics/simulations/bullet_physics_simulation.h>
#include <hodograph_simulation.h>

ExampleGUI::ExampleGUI(GLFWwindow* window,
                       std::shared_ptr<ifx::SceneContainer> scene,
                       std::shared_ptr<HodographSimulation>
                       hodograph_simulation,
                       std::shared_ptr<ifx::PhysicsSimulation> physics_simulation) :
        ifx::GUI(window),
        hodograph_simulation_(hodograph_simulation){
    engine_gui_ = ifx::EngineGUIFactory().CreateEngineGUI(scene,
                                                          physics_simulation);
}
ExampleGUI::~ExampleGUI(){}

void ExampleGUI::Render(){
    NewFrame();

    engine_gui_->Render();

    ImGui::Begin("Hodograph");
    RenderHodographWindow();
    ImGui::End();

    ImGui::Render();
}

void ExampleGUI::RenderHodographWindow(){
    if(ImGui::TreeNode("Simulation")){
        RenderSimulationInfo();
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Properties")){
        RenderProperties();
        ImGui::TreePop();
    }
    if(ImGui::TreeNode("Graphs")){
        RenderGraphs();
        ImGui::TreePop();
    }

}

void ExampleGUI::RenderSimulationInfo(){
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("Time: %.2f [s]", hodograph_simulation_->time_data().total_time);

    if (ImGui::Button("Reset")) {
        hodograph_simulation_->SetRunning(true);
        hodograph_simulation_->Reset();
    }
    ImGui::SameLine();

    std::string play_button_text;
    if (hodograph_simulation_->IsRunning())
        play_button_text = "Pause";
    else
        play_button_text = "Play";

    if (ImGui::Button(play_button_text.c_str())) {
        hodograph_simulation_->SetRunning(!hodograph_simulation_->IsRunning());
    }
}

void ExampleGUI::RenderProperties(){
    ImGui::SliderFloat("Angular Velocity",
                       hodograph_simulation_->angular_velocity(), 0, 10);
    ImGui::SliderFloat("Radius",
                       hodograph_simulation_->radius(), 0.1, 10);
    ImGui::SliderFloat("Line Length",
                       hodograph_simulation_->line_length(), 0.1, 10);
    ImGui::SliderFloat("Error",
                       hodograph_simulation_->error(), 0, 0.1);

    static float line_error_length = hodograph_simulation_->line_error_length();
    static float alpha = hodograph_simulation_->alpha();
    line_error_length = hodograph_simulation_->line_error_length();
    alpha = hodograph_simulation_->alpha();
    ImGui::InputFloat("Line Length with Error", &line_error_length);
    ImGui::InputFloat("Alpha", &alpha);
}

void ExampleGUI::RenderGraphs(){
    RenderPositionGraphs();
}

void ExampleGUI::RenderPositionGraphs(){
    std::vector<float>& positions
            = hodograph_simulation_->hodograph_cache().positions_z;
    std::vector<float>& velocities
            = hodograph_simulation_->hodograph_cache().velocities_z;
    std::vector<float>& accelerations
            = hodograph_simulation_->hodograph_cache().accelerations_z;
    if (ImGui::Button("Reset")) {
        hodograph_simulation_->ResetCache();
    }

    ImGui::PlotLines("Position",
                     positions.data(),
                     positions.size(),
                     0,
                     "x",
                     FLT_MAX, FLT_MAX, ImVec2(0,80));

    ImGui::PlotLines("Velocity",
                     velocities.data(),
                     velocities.size(),
                     0,
                     "v",
                     FLT_MAX, FLT_MAX, ImVec2(0,80));

    ImGui::PlotLines("Acceleration",
                     accelerations.data(),
                     accelerations.size(),
                     0,
                     "a",
                     FLT_MAX, FLT_MAX, ImVec2(0,80));
}
