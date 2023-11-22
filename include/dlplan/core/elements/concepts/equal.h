#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_EQUAL_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_EQUAL_H_

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
class EqualConcept : public Concept {
private:
    const std::shared_ptr<const Role> m_role_left;
    const std::shared_ptr<const Role> m_role_right;

    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, ConceptDenotation& result) const;

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    EqualConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override;

    size_t hash_impl() const override;

    ConceptDenotation evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void compute_repr_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::EqualConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::EqualConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::EqualConcept>& right_concept) const;
    };

    template<>
    struct hash<dlplan::core::EqualConcept>
    {
        std::size_t operator()(const dlplan::core::EqualConcept& concept_) const;
    };
}

#endif
