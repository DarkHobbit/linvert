#include "linvertapplication.h"

int main(int argc, char *argv[])
{
    LInvertApplication a(argc, argv);
    int ret = a.start();
    a.exit(ret);
    return ret;
}
