#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

enum TrafficLightPhase {
    red,
    green
};

class Vehicle;

template <class T>
class MessageQueue {
public:
    void send(T&& message);
    T receive();

private:
    std::deque<T> _queue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

class TrafficLight : public TrafficObject {
public:
    TrafficLight();
    TrafficLightPhase getCurrentPhase();
    void waitForGreen();
    void simulate();

private:
    void cycleThroughPhases();    
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> _messages;
};

#endif