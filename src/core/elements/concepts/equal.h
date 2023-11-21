#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class EqualConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::EqualConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::EqualConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::EqualConcept* t, const unsigned int version);

}


namespace dlplan::core {
class EqualConcept : public Concept {
private:
    const std::shared_ptr<const Role> m_role_left;
    const std::shared_ptr<const Role> m_role_right;

    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, ConceptDenotation& result) const {
        // find counterexample [(a,b) in R and (a,b) not in S] or [(a,b) not in R and (a,b) in S]
        result.set();
        for (const auto& pair : left_denot.to_vector()) {
            if (!right_denot.contains(pair)) result.erase(pair.first);
        }
        for (const auto& pair : right_denot.to_vector()) {
            if (!left_denot.contains(pair)) result.erase(pair.first);
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role_left->evaluate(state, caches),
            *m_role_right->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
                denotation);
            // register denotation and append it to denotations.
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    EqualConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right)
        : Concept(vocabulary_info, index, role_left->is_static() && role_right->is_static()),
          m_role_left(role_left), m_role_right(role_right) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, EqualConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const EqualConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, EqualConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const EqualConcept&>(other);
            return m_is_static == other_derived.m_is_static
                && m_role_left == other_derived.m_role_left
                && m_role_right == other_derived.m_role_right;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_role_left, m_role_right);
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_equal" << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::EqualConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::EqualConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::EqualConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::EqualConcept(index, vocabulary, role_left, role_right);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::EqualConcept, std::weak_ptr<dlplan::core::EqualConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::EqualConcept, std::weak_ptr<dlplan::core::EqualConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::EqualConcept, std::weak_ptr<dlplan::core::EqualConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::EqualConcept* first = nullptr;
    std::weak_ptr<dlplan::core::EqualConcept>* second = nullptr;
    ar >> const_cast<dlplan::core::EqualConcept&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::core::EqualConcept, std::weak_ptr<dlplan::core::EqualConcept>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::EqualConcept, "dlplan::core::EqualConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::EqualConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::EqualConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::EqualConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::EqualConcept>
    {
        std::size_t operator()(const dlplan::core::EqualConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
