/*
 * SPDX-FileCopyrightText: Valve Corporation.
 * SPDX-License-Identifier: GPL-3.0
 */

#pragma once

#include <functional>
#include <list>

namespace util
{
    /**
     * @brief A class template for creating an observable object.
     *
     * Provides a way to create an object that can be observed by other objects.
     * It allows listeners to be added and removed, and provides a protected function
     * for notifying all listeners of an event.
     *
     * @tparam EventType The type of the event generated by the observable object.
     * @tparam Args The types of the arguments passed to the listeners when an event is generated.
     */
    template <typename EventType, typename... Args>
    class Observable
    {
      public:
        /**
         * @brief Type alias for the callback function used by listeners.
         */
        using Callback = std::function<void(const EventType&, Args...)>;

        /**
         * @brief Type alias for the handle used to identify listeners.
         */
        using Handle = typename std::list<Callback>::iterator;

        /**
         * @brief Adds a listener to the observable object.
         *
         * @param callback The callback function representing the listener.
         *
         * @return A handle that can be used to remove the listener from the list.
         */
        Handle append_listener(const Callback& callback);

        /**
         * @brief Removes a listener from the observable object.
         *
         * @param handle The handle of the listener to remove.
         */
        void remove_listener(const Handle& handle);

      protected:
        /**
         * @brief Notifies all listeners of an event.
         *
         * @param event The event generated by the observable object.
         * @param args The arguments to pass to the listeners.
         */
        void notify(const EventType& event, Args... args) const;

      private:
        /// The list of registered listeners.
        std::list<Callback> listeners_{};
    };

    template <typename EventType, typename... Args>
    typename Observable<EventType, Args...>::Handle
      Observable<EventType, Args...>::append_listener(const Callback& callback)
    {
        listeners_.push_back(callback);

        auto listener = listeners_.end();
        --listener;

        return listener;
    }

    template <typename EventType, typename... Args>
    void Observable<EventType, Args...>::remove_listener(const Handle& handle)
    {
        listeners_.erase(handle);
    }

    template <typename EventType, typename... Args>
    void Observable<EventType, Args...>::notify(const EventType& event, Args... args) const
    {
        for (const auto& listener : listeners_) {
            listener(event, args...);
        }
    }
}
