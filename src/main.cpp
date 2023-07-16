#include "game.hpp"
#include <cstdlib>
#include <time.h>

int main (int argc, char *argv[])
{
    srand(time(NULL));
    ShSpace::init();
    ShSpace::run();
    ShSpace::quit();
    return 0;
}
