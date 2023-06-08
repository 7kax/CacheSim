#include <iostream>
#include "Config.h"
#include "IO.h"
#include "general.h"
using namespace std;

int main(int argc, char *argv[])
{
    IO::iManager.initIO(argc, argv);
    general::TOP.initGeneral();
    general::TOP.run();
    general::TOP.showInfo();

    return 0;
}