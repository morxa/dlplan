#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

#include <sstream>
#include <memory>

#include <boost/serialization/serialization.hpp>

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
class TopRole : public Role {
private:
    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotation.set();
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, TopRole& role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const TopRole* role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, TopRole* role, const unsigned int version);

public:
    TopRole(std::shared_ptr<const VocabularyInfo> vocabulary_info, ElementIndex index)
    : Role(vocabulary_info, index, true) {
    }

    RoleDenotation evaluate(const State& state) const override {
        auto denotation = RoleDenotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;

    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name();
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }

    static std::string get_name() {
        return "r_top";
    }
};

}

#endif
