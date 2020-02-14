#include <QtCore/QException>

class RuntimeException : QException {
protected:
    QString m_message;
public:
    explicit RuntimeException(const QString &message);

    QString message();
};
