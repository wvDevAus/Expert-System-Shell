#pragma once

#include <optional>
#include <string>

#include "nlohmann/json.hpp"

#include "knowledge/facts/Database.hpp"
#include "knowledge/facts/Facts.hpp"
#include "knowledge/facts/Value.hpp"

namespace expert_system::knowledge::rules {

        /// The symbolic representations of the compatible Antecedent conditions.
    enum class ConditionType {
        kUnknown,
        kEqualTo,
        kLessThan,
        kGreaterThan
    };

        /// JSON serialization for the ConditionType enum.
    NLOHMANN_JSON_SERIALIZE_ENUM(ConditionType,
                                {{ConditionType::kUnknown, nullptr},
                                 {ConditionType::kEqualTo, "EqualTo"},
                                 {ConditionType::kLessThan, "GreaterThan"},
                                 {ConditionType::kGreaterThan, "LessThan"}})

    enum class TestOutcome {
        kUnknown,
        kInvalidCondition,
        kInvalidFactIdentifier,
        kInvalidFactType,
        kUnknownFactValue,
        kComparisonSuccess,
        kComparisonFailure
    };

        /// JSON serialization for the TestOutcome enum.
    NLOHMANN_JSON_SERIALIZE_ENUM(TestOutcome,
                               {{TestOutcome::kUnknown, nullptr},
                                {TestOutcome::kInvalidCondition, "InvalidCondition"},
                                {TestOutcome::kInvalidFactIdentifier, "InvalidFactIdentifier"},
                                {TestOutcome::kInvalidFactType, "InvalidFactType"},
                                {TestOutcome::kUnknownFactValue, "UnknownFactValue"},
                                {TestOutcome::kComparisonSuccess, "ComparisonSuccess"},
                                {TestOutcome::kComparisonFailure, "ComparisonFailed"}})

        /**
         * @brief Template Condition.
         * @tparam T The raw type used in the Condition.
         */
    template<class T>
    struct Condition {
            /**
             * @brief Default constructor.
             * Assigns an 'empty' target name and sets the condition to kUnknown.
             */
        Condition() : target_(""), condition_(AntecedentType::kUnknown) {};

            /**
             * @brief Parameterized constructor.
             * @param [in] fact The identifying name of the target Fact.
             * @param [in] condition The conditional check to perform when testing the Condition.
             * @param [in] target The target value for the Condition's test.
             */
        Condition(std::string fact, ConditionType condition, T target)
                : fact_(fact), condition_(condition), target_(target) {};

            /**
             * @brief Tests the current Condition.
             * @param source The Fact Database to operate on.
             * @return A TestOutcome enum symbol indicating the test's result.
             */
        TestOutcome Test(facts::FactDatabase& source) {
            // Not compatible with non-specialized Fact types
            return TestOutcome::kUnknown;
        };

            /// The identifying name of the target Fact.
        std::string fact_;

            /// The symbolic representation of the Condition's operation to perform.
        ConditionType condition_;

            /// The target value for the Condition's test.
        T target_;
    };

        /// Boolean specialization of templated Condition::Test.
    template<>
    TestOutcome Condition<bool>::Test(facts::FactDatabase& source) {
        // Check that the specified condition is valid
        if (condition_ == ConditionType::kUnknown) {
            // Catch and indicate failure
            return TestOutcome::kInvalidCondition;
        }

        // Check the specified Fact exists
        auto find_result = source.Find(fact_);
        if (find_result != std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactIdentifier;
        }

        // Check that the specified Fact does not have a mismatching type
        if (find_result->get().type_ == facts::FactType::kBoolFact) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactType;
        }

        // Gather the raw Fact's Value
        auto raw_fact = std::get<facts::BoolFact>(find_result->get().fact_);
        auto fact_value = raw_fact.GetValue();
        if (fact_value == std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kUnknownFactValue;
        }

        // Test the Antecedent
        switch(condition_) {
            case ConditionType::kEqualTo: {
                // Perform the test and return the result
                if (fact_value->value_ == target_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            case ConditionType::kLessThan: {
                // Perform the test and return the result
                if (fact_value->value_ < target_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            case ConditionType::kGreaterThan: {
                // Perform the test and return the result
                if (target_ < fact_value->value_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            default:
                // Catch and indicate failure
                return TestOutcome::kInvalidCondition;
        }
    }

        /// Integer specialization of templated Antecedent::Test.
    template<>
    TestOutcome Condition<int>::Test(facts::FactDatabase& source) {
        // Check that the specified condition is valid
        if (condition_ == ConditionType::kUnknown) {
            // Catch and indicate failure
            return TestOutcome::kInvalidCondition;
        }

        // Check the specified Fact exists
        auto find_result = source.Find(fact_);
        if (find_result != std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactIdentifier;
        }

        // Check that the specified Fact does not have a mismatching type
        if (find_result->get().type_ == facts::FactType::kIntFact) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactType;
        }

        // Gather the raw Fact's Value
        auto raw_fact = std::get<facts::IntFact>(find_result->get().fact_);
        auto fact_value = raw_fact.GetValue();
        if (fact_value == std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kUnknownFactValue;
        }

        // Test the Antecedent
        switch(condition_) {
            case ConditionType::kEqualTo: {
                // Perform the test and return the result
                if (fact_value->value_ == target_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            case ConditionType::kLessThan: {
                // Perform the test and return the result
                if (fact_value->value_ < target_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            case ConditionType::kGreaterThan: {
                // Perform the test and return the result
                if (target_ < fact_value->value_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            default:
                // Catch and indicate failure
                return TestOutcome::kInvalidCondition;
        }
    }

        /// Float specialization of templated Antecedent::Test.
    template<>
    TestOutcome Condition<float>::Test(facts::FactDatabase& source) {
        // Check that the specified condition is valid
        if (condition_ == ConditionType::kUnknown) {
            // Catch and indicate failure
            return TestOutcome::kInvalidCondition;
        }

        // Check the specified Fact exists
        auto find_result = source.Find(fact_);
        if (find_result != std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactIdentifier;
        }

        // Check that the specified Fact does not have a mismatching type
        if (find_result->get().type_ == facts::FactType::kFloatFact) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactType;
        }

        // Gather the raw Fact's Value
        auto raw_fact = std::get<facts::FloatFact>(find_result->get().fact_);
        auto fact_value = raw_fact.GetValue();
        if (fact_value == std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kUnknownFactValue;
        }

        // Test the Antecedent
        switch(condition_) {
            case ConditionType::kEqualTo: {
                // Perform the test and return the result
                if (fact_value->value_ == target_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            case ConditionType::kLessThan: {
                // Perform the test and return the result
                if (fact_value->value_ < target_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            case ConditionType::kGreaterThan: {
                // Perform the test and return the result
                if (target_ < fact_value->value_) {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonSuccess;
                } else {
                    // Indicate successful test, return test result
                    return TestOutcome::kComparisonFailure;
                }
            }
            default:
                // Catch and indicate failure
                return TestOutcome::kInvalidCondition;
        }
    }

} // namespace expert_system::knowledge::rules
