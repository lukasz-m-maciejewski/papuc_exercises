#include "CollisionDetector.hpp"

namespace lmg03
{
void CollisionDetector::compute_collisions(std::vector<PhysicalEntity>& entities)
{
    for (auto& e1 : entities)
    {
        for (auto& e2 : entities)
        {
            if (collistion_occured_between(e1, e2))
            {
                recompute_positions(e1, e2);
            }
        }
    }
}

bool CollisionDetector::collistion_occured_between(const PhysicalEntity& e1, const PhysicalEntity& e2)
{
    const auto curr_1 = e1.get_state();
    const auto curr_2 = e2.get_state();
    const auto next_1 = e1.get_next_state();
    const auto next_2 = e2.get_next_state();

    const auto displacement_1 = next_1.position - curr_1.position;
    const auto displacement_2 = next_2.position - curr_2.position;
}
}
