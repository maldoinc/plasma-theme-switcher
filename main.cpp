#include <QString>
#include <KSharedConfig>
#include <KConfigGroup>

void configMerge(const KSharedConfigPtr& srcConf, const KSharedConfigPtr& dstConf)
{
    for(const QString& group: srcConf->groupList()) {
        KConfigGroup cg(srcConf, group);
        KConfigGroup cb2(dstConf, group);

        cg.copyTo(&cb2);
    }
}

void applyColorScheme(const QString& src, const QString& dst)
{
    KSharedConfigPtr srcConf = KSharedConfig::openConfig(src, KSharedConfig::CascadeConfig);
    KSharedConfigPtr dstConf = KSharedConfig::openConfig(dst, KSharedConfig::CascadeConfig);

    configMerge(srcConf, dstConf);

    QString schemeName = KConfigGroup(srcConf, "General").readEntry("Name");
    KConfigGroup(dstConf, "General").writeEntry("ColorScheme", schemeName);
    dstConf->sync();
}

int main(int argc, char** argv) {
    Q_UNUSED(argc)

    auto kdeglobals = QStandardPaths::locate(QStandardPaths::GenericConfigLocation, "kdeglobals");
    applyColorScheme(QString(argv[1]), kdeglobals);

    return 0;
}
