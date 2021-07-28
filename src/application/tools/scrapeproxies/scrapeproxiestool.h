﻿#pragma once

#include <QIcon>
#include <QString>

#include "../tool.h"

class QMenu;

class ScrapeProxiesTool : public Tool
{
public:
    ScrapeProxiesTool();

public Q_SLOTS:
    void addProxiesToProxiesList();
    void copyProxiesToSourcesTable();
};
