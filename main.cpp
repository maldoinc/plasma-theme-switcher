#include <QString>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include "utils/plasma.h"
#include "exceptions/RuntimeException.h"

int main(int argc, char **argv) {
    Q_UNUSED(argc)

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Plasma theme switcher");
    QCoreApplication::setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Quickly apply plasma color schemes and widget styles from the command-line");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption colorShemeOption(
            QStringList{"c", "colors"},
            "A file containing plasma color scheme information. Use this argument once to apply directly or twice to cycle.",
            "colors"
    );

    QCommandLineOption widgetStyleOption(
            QStringList{"w", "widgetStyle"},
            R"(Name of widget style to apply. Use argument once to apply directly or twice to cycle.")",
            "widgetStyle"
    );

    parser.addOption(colorShemeOption);
    parser.addOption(widgetStyleOption);
    parser.process(app);

    if (!(parser.isSet(colorShemeOption) || parser.isSet(widgetStyleOption))) {
        parser.showHelp(1);
    }

    try {
        int appliedColorSchemeIndex = -1;

        if (parser.isSet(colorShemeOption)) {
            appliedColorSchemeIndex = plasmaApplyColorScheme(parser.values(colorShemeOption));
        }

        if (parser.isSet(widgetStyleOption)) {
            const auto widgetStyles = parser.values(widgetStyleOption);

            // If we already applied a color scheme, apply the matching widget style instead of cycling them.
            // This lets the user apply both options as a pair.
            if (appliedColorSchemeIndex == -1 || widgetStyles.count() == 1) {
                plasmaApplyWidgetStyle(widgetStyles);
            } else {
                plasmaApplyWidgetStyle(widgetStyles[appliedColorSchemeIndex]);
            }
        }
    } catch (RuntimeException &e) {
        QTextStream(stderr) << "ERR: " << e.message() << "\n";

        return 1;
    }

    return 0;
}
