#include "FactTypes.hpp"

namespace expert_system {

    EnumFact::EnumFact(const std::list<std::string>& ordered_names) {
        // Call the DynamicEnum's parameterized constructor
        enum_ = DynamicEnum(ordered_names);
    }

    VariantFact::VariantFact() : type_(FactType::kUnknown) {}

    VariantFact::VariantFact(FactType type) {
        // Store the hint for the generated type
        type_ = type;

        // Generate and store a specific Fact specialization
        switch (type) {
            case FactType::kBoolFact: {
                fact_ = BoolFact();
                break;
            }
            case FactType::kIntFact: {
                fact_ = IntFact();
                break;
            }
            case FactType::kFloatFact: {
                fact_ = FloatFact();
                break;
            }
            case FactType::kEnumFact: {
                fact_ = EnumFact();
                break;
            }
            default: {
                fact_ = std::monostate();
                type_ = FactType::kUnknown;
                break;
            }
        }
    }

    void to_json(nlohmann::json& json_sys, const VariantFact& target) {
        // Assign the type
        json_sys["type"] = target.type_;

        // Split logic depending on the current type
        switch (target.type_) {
            case FactType::kBoolFact: {
                // Get the raw Fact type
                auto fact_raw = std::get<BoolFact>(target.fact_);

                // Export the description
                json_sys["description"] = fact_raw.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_raw.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys["range"]["min"] = fact_range.value().min_;
                    json_sys["range"]["max"] = fact_range.value().max_;
                    json_sys["range"]["inclusive"] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys["range"] = nullptr;
                }

                // Finished
                break;
            }
            case FactType::kIntFact: {
                // Get the raw Fact type
                auto fact_raw = std::get<IntFact>(target.fact_);

                // Export the description
                json_sys["description"] = fact_raw.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_raw.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys["range"]["min"] = fact_range.value().min_;
                    json_sys["range"]["max"] = fact_range.value().max_;
                    json_sys["range"]["inclusive"] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys["range"] = nullptr;
                }

                // Finished
                break;
            }
            case FactType::kFloatFact: {
                // Get the raw Fact type
                auto fact_raw = std::get<FloatFact>(target.fact_);

                // Export the description
                json_sys["description"] = fact_raw.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_raw.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys["range"]["min"] = fact_range.value().min_;
                    json_sys["range"]["max"] = fact_range.value().max_;
                    json_sys["range"]["inclusive"] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys["range"] = nullptr;
                }

                // Finished
                break;
            }
            case FactType::kEnumFact: {
                // Get the proxy Fact type
                auto fact_proxy = std::get<EnumFact>(target.fact_);

                // Export the Fact's DynamicEnum
                json_sys["enum"] = fact_proxy.enum_;

                // Export the description
                json_sys["description"] = fact_proxy.fact_.GetDescription();

                // Catch if the Fact has a Range
                auto fact_range = fact_proxy.fact_.GetRange();
                if (fact_range != std::nullopt) {
                    // Store the Range properties
                    json_sys["range"]["min"] = fact_proxy.enum_.At(fact_range.value().min_).value();
                    json_sys["range"]["max"] = fact_proxy.enum_.At(fact_range.value().max_).value();
                    json_sys["range"]["inclusive"] = fact_range.value().bounds_inclusive_;
                }
                else {
                    // Store a null Range
                    json_sys["range"] = nullptr;
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
        target.type_ = FactType::kUnknown;

        // Check if the type data is stored
        if (json_sys.find("type") != json_sys.end()) {
            // Attempt to gather the type
            if (json_sys.at("type").is_string()) {
                // Store the type
                target.type_ = json_sys.at("type").get<FactType>();
            }
            else {
                // Catch the inability to process the data and stop
                return;
            }
        }

        // Process the Fact data
        switch (target.type_) {
            case FactType::kBoolFact: {
                // Create and track an empty BoolFact
                target.fact_.emplace<BoolFact>();
                auto target_fact = std::get<BoolFact>(target.fact_);

                // Attempt to find the Fact's description
                if (json_sys.find("description") != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at("description").is_string()) {
                        // Store the description
                        target_fact.SetDescription(json_sys.at("description").get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find("range") != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at("range").empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.find("min") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("min").is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.find("max") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("max").is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.find("inclusive") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("inclusive").is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_fact.SetRange(json_sys.at("min").get<bool>(),
                                             json_sys.at("max").get<bool>(),
                                             json_sys.at("inclusive").get<bool>());
                    }
                }

                // Finished
                break;
            }
            case FactType::kIntFact: {
                // Create and track an empty IntFact
                target.fact_.emplace<IntFact>();
                auto target_fact = std::get<IntFact>(target.fact_);

                // Attempt to find the Fact's description
                if (json_sys.find("description") != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at("description").is_string()) {
                        // Store the description
                        target_fact.SetDescription(json_sys.at("description").get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find("range") != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at("range").empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.find("min") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("min").is_number_integer()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.find("max") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("max").is_number_integer()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.find("inclusive") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("inclusive").is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_fact.SetRange(json_sys.at("min").get<int>(),
                                             json_sys.at("max").get<int>(),
                                             json_sys.at("inclusive").get<bool>());
                    }
                }

                // Finished
                break;
            }
            case FactType::kFloatFact: {
                // Create and track an empty FloatFact
                target.fact_.emplace<FloatFact>();
                auto target_fact = std::get<FloatFact>(target.fact_);

                // Attempt to find the Fact's description
                if (json_sys.find("description") != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at("description").is_string()) {
                        // Store the description
                        target_fact.SetDescription(json_sys.at("description").get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find("range") != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at("range").empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.find("min") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("min").is_number_float()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.find("max") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("max").is_number_float()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.find("inclusive") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("inclusive").is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_fact.SetRange(json_sys.at("min").get<float>(),
                                             json_sys.at("max").get<float>(),
                                             json_sys.at("inclusive").get<bool>());
                    }
                }

                // Finished
                break;
            }
            case FactType::kEnumFact: {
                // Create and track an empty EnumFact
                target.fact_.emplace<EnumFact>();
                auto target_proxy = std::get<EnumFact>(target.fact_);

                // Attempt to get the Fact's DynamicEnum
                if (json_sys.find("enum") != json_sys.end()) {
                    if (json_sys.at("enum").is_string()) {
                        // Store the description
                        target_proxy.enum_ = json_sys.at("enum").get<DynamicEnum>();
                    }
                }

                // Attempt to find the Fact's description
                if (json_sys.find("description") != json_sys.end()) {
                    // Attempt to gather the description
                    if (json_sys.at("description").is_string()) {
                        // Store the description
                        target_proxy.fact_.SetDescription(json_sys.at("description").get<std::string>());
                    }
                }

                // Check if the Fact has a Range
                if (json_sys.find("range") != json_sys.end()) {
                    // Attempt to gather the Range
                    if (!json_sys.at("range").empty()) {
                        // Catch an invalid Range minimum
                        if (json_sys.find("min") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("min").is_string()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range maximum
                        if (json_sys.find("max") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("max").is_string()) {
                            // Catch failure and stop
                            break;
                        }

                        // Catch an invalid Range bounds flag
                        if (json_sys.find("inclusive") == json_sys.end()) {
                            // Catch failure and stop
                            break;
                        }
                        if (!json_sys.at("inclusive").is_boolean()) {
                            // Catch failure and stop
                            break;
                        }

                        // Store the Fact's Range
                        target_proxy.fact_.SetRange(target_proxy.enum_.At(json_sys.at("min").get<std::string>()).value(),
                                                    target_proxy.enum_.At(json_sys.at("max").get<std::string>()).value(),
                                                    json_sys.at("inclusive").get<bool>());
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

} // namespace expert_system
