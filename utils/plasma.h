#ifndef PLASMA_COLORSCHEME_CLI_PLASMA_H
#define PLASMA_COLORSCHEME_CLI_PLASMA_H

#include <QtCore/QString>

int plasmaApplyColorScheme(const QStringList &colors);
void plasmaApplyWidgetStyle(const QStringList &widgets);
void plasmaApplyWidgetStyle(const QString &widgetStyle);

#endif
