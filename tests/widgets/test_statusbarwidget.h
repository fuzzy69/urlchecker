#pragma once

#include <QtTest>

class TestStatusBarWidget : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testButtons();
    void testActiveThreadsStatus();
    void testJobRuntimeStatus();
};
