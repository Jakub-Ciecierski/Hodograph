#include "factory/hodograph_factory.h"

#include <object/render_object.h>
#include <object/game_object.h>
#include <graphics/factory/model_factory.h>
#include <graphics/factory/program_factory.h>
#include <graphics/factory/texture_factory.h>

HodographFactory::HodographFactory(){}
HodographFactory::~HodographFactory(){}

std::shared_ptr<ifx::GameObject> HodographFactory::CreateCircle(){
    auto material = std::shared_ptr<ifx::Material>(new ifx::Material());
    glm::vec3 color(255, 0 ,0);
    material->diffuse = ifx::TextureFactory().CreateSolidColorTexture(
            color, ifx::TextureTypes::DIFFUSE);
    material->specular = ifx::TextureFactory().CreateSolidColorTexture(
            color, ifx::TextureTypes::SPECULAR);

    auto model = ifx::ModelFactory::LoadCircle(1);
    model->getMesh(0)->material(material);
    auto program = ifx::ProgramFactory().LoadMainProgram();

    auto renderObject
            = std::shared_ptr<ifx::RenderObject>(
                    new ifx::RenderObject(ObjectID(0),model));
    renderObject->addProgram(program);

    auto game_object = std::shared_ptr<ifx::GameObject>(new ifx::GameObject());
    game_object->Add(renderObject);

    return game_object;
}

std::shared_ptr<ifx::GameObject> HodographFactory::CreateBox(){
    auto material = std::shared_ptr<ifx::Material>(new ifx::Material());
    glm::vec3 color(0, 255 ,0);
    material->diffuse = ifx::TextureFactory().CreateSolidColorTexture(
            color, ifx::TextureTypes::DIFFUSE);
    material->specular = ifx::TextureFactory().CreateSolidColorTexture(
            color, ifx::TextureTypes::SPECULAR);

    auto model = ifx::ModelFactory::CreateQuad(1,1);
    model->getMesh(0)->material(material);
    auto program = ifx::ProgramFactory().LoadMainProgram();

    auto renderObject
            = std::shared_ptr<ifx::RenderObject>(
                    new ifx::RenderObject(ObjectID(0),model));
    renderObject->addProgram(program);

    auto game_object = std::shared_ptr<ifx::GameObject>(new ifx::GameObject());
    game_object->Add(renderObject);

    return game_object;
}