#include "til_c.h"

#include "../../generator_data.h"

#include "../../../../include/dlplan/core/elements/roles/primitive.h"


namespace dlplan::generator::rules {
void TilCRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (int i = 1; i < target_complexity - 1; ++i) {
        int j = target_complexity - i - 1 ;
        for (const auto& r : data.m_roles_by_iteration[i]) {
            for (const auto& c : data.m_concepts_by_iteration[j]) {
                auto element = factory.make_til_c_role(r, c);
                auto denotations = element->evaluate(states, caches);
                if (data.m_role_hash_table.insert(denotations).second) {
                    std::get<3>(data.m_generated_features).push_back(element);
                    data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
                    increment_generated();
                }
            }
        }
    }
}


std::string TilCRole::get_name() const {
    return "r_til_c";
}

}
