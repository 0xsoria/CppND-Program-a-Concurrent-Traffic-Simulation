#include <iostream>
#include <random>
#include "TrafficLight.h"

#include <stdio.h>      
#include <stdlib.h>     
#include <time.h>

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> lock(_mutex);
    _condition.wait(lock, [this] { return !_queue.empty(); });
    T message = std::move(_queue.front());
    _queue.pop_front();
    return message;
}


template <typename T>
void MessageQueue<T>::send(T &&message)
{
    std::lock_guard<std::mutex> lock(_mutex); 
    _queue.clear();
    _queue.emplace_back(std::move(message));
    _condition.notify_one();
}
 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    while(true)
    {
        if(_messages.receive() == TrafficLightPhase::green)
        {
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}


void TrafficLight::simulate()
{
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}


void TrafficLight::cycleThroughPhases()
{
    int randCycleDuration = rand() % (6000-4000 + 1) + 4000;
    std::chrono::time_point<std::chrono::system_clock> lastUpdate;

    lastUpdate = std::chrono::system_clock::now();
    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        long timeSinceLastUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();
        if (timeSinceLastUpdate >= randCycleDuration)
        {
            randCycleDuration = rand()%(6000-4000 + 1) + 4000;

            if (_currentPhase == red)
            {
                _currentPhase = green;
            }
            else
            {
                _currentPhase = red;
            }
            _messages.send(std::move(_currentPhase)); 
            lastUpdate = std::chrono::system_clock::now();
        }

    }

}
