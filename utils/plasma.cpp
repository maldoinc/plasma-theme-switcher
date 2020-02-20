#include <QString>
#include <KSharedConfig>
#include <KConfigGroup>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusConnection>
#include <QtCore/QFileInfo>
#include "signals.h"
#include "exceptions/RuntimeException.h"

void configMerge(const KSharedConfigPtr &srcConf, const KSharedConfigPtr &dstConf) {
    for (const QString &group: srcConf->groupList()) {
        // General group contains no color information and the name will be applied via another call
        // as the name described here is not the one that should be used
        if (group == "General") {
            continue;
        }

        KConfigGroup cg(srcConf, group);
        KConfigGroup cb2(dstConf, group);

        cg.copyTo(&cb2);
    }
}

/**
 * Returns the basename of the filename in the argument. Plasma uses that as the "name" of the active colorscheme
 * in kdeglobals despite color schemes containing two "name" entries in "Name" and "ColorScheme".
 *
 * Settings app uses General.Name key as the display label for color schemes
 */
QString colorSchemeFileGetName(const QString &filename) {
    return QFileInfo(filename).baseName();
}

void assertListHasOneOrTwoItems(const QStringList &list, const QString &listName) {
    const int numItems = list.length();

    if (numItems == 0 || numItems > 2) {
        auto message = QStringLiteral("Invalid number of items in %1 list. Expected 1 or 2, found %2");

        throw RuntimeException(message.arg(listName).arg(numItems));
    }
}

void applyColorSchemeToFile(const QString &src, const QString &dst) {
    KSharedConfigPtr srcConf = KSharedConfig::openConfig(src, KSharedConfig::CascadeConfig);
    KSharedConfigPtr dstConf = KSharedConfig::openConfig(dst, KSharedConfig::CascadeConfig);

    configMerge(srcConf, dstConf);

    KConfigGroup(dstConf, "General").writeEntry("ColorScheme", colorSchemeFileGetName(src));
    dstConf->sync();
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
}

QString plasmaReadCurrentColorScheme() {
    KSharedConfigPtr conf = KSharedConfig::openConfig(locateKdeGlobals(), KSharedConfig::CascadeConfig);

    return conf->group("General").readEntry("ColorScheme");
}

QString plasmaReadCurrentWidgetStyle() {
    KSharedConfigPtr conf = KSharedConfig::openConfig(locateKdeGlobals(), KSharedConfig::CascadeConfig);

    return conf->group("KDE").readEntry("widgetStyle");
}

void plasmaApplyColorScheme(const QStringList &colors) {
    assertListHasOneOrTwoItems(colors, "color scheme");

    if (colors.length() == 1) {
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

    for (const QString &intf: QStringList{"org.freedesktop.impl.portal.Settings", "org.freedesktop.portal.Settings"}) {
        dbusSignal(
                "/org/freedesktop/portal/desktop",
                intf,
                "SettingChanged",
                QStringList{"org.kde.kdeglobals.KDE", "widgetStyle", widgetStyle}
        );
    }
}

void plasmaApplyWidgetStyle(const QString &widgetStyle) {
    KSharedConfigPtr conf = KSharedConfig::openConfig(locateKdeGlobals(), KSharedConfig::CascadeConfig);
    conf->group("KDE").writeEntry("widgetStyle", widgetStyle);
    conf->sync();

    emitWidgetStyleChangedSignals(widgetStyle);
}

void plasmaApplyWidgetStyle(const QStringList &widgets) {
    assertListHasOneOrTwoItems(widgets, "widget style");

    if (widgets.length() == 1) {
        plasmaApplyWidgetStyle(widgets.first());

        return;
    }

    plasmaApplyWidgetStyle(plasmaReadCurrentWidgetStyle() == widgets.first() ? widgets[1] : widgets[0]);
}
