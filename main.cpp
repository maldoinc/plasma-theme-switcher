#include <QString>
#include "colorscheme.h"

int main(int argc, char **argv) {
    Q_UNUSED(argc)

    plasmaApplyColorScheme(QString(argv[1]));

    return 0;
}
