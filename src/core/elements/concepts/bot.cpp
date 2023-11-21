#include "../../../../include/dlplan/core/elements/concepts/bot.h"


namespace dlplan::core {
ConceptDenotation BotConcept::evaluate_impl(const State& state, DenotationsCaches&) const {
    return ConceptDenotation(state.get_instance_info()->get_objects().size());
}

ConceptDenotations BotConcept::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    ConceptDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
        denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
    }
    return denotations;
}

BotConcept::BotConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
: Concept(vocabulary_info, index, true) { }

bool BotConcept::operator==(const Concept& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const BotConcept&>(other);
        return m_is_static == other_derived.m_is_static;
    }
    return false;
}

size_t BotConcept::hash() const {
    return dlplan::utils::hash_combine(m_is_static);
}

ConceptDenotation BotConcept::evaluate(const State& state) const {
    return ConceptDenotation(state.get_instance_info()->get_objects().size());
}

int BotConcept::compute_complexity() const {
    return 1;
}

void BotConcept::compute_repr(std::stringstream& out) const {
    out << "c_bot";
}

int BotConcept::compute_evaluate_time_score() const {
    return SCORE_CONSTANT;
}

}


namespace std {
    bool less<std::shared_ptr<const dlplan::core::BotConcept>>::operator()(
        const std::shared_ptr<const dlplan::core::BotConcept>& left_concept,
        const std::shared_ptr<const dlplan::core::BotConcept>& right_concept) const {
        return *left_concept < *right_concept;
    }

    std::size_t hash<dlplan::core::BotConcept>::operator()(const dlplan::core::BotConcept& concept_) const {
        return concept_.hash();
    }
}
