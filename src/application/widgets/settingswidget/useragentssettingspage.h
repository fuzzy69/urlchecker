#pragma once

#include <QWidget>

#include "my/browserutils.h"

class Ui_UserAgentsSettings;

using my::browser::RotationStrategy;

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
