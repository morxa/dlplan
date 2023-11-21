#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_ROLES_COMPOSE_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_ROLES_COMPOSE_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class ComposeRole : public Role {
private:
    const std::shared_ptr<const Role> m_role_left;
    const std::shared_ptr<const Role> m_role_right;

    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const;

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    ComposeRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right);
    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override;

    size_t hash() const;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::ComposeRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::ComposeRole>& left_role,
            const std::shared_ptr<const dlplan::core::ComposeRole>& right_role) const;
    };

    template<>
    struct hash<dlplan::core::ComposeRole>
    {
        std::size_t operator()(const dlplan::core::ComposeRole& role) const;
    };
}

#endif