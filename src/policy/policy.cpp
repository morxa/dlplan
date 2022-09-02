#include "../include/dlplan/policy.h"

#include "condition.h"
#include "effect.h"
#include "policy_builder.h"
#include "reader.h"
#include "writer.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {


BaseCondition::BaseCondition(std::shared_ptr<const core::BaseElement> base_feature, int index)
    : m_base_feature(base_feature), m_index(index) { }

BaseCondition::BaseCondition(BaseCondition&& other) = default;

BaseCondition& BaseCondition::operator=(BaseCondition&& other) = default;

BaseCondition::~BaseCondition() = default;

void BaseCondition::set_index(int index) {
    m_index = index;
}

int BaseCondition::get_index() const {
    return m_index;
}

std::shared_ptr<const core::BaseElement> BaseCondition::get_base_feature() const {
    return m_base_feature;
}

std::string BaseCondition::str() const {
    return compute_repr();
}


BaseEffect::BaseEffect(std::shared_ptr<const core::BaseElement> base_feature, int index)
    : m_base_feature(base_feature), m_index(index) { }

BaseEffect::BaseEffect(BaseEffect&& other) = default;

BaseEffect& BaseEffect::operator=(BaseEffect&& other) = default;

BaseEffect::~BaseEffect() = default;

void BaseEffect::set_index(int index) {
    m_index = index;
}

int BaseEffect::get_index() const {
    return m_index;
}

std::shared_ptr<const core::BaseElement> BaseEffect::get_base_feature() const {
    return m_base_feature;
}

std::string BaseEffect::str() const {
    return compute_repr();
}


PolicyBuilder::PolicyBuilder() = default;

PolicyBuilder::PolicyBuilder(const PolicyBuilder& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyBuilder& PolicyBuilder::operator=(const PolicyBuilder& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyBuilder::PolicyBuilder(PolicyBuilder&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyBuilder& PolicyBuilder::operator=(PolicyBuilder&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyBuilder::~PolicyBuilder() = default;

std::shared_ptr<const core::Boolean> PolicyBuilder::add_boolean_feature(core::Boolean boolean) {
    return m_pImpl->add_boolean_feature(boolean);
}

std::shared_ptr<const core::Numerical> PolicyBuilder::add_numerical_feature(core::Numerical numerical) {
    return m_pImpl->add_numerical_feature(numerical);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_pos_condition(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_pos_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_neg_condition(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_neg_condition(b);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_gt_condition(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_gt_condition(n);
}

std::shared_ptr<const BaseCondition> PolicyBuilder::add_eq_condition(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_eq_condition(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_pos_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_pos_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_neg_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_neg_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_bot_effect(std::shared_ptr<const core::Boolean> b) {
    return m_pImpl->add_bot_effect(b);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_inc_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_inc_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_dec_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_dec_effect(n);
}

std::shared_ptr<const BaseEffect> PolicyBuilder::add_bot_effect(std::shared_ptr<const core::Numerical> n) {
    return m_pImpl->add_bot_effect(n);
}

std::shared_ptr<const Rule> PolicyBuilder::add_rule(
    std::vector<std::shared_ptr<const BaseCondition>>&& conditions,
    std::vector<std::shared_ptr<const BaseEffect>>&& effects) {
    return m_pImpl->add_rule(std::move(conditions), std::move(effects));
}

Policy PolicyBuilder::get_result() {
    return m_pImpl->get_result();
}


PolicyMinimizer::PolicyMinimizer() { }

PolicyMinimizer::PolicyMinimizer(const PolicyMinimizer& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(const PolicyMinimizer& other) = default;

PolicyMinimizer::PolicyMinimizer(PolicyMinimizer&& other) = default;

PolicyMinimizer& PolicyMinimizer::operator=(PolicyMinimizer&& other) = default;

PolicyMinimizer::~PolicyMinimizer() { }


std::unordered_set<std::shared_ptr<const Rule>> PolicyMinimizer::try_merge_by_condition(const Policy& policy, PolicyBuilder& builder) const {
    for (const auto& rule_1 : policy.get_rules()) {
        auto rule_1_conditions = std::unordered_set<std::shared_ptr<const BaseCondition>>(rule_1->get_conditions().begin(), rule_1->get_conditions().end());
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            auto rule_2_conditions = std::unordered_set<std::shared_ptr<const BaseCondition>>(rule_2->get_conditions().begin(), rule_2->get_conditions().end());
            if (rule_1->get_effects() != rule_2->get_effects()) {
                continue;
            }
            std::unordered_set<std::shared_ptr<const BaseCondition>> diff;
            // TODO: make function from here
            std::set_symmetric_difference(rule_1_conditions.begin(), rule_1_conditions.end(), rule_2_conditions.begin(), rule_2_conditions.end(), std::inserter(diff, diff.begin()));
            if (diff.size() != 2) {
                continue;
            }
            if (!check_feature_index_equality(diff)) {
                continue;
            }
            if (!check_type_equality(diff)) {
                continue;
            }
            // TODO: make function till here
            // rule_1 and rule_2 can be merged
            std::vector<std::shared_ptr<const BaseCondition>> conditions = compute_merged_values(rule_1->get_conditions(), diff, builder);
            std::vector<std::shared_ptr<const BaseEffect>> effects = compute_merged_values(rule_1->get_effects(), {}, builder);
            // add new rule and return old rules.
            builder.add_rule(std::move(conditions), std::move(effects));
            return std::unordered_set<std::shared_ptr<const Rule>>({rule_1, rule_2});
        }
    }
    return std::unordered_set<std::shared_ptr<const Rule>>();
}

std::unordered_set<std::shared_ptr<const Rule>> PolicyMinimizer::try_merge_by_effect(const Policy& policy, PolicyBuilder& builder) const {
    for (const auto& rule_1 : policy.get_rules()) {
        auto rule_1_effects = std::unordered_set<std::shared_ptr<const BaseEffect>>(rule_1->get_effects().begin(), rule_1->get_effects().end());
        for (const auto& rule_2 : policy.get_rules()) {
            if (rule_1->get_index() >= rule_2->get_index()) {
                continue;
            }
            auto rule_2_effects = std::unordered_set<std::shared_ptr<const BaseEffect>>(rule_2->get_effects().begin(), rule_2->get_effects().end());
            if (rule_1->get_conditions() != rule_2->get_conditions()) {
                continue;
            }
            std::unordered_set<std::shared_ptr<const BaseEffect>> diff_1_2;
            std::set_symmetric_difference(rule_1_effects.begin(), rule_1_effects.end(), rule_2_effects.begin(), rule_2_effects.end(), std::inserter(diff_1_2, diff_1_2.begin()));
            if (diff_1_2.size() != 2) {
                continue;
            }
            if (!check_feature_index_equality(diff_1_2)) {
                continue;
            }
            if (!check_type_equality(diff_1_2)) {
                continue;
            }
            for (const auto& rule_3 : policy.get_rules()) {
                if (rule_2->get_index() >= rule_3->get_index()) {
                    continue;
                }
                auto rule_3_effects = std::unordered_set<std::shared_ptr<const BaseEffect>>(rule_3->get_effects().begin(), rule_3->get_effects().end());
                if (rule_2->get_conditions() != rule_3->get_conditions()) {
                    continue;
                }
                std::unordered_set<std::shared_ptr<const BaseEffect>> diff_1_2_3;
                std::set_symmetric_difference(diff_1_2.begin(), diff_1_2.end(), rule_3_effects.begin(), rule_3_effects.end(), std::inserter(diff_1_2_3, diff_1_2_3.begin()));
                if (diff_1_2_3.size() != 3) {
                    continue;
                }
                if (!check_feature_index_equality(diff_1_2_3)) {
                    continue;
                }
                if (!check_type_equality(diff_1_2_3)) {
                    continue;
                }
                // rule_1 and rule_2 can be merged
                std::vector<std::shared_ptr<const BaseCondition>> conditions = compute_merged_values(rule_1->get_conditions(), {}, builder);
                std::vector<std::shared_ptr<const BaseEffect>> effects = compute_merged_values(rule_1->get_effects(), diff_1_2_3, builder);
                // add new rule and return old rules.
                builder.add_rule(std::move(conditions), std::move(effects));
                return std::unordered_set<std::shared_ptr<const Rule>>({rule_1, rule_2, rule_3});
            }
        }
    }
    return std::unordered_set<std::shared_ptr<const Rule>>();
}

Policy PolicyMinimizer::minimize_greedy(const Policy& policy) const {
    Policy current_policy = policy;
    std::unordered_set<std::shared_ptr<const Rule>> merged_rules;
    do {
        PolicyBuilder builder;
        merged_rules = try_merge_by_condition(current_policy, builder);
        if (merged_rules.empty()) {
            merged_rules = try_merge_by_effect(current_policy, builder);
        }
        for (const auto& rule : current_policy.get_rules()) {
            if (merged_rules.count(rule)) {
                continue;
            }
            rule->visit(builder);
        }
        current_policy = builder.get_result();
    } while (!merged_rules.empty());

    return current_policy;
}

Policy PolicyMinimizer::minimize_greedy(const Policy& policy, const core::StatePairs& true_state_pairs, const core::StatePairs& false_state_pairs) const {
    /*
       Idea: A rule C -> E dominates a rules C' -> E' if C subseteq C', E subseteq E'
             and substituting C' -> E' with C -> E in policy leads to consistent classification.
    */
    /* 1. try to remove conditions such that classification remains.
    */
    /* 2. try to remove effects such that classification remains.
    */
    throw std::runtime_error("Not implemented.");
}


PolicyReader::PolicyReader() = default;

PolicyReader::PolicyReader(const PolicyReader& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyReader& PolicyReader::operator=(const PolicyReader& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyReader::PolicyReader(PolicyReader&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyReader& PolicyReader::operator=(PolicyReader&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyReader::~PolicyReader() = default;

Policy PolicyReader::read(const std::string& data, core::SyntacticElementFactory& factory) const {
    return m_pImpl->read(data, factory);
}


PolicyWriter::PolicyWriter() { }

PolicyWriter::PolicyWriter(const PolicyWriter& other)
    : m_pImpl(*other.m_pImpl) { }

PolicyWriter& PolicyWriter::operator=(const PolicyWriter& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

PolicyWriter::PolicyWriter(PolicyWriter&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

PolicyWriter& PolicyWriter::operator=(PolicyWriter&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

PolicyWriter::~PolicyWriter() { }

std::string PolicyWriter::write(const Policy& policy) const {
    return m_pImpl->write(policy);
}

}
