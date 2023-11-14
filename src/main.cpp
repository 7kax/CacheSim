#include "objects.h"
using namespace std;

int main(int argc, char *argv[]) {
    using namespace CacheSim;
    iManager.initIO(argc, argv);
    TOP.initGeneral();
    TOP.run();
    TOP.showInfo();

    return 0;
}