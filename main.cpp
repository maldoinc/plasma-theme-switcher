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

    QString schemeName = KConfigGroup(srcConf, QStringLiteral("General")).readEntry("Name");
    KConfigGroup(dstConf, QStringLiteral("General")).writeEntry("ColorScheme", schemeName);
    dstConf->sync();
}

int main(int argc, char** argv) {
    Q_UNUSED(argc)

    applyColorScheme(QString(argv[1]), QString(argv[2]));

    return 0;
}
