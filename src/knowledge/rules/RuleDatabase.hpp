#pragma once

#include <map>
#include <string>

#include "knowledge/rules/Rule.hpp"

namespace expert_system::knowledge::rules {

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
             * @brief The database's collection of Rules.
             * Maps a Rule to a unique name.
             */
        std::map<std::string, Rule> managed_rules_;
    };

}