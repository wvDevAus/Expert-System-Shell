#pragma once

namespace expert_system {

        /**
         * @brief A range of valid values for a Fact.
         * @tparam T The raw type of the Fact.
         */
    template<class T>
    struct Range {
            /// Use of the default constructor is not allowed!
        Range() = delete;

            /**
             * @brief Parameterized constructor.
             * @param [in] min The Range's minimum value.
             * @param [in] max The Range's maximum value.
             * @param [in] bounds_inclusive Flag for including the bounds of the Range.
             */
        Range(T min, T max, bool bounds_inclusive)
            : min_(min), max_(max), bounds_inclusive_(bounds_inclusive){};

            /**
             * @brief The minimum value.
             * @warning This may be larger than max_!
             */
        T min_;

            /**
             * @brief The maximum value.
             * @warning This may be smaller than min_!
             */
        T max_;

            /**
             * @brief Flag for including the bounds of the Range.
             * True if min_ and max_ should be valid values within the Range, False otherwise.
             */
        bool bounds_inclusive_;

            /**
             * @brief Checks if a provided value is within the specified Range.
             * @param range [in] The target Range of type T.
             * @param value [in] A value of type T to check.
             * @return True if the provided value is within the Range, otherwise False.
             * @warning This has undefined behaviour if the Range's min_ is smaller than max_!
             */
        static bool InRange(Range<T> range, T value) {
            // Catch if the bounds are also value values
            if (range.bounds_inclusive) {
                // Perform the check including bounds and return the result
                return (range.min_ <= value) && (value <= range.max_);
            }
            // Perform the check without bounds and return the result
            return (range.min_ < value) && (value < range.max_);
        };
    };

}// namespace expert_system
