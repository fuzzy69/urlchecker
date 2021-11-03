#pragma once

#include <QtTest>

class TestProxiesWidget : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testAddProxies();
    void testRemoveAllProxies();
};
