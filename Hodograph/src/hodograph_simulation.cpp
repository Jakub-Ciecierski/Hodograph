#include "hodograph_simulation.h"

#include <object/game_object.h>
#include <game/scene_container.h>
#include <graphics/factory/render_object_factory.h>

HodographSimulation::HodographSimulation(
        std::shared_ptr<ifx::GameObject> circle,
        std::shared_ptr<ifx::GameObject> box,
        std::shared_ptr<ifx::SceneContainer> scene) :
        angular_velocity_(1),
        radius_(1),
        line_length_(radius_*3),
        line_error_length_(line_length_),
        error_(0),
        alpha_(0),
        scene_(scene),
        is_first_iteration_(true){
    game_objects_.circle = circle;
    game_objects_.box = box;

    InitGameObjects();
}

HodographSimulation::~HodographSimulation(){}

void HodographSimulation::Update(){
    if(!UpdateTime())
            return;

    UpdateLine();
    UpdateGameObjects();
    UpdateCache();

    is_first_iteration_ = false;
}

void HodographSimulation::ResetCache(){
    hodograph_cache_.positions.clear();
    hodograph_cache_.velocities.clear();
    hodograph_cache_.accelerations.clear();

    hodograph_cache_.positions_z.clear();
    hodograph_cache_.velocities_z.clear();
    hodograph_cache_.accelerations_z.clear();
}

void HodographSimulation::InitGameObjects(){
    const float scale_factor = 0.2f;

    game_objects_.box->rotateTo(glm::vec3(0, 90, 0));
    game_objects_.box->scale(scale_factor);

    game_objects_.circle->moveTo(glm::vec3(-(radius_ + 0.1f), 0, 0));
}

void HodographSimulation::UpdateGameObjects(){
    UpdateCircleGameObject();
    UpdateBoxGameObject();
    UpdateLineGameObject();
}

void HodographSimulation::UpdateCircleGameObject(){
    game_objects_.circle->scale(glm::vec3(1, radius_, radius_));
}

void HodographSimulation::UpdateBoxGameObject(){
    game_objects_.box->moveTo(line_.position1);
    const float a = 1;
    const float scale_factor = 0.2f;

    game_objects_.box->move(glm::vec3(0, -a*scale_factor/2.0f, a*scale_factor));
}

void HodographSimulation::UpdateLineGameObject(){
    if(line_.game_object)
        scene_->Remove(line_.game_object);
    auto render_object = ifx::RenderObjectFactory().CreateLine(line_.position0,
                                                               line_.position1);
    line_.game_object = std::shared_ptr<ifx::GameObject>(new ifx::GameObject());
    line_.game_object->Add(render_object);
    scene_->Add(line_.game_object);
}

void HodographSimulation::UpdateLine(){
    UpdateErrorLine();
    UpdateLinePosition();
}

void HodographSimulation::UpdateErrorLine(){
    std::normal_distribution<float> distribution(0, error_);
    float error_t = distribution(generator_);

    line_error_length_ = line_length_ + error_t;
}

void HodographSimulation::UpdateLinePosition(){
    UpdateLinePosition0();
    UpdateLinePosition1();

    UpdateAlpha();
}

void HodographSimulation::UpdateLinePosition0(){
    const float x = -0.01;
    float z0 = radius_ * sin(alpha_);
    float y0 = radius_ * cos(alpha_);
    line_.position0 = glm::vec3(x,y0,z0);
}

void HodographSimulation::UpdateLinePosition1(){
    const float x = -0.01;

    float a_sqr = line_error_length_* line_error_length_;
    float b_sqr = line_.position0.y * line_.position0.y;
    float lx1 = sqrt(a_sqr - b_sqr);
    float z1 = line_.position0.z + lx1;

    if(is_first_iteration_){
        line_.position1 = glm::vec3(x, 0, z1);
        line_.last_last_position1 = line_.position1;
        line_.last_position1 = line_.position1;
    }else{
        line_.last_last_position1 = line_.last_position1;
        line_.last_position1 = line_.position1;
        line_.position1 = glm::vec3(x, 0, z1);
    }

}

void HodographSimulation::UpdateAlpha(){
    alpha_ += angular_velocity_ * time_data_.time_delta;
    ClampAlpha();
}

void HodographSimulation::ClampAlpha(){
    if(alpha_ >= 2*M_PI)
        alpha_ = 0;
}

void HodographSimulation::UpdateCache(){
    auto& current = line_.position1;
    auto& last = line_.last_position1;
    auto& last_last = line_.last_last_position1;

    float time_delta_sqr = time_data_.time_delta * time_data_.time_delta;
    auto velocity = (current - last_last) / (2.0f * time_data_.time_delta);
    auto acceleration = (current - (last*2.0f) + last_last)
                          / (time_delta_sqr);

    hodograph_cache_.positions.push_back(line_.position1);
    hodograph_cache_.velocities.push_back(velocity);
    hodograph_cache_.accelerations.push_back(acceleration);

    hodograph_cache_.positions_z.push_back(line_.position1.z);
    hodograph_cache_.velocities_z.push_back(velocity.z);
    hodograph_cache_.accelerations_z.push_back(acceleration.z);
}

