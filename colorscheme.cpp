#include <QString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>

void configMerge(const KSharedConfigPtr &srcConf, const KSharedConfigPtr &dstConf) {
    for (const QString &group: srcConf->groupList()) {
        KConfigGroup cg(srcConf, group);
        KConfigGroup cb2(dstConf, group);

        cg.copyTo(&cb2);
    }
}

void applyColorSchemeToFile(const QString &src, const QString &dst) {
    KSharedConfigPtr srcConf = KSharedConfig::openConfig(src, KSharedConfig::CascadeConfig);
    KSharedConfigPtr dstConf = KSharedConfig::openConfig(dst, KSharedConfig::CascadeConfig);

    configMerge(srcConf, dstConf);

    QString schemeName = KConfigGroup(srcConf, "General").readEntry("Name");
    KConfigGroup(dstConf, "General").writeEntry("ColorScheme", schemeName);
    dstConf->sync();
}

void emitColorSchemeChangedSignals() {
    QDBusMessage message = QDBusMessage::createSignal("/KGlobalSettings", "org.kde.KGlobalSettings", "notifyChange");
    message.setArguments(QList<QVariant>{0, 0});
    QDBusConnection::sessionBus().send(message);
}

void reloadKwin() {
    QDBusMessage message = QDBusMessage::createSignal("/KWin", "org.kde.KWin", "reloadConfig");
    QDBusConnection::sessionBus().send(message);
}

void plasmaApplyColorScheme(const QString &source) {
    QString kdeglobals = QStandardPaths::locate(QStandardPaths::GenericConfigLocation, "kdeglobals");
    applyColorSchemeToFile(source, kdeglobals);

    emitColorSchemeChangedSignals();
    reloadKwin();
}