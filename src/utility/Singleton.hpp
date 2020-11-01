#pragma once

namespace expert_system::utility {

        /**
         * @brief Templated Singleton, providing global access to a single instance of a variable.
         * @tparam T The target class/type to encapsulate a single instance of.
         */
    template <class T>
    class Singleton {
    public:
            /**
             * @brief Provides access to the static instance of the T.
             * @return A reference to the protected T instance.
             */
        static T& Get() {
            // Return the instance by reference
            static T instance_;
            return instance_;
        }

            /// Use of copy or assignment constructors is not allowed.
        Singleton(const Singleton&) = delete;

            /// Use of copy or assignment constructors is not allowed.
        Singleton& operator=(const Singleton &) = delete;

            /// Use of copy or assignment constructors is not allowed.
        Singleton(Singleton &&) = delete;

            /// Use of copy or assignment constructors is not allowed.
        Singleton & operator=(Singleton &&) = delete;

    private:
            /// No direct construction of this class is permitted.
        Singleton() = default;
    };

} // namespace expert_system::utility
