#pragma once

#include <functional>
#include <list>
#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

#include "databases/facts/FactTypes.hpp"

namespace expert_system {

        /**
         * @brief Type definition for the functions that can be used to iterate through the FactDatabase.
         * Compatible functions must take a string and const reference to a VariantFact, and return void.
         */
    using FactIteratingFunction = std::function<void(std::string, const VariantFact& fact)>;

        /// A database to centralize the storage and operation of Facts.
    class FactDatabase {
    public:
            /// Default constructor.
        FactDatabase();

            /**
             * @brief Attempts to create and store a new Fact into the database.
             * @param [in] name The name of the new Fact.
             * @param [in] type The specialized type of Fact to create.
             * @return A reference to the Fact if successful, or std::nullopt otherwise.
             * @note This will fail if the provided name is already in use.
             */
        std::optional<std::reference_wrapper<VariantFact>> Create(std::string name, FactType type);

            /**
             * @brief Attempts to gather a stored Fact by name.
             * @param [in] name The name of the target Fact.
             * @return A reference to the Fact if successful, or std::nullopt otherwise.
             */
        std::optional<std::reference_wrapper<VariantFact>> Find(std::string name);

            /**
             * @brief Gathers a list of the stored Fact names.
             * @return A list of Fact name strings, in ascending order.
             * @note If no Facts are stored, this will return an empty list.
             */
        std::list<std::string> List();

            /**
             * @brief Gathers the amount of the stored Facts.
             * @return The amount of stored Facts.
             */
        int Count();

            /**
             * @brief Attempts to delete a specific stored Fact.
             * @param [in] name The name of the target Fact.
             * @return True if the Fact was found and deleted, False otherwise.
             * @note This will return False if the specified Fact does not exist.
             */
        bool Remove(std::string name);

            /**
             * @brief Deletes all of the stored Facts.
             * @note This will remove all Fact data, including session values.
             */
        void Clear();

            /**
             * @brief Deletes all session data from all of the stored Facts.
             * @note Only the session value data is deleted, not the Fact itself.
             */
        void Reset();

            /**
             * @brief Iterates over all stored Facts, passing each to an external function.
             * @param [in] iterating_function A reference to function that can process the stored Facts.
             * @note No guarantees are made for Facts to be processed in a specific order.
             */
        void Iterate(FactIteratingFunction& iterating_function);

    private:
            /**
             * @brief The database for all stored Facts.
             * Maps the Fact name to its data structure.
             */
        std::unordered_map<std::string, VariantFact> stored_facts_;
    };

}// namespace expert_system
