#ifndef PROJECT_HODOGRAPH_SIMULATION_H
#define PROJECT_HODOGRAPH_SIMULATION_H

#include <vr/simulation.h>

#include <math/math_ifx.h>

#include <random>
#include <memory>

namespace ifx{
class GameObject;
class SceneContainer;
}

struct HodographGameObjects{
    std::shared_ptr<ifx::GameObject> circle;
    std::shared_ptr<ifx::GameObject> box;
};

struct Line {
    glm::vec3 position0;
    glm::vec3 position1;

    glm::vec3 last_position1;
    glm::vec3 last_last_position1;

    std::shared_ptr<ifx::GameObject> game_object = nullptr;
};

struct HodographCache{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> velocities;
    std::vector<glm::vec3> accelerations;

    std::vector<float> positions_z;
    std::vector<float> velocities_z;
    std::vector<float> accelerations_z;
};

class HodographSimulation : public ifx::Simulation {
public:

    HodographSimulation(std::shared_ptr<ifx::GameObject> circle,
                        std::shared_ptr<ifx::GameObject> box,
                        std::shared_ptr<ifx::SceneContainer> scene);
    ~HodographSimulation();

    float* angular_velocity(){return &angular_velocity_;}
    float* radius(){return &radius_;}
    float* line_length(){return &line_length_;}
    float* error(){return &error_;}

    float line_error_length(){return line_error_length_;}
    float alpha(){return alpha_;}
    HodographCache& hodograph_cache(){return hodograph_cache_;}

    void Update() override;

    void ResetCache();
private:
    void InitGameObjects();

    void UpdateGameObjects();
    void UpdateCircleGameObject();
    void UpdateBoxGameObject();
    void UpdateLineGameObject();

    void UpdateLine();
    void UpdateErrorLine();
    void UpdateLinePosition();
    void UpdateLinePosition0();
    void UpdateLinePosition1();

    void UpdateAlpha();
    void ClampAlpha();

    void UpdateCache();

    float angular_velocity_;
    float radius_;
    float line_length_;
    float error_;
    float alpha_;

    float line_error_length_;
    std::default_random_engine generator_;

    Line line_;

    HodographCache hodograph_cache_;

    HodographGameObjects game_objects_;

    std::shared_ptr<ifx::SceneContainer> scene_;

    bool is_first_iteration_;
};

#endif //PROJECT_HODOGRAPH_SIMULATION_H
