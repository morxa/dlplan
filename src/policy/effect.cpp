#include "effect.h"

#include "../core/elements/utils.h"
#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>

using namespace dlplan;


namespace dlplan::policy {

BooleanEffect::BooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BaseEffect(identifier), m_boolean(boolean) { }

int BooleanEffect::compute_evaluate_time_score() const {
    return m_boolean->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> BooleanEffect::get_boolean() const {
    return m_boolean;
}

std::shared_ptr<const NamedNumerical> BooleanEffect::get_numerical() const {
    return nullptr;
}


NumericalEffect::NumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : BaseEffect(identifier), m_numerical(numerical) { }

int NumericalEffect::compute_evaluate_time_score() const {
    return m_numerical->compute_evaluate_time_score();
}

std::shared_ptr<const NamedBoolean> NumericalEffect::get_boolean() const {
    return nullptr;
}

std::shared_ptr<const NamedNumerical> NumericalEffect::get_numerical() const {
    return m_numerical;
}


PositiveBooleanEffect::PositiveBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BooleanEffect(identifier, boolean) {}

bool PositiveBooleanEffect::operator==(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const PositiveBooleanEffect&>(other);
        return m_boolean == other_derived.m_boolean;
    }
    return false;
}

size_t PositiveBooleanEffect::hash() const {
    return dlplan::utils::hash_combine(m_boolean);
}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return m_boolean->get_boolean()->evaluate(target_state);
}

bool PositiveBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_boolean->get_boolean()->evaluate(target_state, caches);
}

std::string PositiveBooleanEffect::compute_repr() const{
    return "(:e_b_pos \"" + m_boolean->get_boolean()->compute_repr() + "\")";
}

std::string PositiveBooleanEffect::str() const {
    return "(:e_b_pos " + m_boolean->get_key() + ")";
}


NegativeBooleanEffect::NegativeBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BooleanEffect(identifier, boolean) {}

bool NegativeBooleanEffect::operator==(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const NegativeBooleanEffect&>(other);
        return m_boolean == other_derived.m_boolean;
    }
    return false;
}

size_t NegativeBooleanEffect::hash() const {
    return dlplan::utils::hash_combine(m_boolean);
}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state) const {
    return !m_boolean->get_boolean()->evaluate(target_state);
}

bool NegativeBooleanEffect::evaluate(const core::State&, const core::State& target_state, core::DenotationsCaches& caches) const {
    return !m_boolean->get_boolean()->evaluate(target_state, caches);
}

std::string NegativeBooleanEffect::compute_repr() const{
    return "(:e_b_neg \"" + m_boolean->get_boolean()->compute_repr() + "\")";
}

std::string NegativeBooleanEffect::str() const {
    return "(:e_b_neg " + m_boolean->get_key() + ")";
}


UnchangedBooleanEffect::UnchangedBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean)
    : BooleanEffect(identifier, boolean) {}

bool UnchangedBooleanEffect::operator==(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const UnchangedBooleanEffect&>(other);
        return m_boolean == other_derived.m_boolean;
    }
    return false;
}

size_t UnchangedBooleanEffect::hash() const {
    return dlplan::utils::hash_combine(m_boolean);
}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_boolean->get_boolean()->evaluate(source_state) == m_boolean->get_boolean()->evaluate(target_state);
}

bool UnchangedBooleanEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    return m_boolean->get_boolean()->evaluate(source_state, caches) == m_boolean->get_boolean()->evaluate(target_state, caches);
}

std::string UnchangedBooleanEffect::compute_repr() const{
    return "(:e_b_bot \"" + m_boolean->get_boolean()->compute_repr() + "\")";
}

std::string UnchangedBooleanEffect::str() const {
    return "(:e_b_bot " + m_boolean->get_key() + ")";
}


IncrementNumericalEffect::IncrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NumericalEffect(identifier, numerical) {}

bool IncrementNumericalEffect::operator==(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const IncrementNumericalEffect&>(other);
        return m_numerical == other_derived.m_numerical;
    }
    return false;
}

size_t IncrementNumericalEffect::hash() const {
    return dlplan::utils::hash_combine(m_numerical);
}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_numerical->get_numerical()->evaluate(source_state);
    int target_eval = m_numerical->get_numerical()->evaluate(target_state);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval < target_eval;
}

bool IncrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_numerical->get_numerical()->evaluate(source_state, caches);
    int target_eval = m_numerical->get_numerical()->evaluate(target_state, caches);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval < target_eval;
}

std::string IncrementNumericalEffect::compute_repr() const{
    return "(:e_n_inc \"" + m_numerical->get_numerical()->compute_repr() + "\")";
}

std::string IncrementNumericalEffect::str() const {
    return "(:e_n_inc " + m_numerical->get_key() + ")";
}


DecrementNumericalEffect::DecrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NumericalEffect(identifier, numerical) {}

bool DecrementNumericalEffect::operator==(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const DecrementNumericalEffect&>(other);
        return m_numerical == other_derived.m_numerical;
    }
    return false;
}

size_t DecrementNumericalEffect::hash() const {
    return dlplan::utils::hash_combine(m_numerical);
}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    int source_eval = m_numerical->get_numerical()->evaluate(source_state);
    int target_eval = m_numerical->get_numerical()->evaluate(target_state);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval > target_eval;
}

bool DecrementNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_numerical->get_numerical()->evaluate(source_state, caches);
    int target_eval = m_numerical->get_numerical()->evaluate(target_state, caches);
    if (source_eval == INF) return false;
    if (target_eval == INF) return false;
    return source_eval > target_eval;
}

std::string DecrementNumericalEffect::compute_repr() const{
    return "(:e_n_dec \"" + m_numerical->get_numerical()->compute_repr() + "\")";
}

std::string DecrementNumericalEffect::str() const {
    return "(:e_n_dec " + m_numerical->get_key() + ")";
}


UnchangedNumericalEffect::UnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical)
    : NumericalEffect(identifier, numerical) {}

bool UnchangedNumericalEffect::operator==(const BaseEffect& other) const {
    if (typeid(*this) == typeid(other)) {
        if (this == &other) return true;
        const auto& other_derived = static_cast<const UnchangedNumericalEffect&>(other);
        return m_numerical == other_derived.m_numerical;
    }
    return false;
}

size_t UnchangedNumericalEffect::hash() const {
    return dlplan::utils::hash_combine(m_numerical);
}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state) const {
    return m_numerical->get_numerical()->evaluate(source_state) == m_numerical->get_numerical()->evaluate(target_state);
}

bool UnchangedNumericalEffect::evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const {
    int source_eval = m_numerical->get_numerical()->evaluate(source_state, caches);
    int target_eval = m_numerical->get_numerical()->evaluate(target_state, caches);
    return source_eval == target_eval;
}

std::string UnchangedNumericalEffect::compute_repr() const{
    return "(:e_n_bot \"" + m_numerical->get_numerical()->compute_repr() + "\")";
}

std::string UnchangedNumericalEffect::str() const {
    return "(:e_n_bot " + m_numerical->get_key() + ")";
}

}

namespace boost::serialization {
template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::BooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BaseEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::BooleanEffect* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::BooleanEffect* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::NumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BaseEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::NumericalEffect* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::NumericalEffect* /* t */ , const unsigned int /* version */ )
{
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::PositiveBooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::PositiveBooleanEffect* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_boolean;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::PositiveBooleanEffect* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedBoolean> boolean;
    ar >> identifier;
    ar >> boolean;
    ::new(t)dlplan::policy::PositiveBooleanEffect(identifier, boolean);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::NegativeBooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::NegativeBooleanEffect* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_boolean;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::NegativeBooleanEffect* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedBoolean> boolean;
    ar >> identifier;
    ar >> boolean;
    ::new(t)dlplan::policy::NegativeBooleanEffect(identifier, boolean);
}

template<typename Archive>
void serialize( Archive& /* ar */ , dlplan::policy::UnchangedBooleanEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::BooleanEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::UnchangedBooleanEffect* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_boolean;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::UnchangedBooleanEffect* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedBoolean> boolean;
    ar >> identifier;
    ar >> boolean;
    ::new(t)dlplan::policy::UnchangedBooleanEffect(identifier, boolean);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::IncrementNumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::IncrementNumericalEffect* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_numerical;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::IncrementNumericalEffect* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedNumerical> numerical;
    ar >> identifier;
    ar >> numerical;
    ::new(t)dlplan::policy::IncrementNumericalEffect(identifier, numerical);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::DecrementNumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::DecrementNumericalEffect* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_numerical;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::DecrementNumericalEffect* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedNumerical> numerical;
    ar >> identifier;
    ar >> numerical;
    ::new(t)dlplan::policy::DecrementNumericalEffect(identifier, numerical);
}

template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::UnchangedNumericalEffect& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::policy::NumericalEffect>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::policy::UnchangedNumericalEffect* t, const unsigned int /* version */ )
{
    ar << t->m_identifier;
    ar << t->m_numerical;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::policy::UnchangedNumericalEffect* t, const unsigned int /* version */ )
{
    int identifier;
    std::shared_ptr<const dlplan::policy::NamedNumerical> numerical;
    ar >> identifier;
    ar >> numerical;
    ::new(t)dlplan::policy::UnchangedNumericalEffect(identifier, numerical);
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>* t, const unsigned int /*version*/) {
    dlplan::policy::PositiveBooleanEffect* first = nullptr;
    std::weak_ptr<dlplan::policy::PositiveBooleanEffect>* second = nullptr;
    ar >> const_cast<dlplan::policy::PositiveBooleanEffect&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>(*first, *second);
    delete first;
    delete second;
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>* t, const unsigned int /*version*/) {
    dlplan::policy::NegativeBooleanEffect* first = nullptr;
    std::weak_ptr<dlplan::policy::NegativeBooleanEffect>* second = nullptr;
    ar >> const_cast<dlplan::policy::NegativeBooleanEffect&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>(*first, *second);
    delete first;
    delete second;
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>* t, const unsigned int /*version*/) {
    dlplan::policy::UnchangedBooleanEffect* first = nullptr;
    std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>* second = nullptr;
    ar >> const_cast<dlplan::policy::UnchangedBooleanEffect&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>(*first, *second);
    delete first;
    delete second;
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>* t, const unsigned int /*version*/) {
    dlplan::policy::IncrementNumericalEffect* first = nullptr;
    std::weak_ptr<dlplan::policy::IncrementNumericalEffect>* second = nullptr;
    ar >> const_cast<dlplan::policy::IncrementNumericalEffect&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>(*first, *second);
    delete first;
    delete second;
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>* t, const unsigned int /*version*/) {
    dlplan::policy::DecrementNumericalEffect* first = nullptr;
    std::weak_ptr<dlplan::policy::DecrementNumericalEffect>* second = nullptr;
    ar >> const_cast<dlplan::policy::DecrementNumericalEffect&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>(*first, *second);
    delete first;
    delete second;
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>* t, const unsigned int /*version*/) {
    dlplan::policy::UnchangedNumericalEffect* first = nullptr;
    std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>* second = nullptr;
    ar >> const_cast<dlplan::policy::UnchangedNumericalEffect&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>(*first, *second);
    delete first;
    delete second;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::BooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::BooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::BooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::BooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::PositiveBooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::PositiveBooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::PositiveBooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::PositiveBooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NegativeBooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NegativeBooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NegativeBooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NegativeBooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedBooleanEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::UnchangedBooleanEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::UnchangedBooleanEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedBooleanEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::NumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::NumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::NumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::NumericalEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::IncrementNumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::IncrementNumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::IncrementNumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::IncrementNumericalEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::DecrementNumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::DecrementNumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::DecrementNumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::DecrementNumericalEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedNumericalEffect& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::UnchangedNumericalEffect& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::policy::UnchangedNumericalEffect* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::policy::UnchangedNumericalEffect* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::PositiveBooleanEffect, std::weak_ptr<dlplan::policy::PositiveBooleanEffect>>* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::NegativeBooleanEffect, std::weak_ptr<dlplan::policy::NegativeBooleanEffect>>* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::UnchangedBooleanEffect, std::weak_ptr<dlplan::policy::UnchangedBooleanEffect>>* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::IncrementNumericalEffect, std::weak_ptr<dlplan::policy::IncrementNumericalEffect>>* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::DecrementNumericalEffect, std::weak_ptr<dlplan::policy::DecrementNumericalEffect>>* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::policy::UnchangedNumericalEffect, std::weak_ptr<dlplan::policy::UnchangedNumericalEffect>>* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::PositiveBooleanEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::NegativeBooleanEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::UnchangedBooleanEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::IncrementNumericalEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::DecrementNumericalEffect)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::policy::UnchangedNumericalEffect)


namespace std {
        bool less<std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }


        bool less<std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        bool less<std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>>::operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& right_effect) const {
        return *left_effect < *right_effect;
    }

        std::size_t hash<dlplan::policy::PositiveBooleanEffect>::operator()(
            const dlplan::policy::PositiveBooleanEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::NegativeBooleanEffect>::operator()(
            const dlplan::policy::NegativeBooleanEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::UnchangedBooleanEffect>::operator()(
            const dlplan::policy::UnchangedBooleanEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::IncrementNumericalEffect>::operator()(
            const dlplan::policy::IncrementNumericalEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::DecrementNumericalEffect>::operator()(
            const dlplan::policy::DecrementNumericalEffect& effect) const {
        return effect.hash();
    }

        std::size_t hash<dlplan::policy::UnchangedNumericalEffect>::operator()(
            const dlplan::policy::UnchangedNumericalEffect& effect) const {
        return effect.hash();
    }
}
