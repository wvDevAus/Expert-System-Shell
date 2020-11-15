#include "FactDatabase.hpp"

#include <tuple>

namespace expert_system::knowledge::facts {

    std::optional<std::reference_wrapper<VariantFact>> FactDatabase::Create(
            const std::string& name, utility::ExpertSystemTypes type) {
        // Catch if the name is already used
        if (stored_facts_.find(name) != stored_facts_.end()) {
            // Stop and indicate failure
            return std::nullopt;
        }

        // Create the Fact
        stored_facts_.insert(std::pair<std::string, VariantFact>(name, type));

        // Attempt to gather a reference to the Fact
        return Find(name);
    }

    std::optional<std::reference_wrapper<VariantFact>> FactDatabase::Find(
            const std::string& name) {
        // Catch an invalid/unused Fact name
        if (stored_facts_.find(name) == stored_facts_.end()) {
            // Stop and indicate failure
            return std::nullopt;
        }

        // Gather a reference to the Fact
        return stored_facts_[name];
    }

    bool FactDatabase::Known(const std::string& name) {
        // Attempt to Find the requested Fact
        auto target_fact = Find(name);
        if (target_fact == std::nullopt) {
            // Catch an invalid Fact and indicate failure
            return false;
        }

        // Check if the Fact has a session value
        switch (target_fact->get().type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Gather the raw data and check if the session value exists
                auto raw_fact = std::get<BoolFact>(target_fact->get().fact_);
                return (raw_fact.GetValue().has_value());
            }
            case utility::ExpertSystemTypes::kInt: {
                // Gather the raw data and check if the session value exists
                auto raw_fact = std::get<IntFact>(target_fact->get().fact_);
                return (raw_fact.GetValue().has_value());
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Gather the raw data and check if the session value exists
                auto raw_fact = std::get<FloatFact>(target_fact->get().fact_);
                return (raw_fact.GetValue().has_value());
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Gather the raw data and check if the session value exists
                auto raw_fact = std::get<EnumFact>(target_fact->get().fact_);
                return (raw_fact.fact_.GetValue().has_value());
            }
            default:
                // Indicate failure
                return false;
        }
    }

    std::set<std::string> FactDatabase::List(FactFilter filter) {
        // Create a temporary list to store the Fact names in
        std::set<std::string> fact_names;

        // Iterate through the database's keys
        for (const auto& map_iterator: stored_facts_) {
            // Split the logic based on the specified filter
            switch (filter) {
                case FactFilter::kAll: {
                    // Append the name to the end of the list
                    fact_names.insert(map_iterator.first);
                    break;
                }
                case FactFilter::kHasValue: {
                    // Check if the Fact has a value
                    if (Known(map_iterator.first)) {
                        // Append the name to the end of the list
                        fact_names.insert(map_iterator.first);
                    }
                    break;
                }
                case FactFilter::kHasNoValue: {
                    // Check if the Fact has no value
                    if (!Known(map_iterator.first)) {
                        // Append the name to the end of the list
                        fact_names.insert(map_iterator.first);
                    }
                    break;
                }
                default : {
                    // Skip this Fact
                    break;
                }
            }
        }

        // Return the list of Fact names
        return fact_names;
    }

    int FactDatabase::Count() {
        // Return the amount of elements in the map
        return (int) stored_facts_.size();
    }

    bool FactDatabase::Remove(const std::string& name) {
        // Catch if the name is currently used
        if (stored_facts_.find(name) == stored_facts_.end()) {
            // Stop and indicate failure
            return false;
        }

        // Delete the Fact with the specified name
        stored_facts_.erase(name);
        return true;
    }

    void FactDatabase::Clear() {
        // Just empty the entire contents of the map
        stored_facts_.clear();
    }

    void FactDatabase::Reset() {
        // Iterate through the map's contents
        for (auto map_iterator: stored_facts_) {
            // Split logic for each Fact type, assume the stored hint is correct
            switch (map_iterator.second.type_) {
                case utility::ExpertSystemTypes::kBool: {
                    // Gather a reference to the Fact and clear its session data
                    std::get<BoolFact>(map_iterator.second.fact_).ClearValue();
                    break;
                }
                case utility::ExpertSystemTypes::kInt: {
                    // Gather a reference to the Fact and clear its session data
                    std::get<IntFact>(map_iterator.second.fact_).ClearValue();
                    break;
                }
                case utility::ExpertSystemTypes::kFloat: {
                    // Gather a reference to the Fact and clear its session data
                    std::get<FloatFact>(map_iterator.second.fact_).ClearValue();
                    break;
                }
                case utility::ExpertSystemTypes::kEnum: {
                    // Gather a reference to the Fact and clear its session data
                    std::get<EnumFact>(map_iterator.second.fact_).fact_.ClearValue();
                    break;
                }
                default:
                    break;
            }
        }
    }

    void FactDatabase::Iterate(const FactIteratingFunction& iterating_function) {
        // Iterate through the map's contents
        for (auto map_iterator: stored_facts_) {
            // Pass the current Fact to the function
            iterating_function(map_iterator.first, map_iterator.second);
        }
    }

    void to_json(nlohmann::json& json_sys, const FactDatabase& target) {
        // Just export the map's contents
        json_sys = target.stored_facts_;
    }

    void from_json(const nlohmann::json& json_sys, FactDatabase& target) {
        // Just import the map's contents;
        target.stored_facts_ = json_sys.get<std::unordered_map<std::string, VariantFact>>();
    }

} // namespace expert_system::knowledge::facts
