#pragma once

#include <QWidget>

#include "../../../common/useragentmanager.h"

class Ui_UserAgentsSettings;

using common::browser::RotationStrategy;

class UserAgentsSettingsPage : public QWidget {
    Q_OBJECT
public:
    explicit UserAgentsSettingsPage(QWidget* parent = nullptr);
    ~UserAgentsSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void setRotationStrategy(RotationStrategy rotationStrategy);
    RotationStrategy rotationStrategy();

    Ui_UserAgentsSettings* m_ui;
};
