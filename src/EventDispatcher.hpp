#pragma once

#include <algorithm>
#include <utility>
#include <vector>

template <typename Type, typename Data>
class EventDispatcher {
public:
    class Observer {
    public:
        virtual void onEventCalled(const Type& type, const Data& data) = 0;
    };

public:
    /**  */
    void subscribe(Observer& observer);

    /**  */
    void unsubscribe(Observer& observer);

    /**  */
    void signal(const Type& type, const Data& data);

private:
    enum class DelayedAction { Subscribe, Unsubscribe };

    std::vector<Observer*> _observers;
    std::vector<std::pair<DelayedAction, Observer*>> _delayedActions;
    bool _isRunning = false;
};

// --------------

template <typename Type, typename Data>
void EventDispatcher<Type, Data>::subscribe(EventDispatcher::Observer& observer) {
    if (_isRunning) {
        _delayedActions.emplace_back(DelayedAction::Subscribe, &observer);
    } else {
        _observers.emplace_back(&observer);
    }
}

template <typename Type, typename Data>
void EventDispatcher<Type, Data>::unsubscribe(EventDispatcher::Observer& observer) {
    if (_isRunning) {
        _delayedActions.emplace_back(DelayedAction::Unsubscribe, &observer);
    } else {
        _observers.erase(std::remove(_observers.begin(), _observers.end(), &observer));
    }
}

template <typename Type, typename Data>
void EventDispatcher<Type, Data>::signal(const Type& type, const Data& data) {
    _isRunning = true;
    for (Observer* observer : _observers) {
        observer->onEventCalled(type, data);
    }
    _isRunning = false;

    if (!_delayedActions.empty()) {
        for (size_t i = 0; i < _delayedActions.size(); ++i) {
            switch (_delayedActions[i].first) {
                case DelayedAction::Subscribe:
                    subscribe(*_delayedActions[i].second);
                    break;
                case DelayedAction::Unsubscribe:
                    unsubscribe(*_delayedActions[i].second);
                    break;
                default:
                    break;
            }
        }

        _delayedActions.resize(0);
    }
}
