#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_TOP_H_

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
class TopConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::TopConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::TopConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::TopConcept* t, const unsigned int version);

}


namespace dlplan::core {
class TopConcept : public Concept {
private:
    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotation.set();
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    TopConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
        : Concept(vocabulary_info, index, true) {
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, TopConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const TopConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, TopConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const TopConcept&>(other);
            return m_is_static == other_derived.m_is_static;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static);
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_top";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::TopConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::TopConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::TopConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    ar >> vocabulary;
    ar >> index;
    ::new(t)dlplan::core::TopConcept(index, vocabulary);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::TopConcept, std::weak_ptr<dlplan::core::TopConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::TopConcept, std::weak_ptr<dlplan::core::TopConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::TopConcept, std::weak_ptr<dlplan::core::TopConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::TopConcept* first = nullptr;
    std::weak_ptr<dlplan::core::TopConcept>* second = nullptr;
    ar >> const_cast<dlplan::core::TopConcept&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::core::TopConcept, std::weak_ptr<dlplan::core::TopConcept>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::TopConcept, "dlplan::core::TopConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::TopConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::TopConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::TopConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::TopConcept>
    {
        std::size_t operator()(const dlplan::core::TopConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
