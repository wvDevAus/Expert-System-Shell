#pragma once

#include <optional>
#include <string>
#include <utility>

#include "nlohmann/json.hpp"

#include "knowledge/facts/FactDatabase.hpp"
#include "knowledge/facts/Facts.hpp"
#include "knowledge/facts/Value.hpp"
#include "utility/Confidence.hpp"

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

        /// The symbolic representations for Condition Test outcomes.
    enum class TestOutcome {
        kUnknown,
        kInvalidCondition,
        kInvalidFactIdentifier,
        kInvalidFactType,
        kUnknownFactValue,
        kInsufficientConfidence,
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
                                {TestOutcome::kInsufficientConfidence, "InsufficientConfidence"},
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
        Condition() : condition_(ConditionType::kUnknown), invert_(false) {};

            /**
             * @brief Parameterized constructor.
             * @param [in] fact The identifying name of the target Fact.
             * @param [in] condition The conditional check to perform when testing the Condition.
             * @param [in] target The target value for the Condition's test.
             * @param [in] invert A flag to indicate if the Condition's Test outcomes should be inverted.
             * @param [in] confidence_factor The Condition's confidence factor.
             */
        Condition(std::string fact, ConditionType condition, T target,
                  bool invert, float confidence_factor)
                : fact_(std::move(fact)), condition_(condition), target_(target),
                  invert_(invert), confidence_factor_(confidence_factor) {};

            /**
             * @brief Tests the current Condition.
             * @param [in] source The Fact Database to operate on.
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

            /**
             * @brief A flag to indicate if the Condition's Test outcomes should be inverted.
             * True to invert the Test results, False to not invert the Test results.
             */
        bool invert_;

            /// The Condition's minimum required confidence factor to trigger.
        utility::Confidence confidence_factor_;
    };

        /// Boolean specialization overload of templated Condition::Test.
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
        if (find_result->get().type_ == utility::ExpertSystemTypes::kBool) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactType;
        }

        // Gather the raw Fact's Value
        auto& raw_fact = std::get<facts::BoolFact>(find_result->get().fact_);
        auto fact_value = raw_fact.GetValue();
        if (fact_value == std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kUnknownFactValue;
        }

        // Catch if the Fact's Value does not have a sufficient confidence factor
        if (fact_value.value().confidence_factor_ < confidence_factor_) {
            // Catch and indicate failure
            return TestOutcome::kInsufficientConfidence;
        }

        // Test the Antecedent
        switch(condition_) {
            case ConditionType::kEqualTo: {
                // Perform the test and return the result
                if (fact_value->value_ == target_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kLessThan: {
                // Perform the test and return the result
                if (fact_value->value_ < target_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kGreaterThan: {
                // Perform the test and return the result
                if (target_ < fact_value->value_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            default:
                // Catch and indicate failure
                return TestOutcome::kInvalidCondition;
        }
    }

        /// Integer specialization overload of templated Antecedent::Test.
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
        if (find_result->get().type_ == utility::ExpertSystemTypes::kInt) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactType;
        }

        // Gather the raw Fact's Value
        auto& raw_fact = std::get<facts::IntFact>(find_result->get().fact_);
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
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kLessThan: {
                // Perform the test and return the result
                if (fact_value->value_ < target_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kGreaterThan: {
                // Perform the test and return the result
                if (target_ < fact_value->value_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            default:
                // Catch and indicate failure
                return TestOutcome::kInvalidCondition;
        }
    }

        /// Float specialization overload of templated Antecedent::Test.
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
        if (find_result->get().type_ == utility::ExpertSystemTypes::kFloat) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactType;
        }

        // Gather the raw Fact's Value
        auto& raw_fact = std::get<facts::FloatFact>(find_result->get().fact_);
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
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kLessThan: {
                // Perform the test and return the result
                if (fact_value->value_ < target_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kGreaterThan: {
                // Perform the test and return the result
                if (target_ < fact_value->value_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            default:
                // Catch and indicate failure
                return TestOutcome::kInvalidCondition;
        }
    }

    template<>
    TestOutcome Condition<std::string>::Test(facts::FactDatabase& source) {
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
        if (find_result->get().type_ == utility::ExpertSystemTypes::kEnum) {
            // Catch and indicate failure
            return TestOutcome::kInvalidFactType;
        }

        // Gather the raw Fact's Value
        auto& raw_fact = std::get<facts::EnumFact>(find_result->get().fact_);
        auto fact_value = raw_fact.fact_.GetValue();
        if (fact_value == std::nullopt) {
            // Catch and indicate failure
            return TestOutcome::kUnknownFactValue;
        }

        // Test the Antecedent
        auto& fact_enum = raw_fact.enum_;
        switch(condition_) {
            case ConditionType::kEqualTo: {
                // Perform the test and return the result
                if (fact_enum.At(fact_value->value_) == target_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kLessThan: {
                // Perform the test and return the result
                if (fact_enum.At(fact_value->value_) < target_) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            case ConditionType::kGreaterThan: {
                // Perform the test and return the result
                if (target_ < fact_enum.At(fact_value->value_)) {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    } else {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    }
                } else {
                    // Catch if the result should be inverted
                    if (invert_) {
                        // Indicate successful test, return test result
                        return TestOutcome::kComparisonSuccess;
                    } else {
                        // Indicate failed test, return test result
                        return TestOutcome::kComparisonFailure;
                    }
                }
            }
            default:
                // Catch and indicate failure
                return TestOutcome::kInvalidCondition;
        }
    }

} // namespace expert_system::knowledge::rules
