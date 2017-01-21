#ifndef PROJECT_HODOGRAPH_FACTORY_H
#define PROJECT_HODOGRAPH_FACTORY_H

#include <memory>

namespace ifx{
class GameObject;
}

class HodographFactory {
public:

    HodographFactory();
    ~HodographFactory();

    std::shared_ptr<ifx::GameObject> CreateCircle();
    std::shared_ptr<ifx::GameObject> CreateBox();
};


#endif //PROJECT_HODOGRAPH_FACTORY_H
