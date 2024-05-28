#include "tetris.h"
#include "animatedBackground.h"
#include "constants.h"
#include "fire.h"
#include "ia.h"

#include "game.h"



int
main()
{
    game* g;
    g = new game();
    g->beforeLoop();
    g->loop();
    g->freeAndExit();
    delete g;
}


//#if defined(_WINDOWS) && defined(NDEBUG)
//int WINAPI
//WinMain(_In_ HINSTANCE hThisInstance,
//        _In_opt_ HINSTANCE hPrevInstance,
//        _In_ LPSTR lpszArgument,
//        _In_ int nCmdShow)
//#endif
//
//#if defined(_WINDOWS) && !defined(NDEBUG)
//  int main()
//#endif
//
//#if !defined(_WINDOWS)
//    int main()
//#endif

