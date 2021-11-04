#pragma once

#include <QtTest>

class TestSideBarWidget : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testAddActions();
    void testSelectingActions();
    void testSignals();
};
