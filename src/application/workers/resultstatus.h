#pragma once

#include <QMetaObject>


enum class ResultStatus : qint8
{
    OK,
    PROCESSING,
    FAILED
};

Q_DECLARE_METATYPE(ResultStatus)
