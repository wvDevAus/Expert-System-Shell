#include "DynamicEnum.hpp"

#include <tuple>

namespace expert_system {

    DynamicEnum::DynamicEnum() {
        // Do nothing, everything here is automated.
    }

    DynamicEnum::DynamicEnum(std::list<std::string> ordered_names) {
        // Iterate through the provided list
        for (auto current_name: ordered_names) {
            // Attempt to push the current enum value to the back
            PushBack(current_name);
        }
    }

    bool DynamicEnum::PushBack(std::string name) {
        // Catch if the name is already used
        if (Has(name)) {
            // Indicate failure
            return false;
        }

        // Add the enum value to the map, storing its position
        name_to_position_.insert(std::pair<std::string, int>(name, (int) position_to_name_.size()));

        // Add the enum value to the vector, storing its map iterator
        position_to_name_.push_back(name);

        // Indicate success
        return true;
    }

    bool DynamicEnum::Insert(std::string name, int position) {
        // Catch if the requested position for the new enum is invalid or at the end of the vector
        if ((position < 0) || ((int) name_to_position_.size() <= position)) {
            // Pass the new enum value to the PushBack to handle
            return PushBack(name);
        }

        // Catch if the name is already used
        if (Has(name)) {
            // Indicate failure
            return false;
        }

        // Add the enum value to the map, storing its position
        name_to_position_.insert(std::pair<std::string, int>(name, position));

        // Add the enum value to the vector, storing its map iterator
        position_to_name_.insert(position_to_name_.begin() + position, name);

        // Iterate through the vector, starting at the element first pushed forward
        for (auto iterator = position_to_name_.begin() + position; iterator != position_to_name_.end(); ++iterator) {
            // Increment the position value stored
            auto current_enum_name = *iterator;
            name_to_position_[current_enum_name] += 1;
        }

        // Indicate success
        return true;
    }

    bool DynamicEnum::Has(std::string name) {
        // Search the map for an existing key identical to the provided string
        return name_to_position_.find(name) == name_to_position_.end();
    }

    int DynamicEnum::Size() {
        // Gather the amount of elements stored within the map
        return (int) name_to_position_.size();
    }

    std::optional<std::string> DynamicEnum::At(int position) {
        // Catch if the position is invalid
        if ((position < 0) || ((int) name_to_position_.size() <= position)) {
            // Return an indication that the position is invalid
            return std::nullopt;
        }

        // Get the enum value's name at the requested position
        return position_to_name_[position];
    }

    std::optional<int> DynamicEnum::At(std::string name) {
        // Catch if the name is invalid
        if (!Has(name)) {
            // Return an indication that the position is invalid
            return std::nullopt;
        }

        // Get the enum value's position at the requested name
        return name_to_position_[name];
    }

    std::list<std::string> DynamicEnum::List() {
        // Create a temporary list to store the enum value names in order within
        std::list<std::string> ordered_names;

        // Iterate through the vector to get the enum value names in order
        for (auto current_name: position_to_name_) {
            // Append the gathered name to the back of the list
            ordered_names.push_back(current_name);
        }

        // Return a copy of the temporary list
        return ordered_names;
    }

    bool DynamicEnum::Remove(std::string name) {
        // Catch if the requested enum value does not exist
        if (!Has(name)) {
            // Stop and indicate failure
            return false;
        }

        // Get the position of the enum value and continue from there
        return Remove(name_to_position_[name]);
    }

    bool DynamicEnum::Remove(int position) {
        // Catch if the position is invalid
        if ((position < 0) || ((int) name_to_position_.size() <= position)) {
            // Stop and indicate failure
            return false;
        }

        // Remove the enum value from the map and vector
        name_to_position_.erase(position_to_name_[position]);
        position_to_name_.erase(position_to_name_.begin() + position);

        // Iterate through the vector, starting at the element first pushed forward
        for (auto iterator = position_to_name_.begin() + position; iterator != position_to_name_.end(); ++iterator) {
            // Decrement the position value stored
            auto current_enum_name = *iterator;
            name_to_position_[current_enum_name] -= 1;
        }

        // Indicate success
        return true;
    }

    void DynamicEnum::Clear() {
        // Empty the contents of the map
        name_to_position_.clear();

        // Empty the contents of the vector
        position_to_name_.clear();
    }

} // namespace expert_system
