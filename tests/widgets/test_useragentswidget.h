#pragma once

#include <QtTest>

class TestUserAgentsWidget : public QObject {
    Q_OBJECT

private Q_SLOTS:
    void testAddUserAgents();
    void testRemoveAllUserAgents();
};
