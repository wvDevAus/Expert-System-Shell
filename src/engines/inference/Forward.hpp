#pragma once

#include <optional>
#include <set>
#include <vector>

#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/rules/RuleDatabase.hpp"
#include "engines/explanation/Log.hpp"

namespace expert_system::engines::inference::forward {

        /**
         * @brief Creates a log for the existing Facts stored in a provided database.
         * @param fact_database The database of Facts to operate on.
         * @return The log of existing Fact session values, credited to the user.
         */
    explanation::Log LogExisting(knowledge::facts::FactDatabase& fact_database);

        /**
         * @brief Creates a log for a specific Fact stored in a provided database.
         * @param fact_name The name of the target Fact.
         * @param fact_database The database of Facts to operate on.
         * @return The log of the target Fact, credited to the user, or std:nullopt on error.
         */
    std::optional<explanation::Log> LogSpecific(
            std::string fact_name,
            knowledge::facts::FactDatabase& fact_database);

        /**
         * @brief Identifies the set of Facts that require a session value to trigger a Rule.
         * @param fact_database The database of Facts to operate on.
         * @param rule_database The database of Rules to operate on.
         * @return The set of the Fact names.
         */
    std::set<std::string> IdentifyFactRequests(
            knowledge::facts::FactDatabase& fact_database,
            knowledge::rules::RuleDatabase& rule_database);

        /**
         * @brief Identifies which Rules have been triggered by the current state of a Fact Database.
         * @param fact_database The database of Facts to operate on.
         * @param rule_database The database of Rules to operate on.
         * @return A set of iterators, each pointing to the Rules within the provided rule database.
         */
    std::list<std::map<std::string, knowledge::rules::Rule>::iterator> FindTriggered(
            knowledge::facts::FactDatabase& fact_database,
            knowledge::rules::RuleDatabase& rule_database);

        /**
         * @brief Runs a set of provided Rules, generating logs for the Assignment action of each.
         * @param triggered_rules The set of Rules to action.
         * @param fact_database The database of Facts to operate on.
         * @return A set of logs, one for each Rule that had successfully run its Assignment action.
         */
    std::vector<explanation::Log> RunTriggered(
            std::list<std::map<std::string, knowledge::rules::Rule>::iterator> triggered_rules,
            knowledge::facts::FactDatabase& fact_database);

} // namespace expert_system::engines::inference::forward
