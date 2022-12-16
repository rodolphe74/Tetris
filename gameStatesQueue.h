#pragma once

#include "globals.h"
#include <queue>

class GameStatesQueue
{
  private:
    std::queue<GameState> m_queueStates;
    int countEnqueue = 0;
    int countDequeue = 0;

  public:
    GameState popFront();
    GameState popFront(GameState gs);
    void pushBack(GameState GameState);
    void debugQueueSize();
};
