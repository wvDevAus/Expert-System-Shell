#pragma once

#include <list>
#include <optional>
#include <string>
#include <unordered_map>

namespace expert_system {

        /**
         * @brief An enumeration system with customizable values.
         * Capable of being configured at runtime.
         */
    class DynamicEnum {
    public:
            /// Default constructor.
        DynamicEnum();

            /**
             * @brief Parameterized constructor, assigns starting enums values.
             * @param [in] ordered_names A list of enum names in ascending order.
             * @note Duplicate enum names will be discarded.
             */
        explicit DynamicEnum(std::list<std::string> ordered_names);

            /**
             * @brief Inserts a new enum value, assigned to the current 'end' position.
             * @param [in] name The name of the new enum value.
             * @return True if the new enum value was added, False otherwise.
             * @warning If the name is already used, this will fail and return False!
             */
        bool PushBack(std::string name);

            /**
             * @brief Inserts a new enum value at a specified position.
             * All existing enum values with a equal or 'higher' position will be moved 'up'.
             * @note Providing an invalid position will lead to identical behaviour as PushBack().
             * @param [in] name The name of the new enum value.
             * @param [in] position The intended position of the new enum value.
             * @return True if the new enum value was added, False otherwise.
             * @warning If the name is already used, this will fail and return False!
             */
        bool Insert(std::string name, int position);

            /**
             * @brief Checks if the provided name is already used for an enum value.
             * @param [in] name The name to search existing enum values for.
             * @return True if the provided name was found, false otherwise.
             */
        bool Has(std::string name);

            /**
             * @brief Gathers the amount of enum values stored.
             * @return The amount of enum values stored.
             */
        int Size();

            /**
             * @brief Attempts to gather the name of an enum value at a specified position.
             * @param [in] position The target enum value's position.
             * @return The name of the enum value, or std::nullopt on failure.
             * @note If the provided position is invalid, this will fail.
             */
        std::optional<std::string> At(int position);

            /**
             * @brief Attempts to gather the position of an enum value with a specified name.
             * @param name The name to search existing enum values for.
             * @return The position of the enum value, or std::nullopt on failure.
             * @note If the provided name is not found, this will fail.
             */
        std::optional<int> At(std::string name);

            /**
             * @brief Gathers the entire set of enum value names in ascending order.
             * @return A list of enum value names, in ascending order.
             * @note If no enum values are stored, this will return an empty list.
             */
        std::list<std::string> List();

            /**
             * @brief Removes an enum value via its name.
             * @param name The target enum's name.
             * @return [in] True if the specified enum value existed and was removed, False otherwise.
             * @note This will return False if the specified enum value does not exist.
             */
        bool Remove(std::string name);

            /**
             * @brief Removes an enum value via its position.
             * @param [in] position The target enum's position.
             * @return True if the specified enum value existed and was removed, False otherwise.
             * @note This will return False if the specified enum value does not exist.
             */
        bool Remove(int position);

            /**
             * @brief Deletes all of the stored enum values.
             * @note No names or positions will be valid until more enums values are added.
             */
        void Clear();

    private:
            /**
             * @brief Stores a mapping of the enum value's names to their positions.
             * @note The positions of enum values always begin at 0.
             */
        std::unordered_map<std::string, int> name_to_position_;

            /**
             * @brief Stores the names of the stored enum values in ascending order.
             * This mapping is implicitly defined from the name's position in the vector.
             * @note The positions of enum values always begin at 0.
             */
        std::vector<std::string> position_to_name_;
    };

} // namespace expert_system
