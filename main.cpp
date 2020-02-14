#include <QString>
#include <QtCore/QTextStream>
#include "colorscheme.h"
#include "exceptions/RuntimeException.h"

int main(int argc, char **argv) {
    Q_UNUSED(argc)

    try {
        plasmaApplyColorScheme(QString(argv[1]));
    } catch (RuntimeException &e) {
        QTextStream(stderr) << "ERR: " << e.message();
    }

    return 0;
}
