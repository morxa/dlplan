#include "../../../../include/dlplan/core/elements/roles/top.h"


namespace dlplan::core {
RoleDenotation TopRole::evaluate_impl(const State& state, DenotationsCaches&) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    denotation.set();
    return denotation;
}

RoleDenotations TopRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    RoleDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
        denotation.set();
        denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
    }
    return denotations;
}

TopRole::TopRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
    : Role(vocabulary_info, index, true) { }

bool TopRole::operator==(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const TopRole&>(other);
        return m_is_static == other_derived.m_is_static;
    }
    return false;
}

size_t TopRole::hash() const {
    return dlplan::utils::hash_combine(m_is_static);
}

RoleDenotation TopRole::evaluate(const State& state) const {
    auto denotation = RoleDenotation(state.get_instance_info()->get_objects().size());
    denotation.set();
    return denotation;

}

int TopRole::compute_complexity() const {
    return 1;
}

void TopRole::compute_repr(std::stringstream& out) const {
    out << "r_top";
}

int TopRole::compute_evaluate_time_score() const {
    return SCORE_CONSTANT;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::TopRole>>::operator()(
        const std::shared_ptr<const dlplan::core::TopRole>& left_role,
        const std::shared_ptr<const dlplan::core::TopRole>& right_role) const {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::TopRole>::operator()(const dlplan::core::TopRole& role) const {
        return role.hash();
    }
}