#pragma once
#include "PhysicalEntity.hpp"

namespace lmg03
{
class CollistionDetector
{
public:
    void compute_collisions(const std::vector<PhysicalEntity>& entities);
private:
    bool collistion_occured_between(const PhysicalEntity&, const PhysicalEntity&);
};

}
