#pragma once

#include "globals.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <vector>

struct ThreadDuration
{
    sf::Thread* pthread;
    uint64_t deallocTime;
    bool isDeallocated = false;
};

typedef ThreadDuration ThreadDuration;

class GameStatesQueue
{
  private:
    std::deque<GameState> m_queueStates;
    // std::vector<sf::Thread*> m_threadsToFree;
    std::vector<ThreadDuration*> m_threadsToFree;
    int countEnqueue = 0;
    int countDequeue = 0;
    uint64_t timeSinceEpochMillisec();

  public:
    GameState popFront();
    GameState popFront(GameState gs);
    void pushBack(GameState gs);
    void pushFront(GameState gs);
    void pushBackAfterAWhileInMs(GameState gs, int timeInMs);
    void clear();
    void sweepFinishedThreads();
    void debugQueueSize();
};
