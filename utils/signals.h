#include <QString>
#include <QStringList>

enum KdeGlobalsChangeType {
    ColorSchemeChanged = 0,
    WidgetStyleChanged = 2
};

void kdeGlobalSettingsNotifyChange(KdeGlobalsChangeType changeType);
void dbusSignal(const QString &path, const QString &interface, const QString &name, QStringList arguments);