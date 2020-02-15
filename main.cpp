#include <QString>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include "colorscheme.h"
#include "exceptions/RuntimeException.h"

int main(int argc, char **argv) {
    Q_UNUSED(argc)

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Plasma theme switcher");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Quickly apply plasma color schemes from the command-line");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption colorShemeOption(
            QStringList{"c", "colors"},
            "A file containing plasma color scheme information",
            "colors"
    );

    parser.addOption(colorShemeOption);
    parser.process(app);

    if (!parser.isSet(colorShemeOption)) {
        parser.showHelp(1);
    }

    try {
        plasmaApplyColorScheme(parser.value(colorShemeOption));
    } catch (RuntimeException &e) {
        QTextStream(stderr) << "ERR: " << e.message() << "\n";

        return 1;
    }

    return 0;
}
