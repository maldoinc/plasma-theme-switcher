#include <QString>
#include <QtCore/QProcess>
#include "signals.h"

void dbusSignal(const QString &path, const QString &interface, const QString &name, QStringList arguments) {
    arguments.prepend(interface + "." + name);
    arguments.prepend(path);
    arguments.prepend("--type=signal");

    QProcess process;
    process.start("dbus-send", arguments);
    process.waitForFinished();
}

void kdeGlobalSettingsNotifyChange(KdeGlobalsChangeType changeType) {
    auto type = QString("int32:%0").arg(changeType);

    dbusSignal("/KGlobalSettings", "org.kde.KGlobalSettings", "notifyChange", QStringList{type, "int32:0"});
}