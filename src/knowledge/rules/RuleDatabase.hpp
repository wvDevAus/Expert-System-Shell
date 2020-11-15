#pragma once

#include <list>
#include <map>
#include <string>

#include "knowledge/rules/Rule.hpp"

namespace expert_system::knowledge::rules {

        /// The symbolic representations for applicable Rule filters.
    enum class RuleFilter {
        kAll,
        kHasRunConsequent,
        kHasNotRunConsequent
    };

        /**
         * @brief A basic database that manages an expert system's Rules.
         * Very basic structure, but is capable of operating all of its stored Rules.
         */
    struct RuleDatabase {
            /// Default constructor
        RuleDatabase() = default;

            /**
             * @brief Calls the Run() method on all stored Rules.
             * @param database The Fact Database to operate on.
             */
        void Run(facts::FactDatabase& database);

            /**
             * @brief Lists the stored Rules, with a specified filter.
             * @param filter The identifier of the filter to apply.
             * @return A list of the iterators to the filtered Rules.
             */
        std::list<std::map<std::string, Rule>::iterator> ListRules(
                RuleFilter filter = RuleFilter::kAll);

            /**
             * @brief Lists all of the Facts that the stored Rules require to trigger their Antecedent.
             * @param filter The identifier of the filter to apply.
             * @return A list of the Fact names from the filtered Rules.
             */
        std::set<std::string> ListRuleAntedecentFacts(
                RuleFilter filter = RuleFilter::kAll);

            /**
             * @brief Lists all of the Facts that the stored Rules would modify from their Consequent.
             * @param filter The identifier of the filter to apply.
             * @return A list of the Fact names from the filtered Rules.
             */
        std::set<std::string> ListRuleConsequentFacts(
                RuleFilter filter = RuleFilter::kAll);

            /// Reverts the "has_run_" flag on all stored Rules.
        void ResetRules();

            /**
             * @brief The database's collection of Rules.
             * Maps a Rule to a unique name.
             */
        std::map<std::string, Rule> managed_rules_;
    };

        /**
         * @brief RuleDatabase serialization to JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the RuleDatabase to export.
         */
    void to_json(nlohmann::json& json_sys, const RuleDatabase& target);

        /**
         * @brief RuleDatabase serialization from JSON format.
         * @param [in] json_sys A reference to a JSON object.
         * @param [in,out] target A reference to the RuleDatabase to import.
         */
    void from_json(const nlohmann::json& json_sys, RuleDatabase& target);

}