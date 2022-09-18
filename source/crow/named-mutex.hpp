#pragma once

#include "crow/io-utility.hpp"
#include "crow/types.hpp"
#include <chrono>
#include <string>

namespace Crow {

    class NamedMutex {

    public:

        explicit NamedMutex(const std::string& name);
        ~NamedMutex() noexcept;
        NamedMutex(const NamedMutex&) = delete;
        NamedMutex(NamedMutex&&) = delete;
        NamedMutex& operator=(const NamedMutex&) = delete;
        NamedMutex& operator=(NamedMutex&&) = delete;

        std::string name() const;
        void lock();
        bool try_lock();
        template <typename R, typename P> bool try_lock_for(std::chrono::duration<R, P> rel_time);
        bool try_lock_until(std::chrono::system_clock::time_point abs_time);
        void unlock() noexcept;

    private:

        std::string path_;
        void* handle_ = nullptr;

    };

        template <typename R, typename P>
        bool NamedMutex::try_lock_for(std::chrono::duration<R, P> rel_time) {
            using namespace std::chrono;
            if (rel_time <= duration<R, P>())
                return try_lock();
            else
                return try_lock_until(system_clock::now() + rel_time);
        }

}
