#include "identity.h"

#include "../../generator_data.h"

#include "../../../core/elements/roles/identity.h"


namespace dlplan::generator::rules {
void IdentityRole::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    core::SyntacticElementFactory& factory = data.m_factory;
    for (const auto& c : data.m_concepts_by_iteration[target_complexity-1]) {
        auto element = factory.make_identity_role(c);
        auto denotations = element.get_element()->evaluate(states, caches);
        if (data.m_role_hash_table.insert(denotations).second) {
            data.m_reprs.push_back(element.compute_repr());
            data.m_roles_by_iteration[target_complexity].push_back(std::move(element));
            increment_generated();
        }
    }
}

std::string IdentityRole::get_name() const {
    return core::IdentityRole::get_name();
}

}
