#include "FactDatabase.hpp"

#include <fstream>
#include <tuple>

namespace expert_system {

    std::optional<std::reference_wrapper<VariantFact>> FactDatabase::Create(
            const std::string& name, FactType type) {
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

    std::list<std::string> FactDatabase::List() const {
        // Create a temporary list to store the Fact names in
        std::list<std::string> fact_names;

        // Iterate through the database's keys
        for (const auto& map_iterator: stored_facts_) {
            // Append the name to the end of the list
            fact_names.push_back(map_iterator.first);
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
        if (stored_facts_.find(name) != stored_facts_.end()) {
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
                case FactType::kBoolFact: {
                    // Gather a reference to the Fact and clear its session data
                    std::get<BoolFact>(map_iterator.second.fact_).ClearValue();
                    break;
                }
                case FactType::kIntFact: {
                    // Gather a reference to the Fact and clear its session data
                    std::get<IntFact>(map_iterator.second.fact_).ClearValue();
                    break;
                }
                case FactType::kFloatFact: {
                    // Gather a reference to the Fact and clear its session data
                    std::get<FloatFact>(map_iterator.second.fact_).ClearValue();
                    break;
                }
                case FactType::kEnumFact: {
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

} // expert_system
