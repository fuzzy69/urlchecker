#pragma once

#include <QMap>
#include <QMetaObject>

enum class ResultStatus : qint8 {
    OK,
    PROCESSING,
    FAILED
};

Q_DECLARE_METATYPE(ResultStatus);

static const QMap<ResultStatus, QString> ResultStatusText {
    { ResultStatus::OK, QStringLiteral("OK") },
    { ResultStatus::PROCESSING, QStringLiteral("Processing ...") },
    { ResultStatus::FAILED, QStringLiteral("Failed!") }
};
