#include "gameStatesQueue.h"
#include "constants.h"
#include <chrono>
#include <stdio.h>

uint64_t
GameStatesQueue::timeSinceEpochMillisec()
{

    return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
      .count();
}

GameState
GameStatesQueue::popFront()
{
    if (m_queueStates.size() == 0)
        return undefined;
    GameState state = m_queueStates.front();
    m_queueStates.pop_front();
    countDequeue++;
    return state;
}

GameState
GameStatesQueue::popFront(GameState gs)
{
    if (m_queueStates.size() == 0)
        return gs;
    GameState state = m_queueStates.front();
    m_queueStates.pop_front();
    countDequeue++;
    return state;
}

void
GameStatesQueue::pushBack(GameState gs)
{
    countEnqueue++;
    m_queueStates.push_back(gs);
}

void
GameStatesQueue::pushFront(GameState gs)
{
    printf("pushFront\n");
    countEnqueue++;
    m_queueStates.push_front(gs);
}

void
GameStatesQueue::pushBackAfterAWhileInMs(GameState gs, int timeInMs)
{
    printf("start thread  %llu  - %llu\n",
           timeSinceEpochMillisec(),
           timeSinceEpochMillisec() + timeInMs);
    sf::Thread* t = new sf::Thread([this, gs, timeInMs, t]() {
        sf::Clock c;
        c.restart();

        // while (c.getElapsedTime().asMilliseconds() < timeInMs) {
        //     sf::sleep(sf::Time(sf::milliseconds(timeInMs / 10)));
        //     printf("  wait : %d\n", c.getElapsedTime().asMilliseconds());
        // }

        printf("  wait : %d\n", timeInMs);
        sf::sleep(sf::Time(sf::milliseconds(timeInMs)));

        printf("post in queue\n");
        m_queueStates.push_front(gs);
    });
    printf("save thread %p\n", t);
    ThreadDuration* ptd = new ThreadDuration;
    // { t, timeSinceEpochMillisec() + timeInMs };
    ptd->deallocTime = timeSinceEpochMillisec() + timeInMs;
    ptd->pthread = t;
    m_threadsToFree.push_back(ptd);
    // m_threadsToFree.push_back(ptd);
    printf("launch thread %p\n", t);
    t->launch();
}

void
GameStatesQueue::clear()
{
    m_queueStates.clear();
}

void
GameStatesQueue::sweepFinishedThreads()
{
    // for (std::vector<sf::Thread*>::iterator t = m_threadsToFree.begin();
    //      t != m_threadsToFree.end();
    //      t++) {

    //    (*t)->wait();
    //    printf("deleting thread %p\n", *t);
    //    delete *t;
    //}

    std::vector<ThreadDuration*> newAllocatedThreads;
    uint64_t currentEpoch = timeSinceEpochMillisec();
    for (std::vector<ThreadDuration*>::iterator ptd = m_threadsToFree.begin();
         ptd != m_threadsToFree.end();
         ptd++) {

        if (currentEpoch >
              (*ptd)->deallocTime + SAFETY_ALLOWANCE_DEALLOCATION_TIME_MS &&
            !(*ptd)->isDeallocated) {
            printf(
              "Comparing %llu vs %llu\n", currentEpoch, (*ptd)->deallocTime);
            printf("deleting thread %p\n", (*ptd)->pthread);
            (*ptd)->isDeallocated = true;
            (*ptd)->pthread->wait();
            delete (*ptd)->pthread;
            delete (*ptd);
        } else {
            // keep alived threads
            // printf("keep thread %p\n", (*ptd)->pthread);
            newAllocatedThreads.push_back(*ptd);
        }
    }
    m_threadsToFree.clear();
    m_threadsToFree = newAllocatedThreads; // copy
}

void
GameStatesQueue::debugQueueSize()
{
    int qs = (int)m_queueStates.size();
    if (qs > 0)
        printf("Game states size:%d (%d-%d)\n", qs, countEnqueue, countDequeue);
}
