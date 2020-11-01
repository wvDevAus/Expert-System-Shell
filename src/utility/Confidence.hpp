#pragma once

#include "nlohmann/json.hpp"

namespace expert_system::utility {

        /**
         * @brief A managed confidence factor.
         * Ensures that the confidence factor does not exceed the range of 0.0f and 1.0f.
         */
    class Confidence {
    public:
            /**
             * @brief Default constructor.
             * Initial confidence factor value of 0.0f.
             */
        Confidence();

            /**
             * @brief Parameterized constructor.
             * @param [in] value The confidence factor value to assign.
             * @note This will be replaced by the closest range bounds if outside them.
             */
        explicit Confidence(float value);

            /**
             * @brief Attempts to assign a confidence factor value.
             * @param [in] value The confidence factor value to assign.
             * @note This will be replaced by the closest range bounds if outside them.
             */
        void Set(float value);

            /**
             * @brief Provides a read-only access to the private confidence factor value.
             * @return A copy of the stored confidence factor value.
             */
        float Get();

            /**
             * @brief Creates a new confidence factor from combining this value with another.
             * @param [in] value The confidence factor value to assign.
             * @return A new Confidence, generated from multiplying the provided value with the stored value.
             */
        [[nodiscard]] Confidence Combine(const Confidence& value) const;

    private:
            /// The protected confidence factor value.
        float confidence_factor_;

            /// Enables JSON serializer access to private contents
        friend void to_json(nlohmann::json& json_sys, const Confidence& target);

            /// Enables JSON serializer access to private contents
        friend void from_json(const nlohmann::json& json_sys, Confidence& target);
    };

        /**
         * @brief Confidence serialization to JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the Confidence to export.
         */
    void to_json(nlohmann::json& json_sys, const Confidence& target);

        /**
         * @brief Confidence serialization from JSON format.
         * @param [in,out] json_sys A reference to a JSON object.
         * @param [in] target A reference to the Confidence to import.
         */
    void from_json(const nlohmann::json& json_sys, Confidence& target);

} // namespace expert_system::utility
