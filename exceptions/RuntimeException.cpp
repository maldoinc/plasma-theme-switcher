#include "RuntimeException.h"

RuntimeException::RuntimeException(const QString &message) {
    m_message = message;
}

QString RuntimeException::message() {
    return m_message;
}
