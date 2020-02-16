#include <QString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtCore/QFileInfo>
#include "exceptions/RuntimeException.h"

enum KdeGlobalsChangeType {
    ColorSchemeChanged = 0,
    WidgetStyleChanged = 2
};

void configMerge(const KSharedConfigPtr &srcConf, const KSharedConfigPtr &dstConf) {
    for (const QString &group: srcConf->groupList()) {
        if (group == "General") {
            continue;
        }

        KConfigGroup cg(srcConf, group);
        KConfigGroup cb2(dstConf, group);

        cg.copyTo(&cb2);
    }
}

QString colorSchemeFileGetName(const QString &filename) {
    return QFileInfo(filename).baseName();
}

void applyColorSchemeToFile(const QString &src, const QString &dst) {
    KSharedConfigPtr srcConf = KSharedConfig::openConfig(src, KSharedConfig::CascadeConfig);
    KSharedConfigPtr dstConf = KSharedConfig::openConfig(dst, KSharedConfig::CascadeConfig);

    configMerge(srcConf, dstConf);

    KConfigGroup(dstConf, "General").writeEntry("ColorScheme", colorSchemeFileGetName(src));
    dstConf->sync();
}

void kdeGlobalSettingsNotifyChange(KdeGlobalsChangeType changeType) {
    QDBusMessage message = QDBusMessage::createSignal(
            "/KGlobalSettings",
            "org.kde.KGlobalSettings",
            "notifyChange"
    );

    message.setArguments(QList<QVariant>{changeType, 0});
    QDBusConnection::sessionBus().send(message);
}

void reloadKwin() {
    QDBusMessage message = QDBusMessage::createSignal("/KWin", "org.kde.KWin", "reloadConfig");
    QDBusConnection::sessionBus().send(message);
}

inline QString locateKdeGlobals() {
    return QStandardPaths::locate(QStandardPaths::GenericConfigLocation, "kdeglobals");
}

void plasmaApplyColorScheme(const QString &source) {
    if (!QFileInfo(source).isReadable()) {
        throw RuntimeException(QStringLiteral("File \"%1\" is not readable").arg(source));
    }

    applyColorSchemeToFile(source, locateKdeGlobals());

    kdeGlobalSettingsNotifyChange(ColorSchemeChanged);
    reloadKwin();
}

QString plasmaReadCurrentColorScheme() {
    KSharedConfigPtr conf = KSharedConfig::openConfig(locateKdeGlobals(), KSharedConfig::CascadeConfig);

    return conf->group("General").readEntry("ColorScheme");
}

void plasmaApplyColorScheme(const QStringList &colors) {
    const int numItems = colors.length();

    if (numItems == 0 || numItems > 2) {
        throw RuntimeException(QStringLiteral("Invalid number of items in colors list. Expected 1 or 2, found %1").arg(
                numItems));
    }

    if (numItems == 1) {
        plasmaApplyColorScheme(colors.first());

        return;
    }

    const QString scheme = plasmaReadCurrentColorScheme() == colorSchemeFileGetName(colors.first())
                           ? colors[1]
                           : colors[0];

    plasmaApplyColorScheme(scheme);
}

void emitWidgetStyleChangedSignals(const QString &widgetStyle) {
    kdeGlobalSettingsNotifyChange(WidgetStyleChanged);

    for (const QString &interface: QStringList{"org.freedesktop.impl.portal.Settings",
                                               "org.freedesktop.portal.Settings"}) {
        QDBusMessage message = QDBusMessage::createSignal(
                "/org/freedesktop/portal/desktop",
                interface,
                "SettingChanged"
        );
        message.setArguments(QList<QVariant>{"org.kde.kdeglobals.KDE", "widgetStyle", widgetStyle});
        QDBusConnection::sessionBus().send(message);
    }
}

void plasmaApplyWidgetStyle(const QString &widgetStyle) {
    KSharedConfigPtr conf = KSharedConfig::openConfig(locateKdeGlobals(), KSharedConfig::CascadeConfig);
    conf->group("KDE").writeEntry("widgetStyle", widgetStyle);
    conf->sync();

    emitWidgetStyleChangedSignals(widgetStyle);
}