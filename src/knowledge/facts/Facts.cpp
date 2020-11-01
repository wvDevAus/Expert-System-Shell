#include "Facts.hpp"

#include "utility/SymbolsJSON.hpp"

namespace expert_system::knowledge::facts {

    EnumFact::EnumFact(const std::list<std::string>& ordered_names) {
        // Call the DynamicEnum's parameterized constructor
        enum_ = utility::DynamicEnum(ordered_names);
    }

    VariantFact::VariantFact() : type_(utility::ExpertSystemTypes::kUnknown) {}

    VariantFact::VariantFact(utility::ExpertSystemTypes type) {
        // Store the hint for the generated type
        type_ = type;

        // Generate and store a specific Fact specialization
        switch (type) {
            case utility::ExpertSystemTypes::kBool: {
                fact_ = BoolFact();
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                fact_ = IntFact();
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                fact_ = FloatFact();
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                fact_ = EnumFact();
                break;
            }
            default: {
                fact_ = std::monostate();
                type_ = utility::ExpertSystemTypes::kUnknown;
                break;
            }
        }
    }

    void to_json(nlohmann::json& json_sys, const VariantFact& target) {
        // Assign the type
        json_sys[utility::JSON_ID_TYPE] = target.type_;

        // Split logic depending on the current type
        switch (target.type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Get the raw Fact type
                auto fact_raw = std::get<BoolFact>(target.fact_);

                // Export the description
                json_sys[utility::JSON_ID_DESCRIPTION] = fact_raw.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_raw.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MIN] = fact_range.value().min_;
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MAX] = fact_range.value().max_;
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_INCLUSIVE] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys[utility::JSON_ID_RANGE] = nullptr;
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Get the raw Fact type
                auto fact_raw = std::get<IntFact>(target.fact_);

                // Export the description
                json_sys[utility::JSON_ID_DESCRIPTION] = fact_raw.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_raw.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MIN] = fact_range.value().min_;
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MAX] = fact_range.value().max_;
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_INCLUSIVE] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys[utility::JSON_ID_RANGE] = nullptr;
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Get the raw Fact type
                auto fact_raw = std::get<FloatFact>(target.fact_);

                // Export the description
                json_sys[utility::JSON_ID_DESCRIPTION] = fact_raw.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_raw.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MIN] = fact_range.value().min_;
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MAX] = fact_range.value().max_;
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_INCLUSIVE] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys[utility::JSON_ID_RANGE] = nullptr;
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Get the proxy Fact type
                auto fact_proxy = std::get<EnumFact>(target.fact_);

                // Export the Fact's DynamicEnum
                json_sys[utility::JSON_ID_ENUM] = fact_proxy.enum_;

                // Export the description
                json_sys[utility::JSON_ID_DESCRIPTION] = fact_proxy.fact_.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_proxy.fact_.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MIN] = fact_proxy.enum_.At(fact_range.value().min_).value();
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_MAX] = fact_proxy.enum_.At(fact_range.value().max_).value();
                    json_sys[utility::JSON_ID_RANGE][utility::JSON_ID_RANGE_INCLUSIVE] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys[utility::JSON_ID_RANGE] = nullptr;
                }

                // Finished
                break;
            }
            default: {
                // Assign no more data
                break;
            }
        }
    }

    void from_json(const nlohmann::json& json_sys, VariantFact& target) {
        // Clear the existing Fact data
        target.fact_ = std::monostate();
        target.type_ = utility::ExpertSystemTypes::kUnknown;

        // Check if the type data is stored
        if (json_sys.find(utility::JSON_ID_TYPE) != json_sys.end()) {
            // Attempt to gather the type
            if (json_sys.at(utility::JSON_ID_TYPE).is_string()) {
                // Store the type
                target.type_ = json_sys.at(utility::JSON_ID_TYPE).get<utility::ExpertSystemTypes>();
            }
        }

        // Process the Fact data
        switch (target.type_) {
            case utility::ExpertSystemTypes::kBool: {
                // Create and track an empty BoolFact
                target.fact_.emplace<BoolFact>();
                auto& target_fact = std::get<BoolFact>(target.fact_);

                // Attempt to find the Fact's description
                if (json_sys.find(utility::JSON_ID_DESCRIPTION) != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at(utility::JSON_ID_DESCRIPTION).is_string()) {
                        // Store the description
                        target_fact.SetDescription(json_sys.at(utility::JSON_ID_DESCRIPTION).get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find(utility::JSON_ID_RANGE) != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at(utility::JSON_ID_RANGE).empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MIN) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MAX) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_INCLUSIVE) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_fact.SetRange(json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).get<bool>(),
                                             json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).get<bool>(),
                                             json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).get<bool>());
                    }
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kInt: {
                // Create and track an empty IntFact
                target.fact_.emplace<IntFact>();
                auto& target_fact = std::get<IntFact>(target.fact_);

                // Attempt to find the Fact's description
                if (json_sys.find(utility::JSON_ID_DESCRIPTION) != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at(utility::JSON_ID_DESCRIPTION).is_string()) {
                        // Store the description
                        target_fact.SetDescription(json_sys.at(utility::JSON_ID_DESCRIPTION).get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find(utility::JSON_ID_RANGE) != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at(utility::JSON_ID_RANGE).empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MIN) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).is_number_integer()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MAX) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).is_number_integer()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_INCLUSIVE) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_fact.SetRange(json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).get<int>(),
                                             json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).get<int>(),
                                             json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).get<bool>());
                    }
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kFloat: {
                // Create and track an empty FloatFact
                target.fact_.emplace<FloatFact>();
                auto& target_fact = std::get<FloatFact>(target.fact_);

                // Attempt to find the Fact's description
                if (json_sys.find(utility::JSON_ID_DESCRIPTION) != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at(utility::JSON_ID_DESCRIPTION).is_string()) {
                        // Store the description
                        target_fact.SetDescription(json_sys.at(utility::JSON_ID_DESCRIPTION).get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find(utility::JSON_ID_RANGE) != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at(utility::JSON_ID_RANGE).empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MIN) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).is_number_float()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MAX) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).is_number_float()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_INCLUSIVE) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_fact.SetRange(json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).get<float>(),
                                             json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).get<float>(),
                                             json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).get<bool>());
                    }
                }

                // Finished
                break;
            }
            case utility::ExpertSystemTypes::kEnum: {
                // Create and track an empty EnumFact
                target.fact_.emplace<EnumFact>();
                auto& target_proxy = std::get<EnumFact>(target.fact_);

                // Attempt to get the Fact's DynamicEnum
                if (json_sys.find(utility::JSON_ID_ENUM) != json_sys.end()) {
                    // Store the DynamicEnum
                    target_proxy.enum_ = json_sys.at(utility::JSON_ID_ENUM).get<utility::DynamicEnum>();
                }

                // Attempt to find the Fact's description
                if (json_sys.find(utility::JSON_ID_DESCRIPTION) != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at(utility::JSON_ID_DESCRIPTION).is_string()) {
                        // Store the description
                        target_proxy.fact_.SetDescription(json_sys.at(utility::JSON_ID_DESCRIPTION).get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find(utility::JSON_ID_RANGE) != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at(utility::JSON_ID_RANGE).empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MIN) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).is_string()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_MAX) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).is_string()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.at(utility::JSON_ID_RANGE).find(utility::JSON_ID_RANGE_INCLUSIVE) == json_sys.at(utility::JSON_ID_RANGE).end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_proxy.fact_.SetRange(target_proxy.enum_.At(json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MIN).get<std::string>()).value(),
                                                    target_proxy.enum_.At(json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_MAX).get<std::string>()).value(),
                                                    json_sys.at(utility::JSON_ID_RANGE).at(utility::JSON_ID_RANGE_INCLUSIVE).get<bool>());
                    }
                }

                // Finished
                break;
            }
            default:
                // Do nothing
                break;
        }
    }

} // namespace expert_system::knowledge::facts
