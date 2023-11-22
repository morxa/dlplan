#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

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
class OneOfConcept : public Concept {
private:
    const Constant m_constant;

    void compute_result(const State& state, ConceptDenotation& result) const;

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    OneOfConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Constant& constant);

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
    struct less<std::shared_ptr<const dlplan::core::OneOfConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::OneOfConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::OneOfConcept>& right_concept) const;
    };

    template<>
    struct hash<dlplan::core::OneOfConcept>
    {
        std::size_t operator()(const dlplan::core::OneOfConcept& concept_) const;
    };
}

#endif
