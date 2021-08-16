#pragma once

#include <QWidget>

//#include "../../core/useragents/useragentsmanager.h"
#include "my/browserutils.h"

class Ui_UserAgentsSettings;

using my::browser::RotationStrategy;
//enum class UserAgentsStrategy
//{
//    DEFAULT_USER_AGENT,
//    RANDOM_USER_AGENT,
//    TIE_USER_AGENT_TO_PROXY
//};

class UserAgentsSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit UserAgentsSettingsPage(QWidget *parent = nullptr);
    ~UserAgentsSettingsPage() override;

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    void setRotationStrategy(RotationStrategy rotationStrategy);
    RotationStrategy rotationStrategy();

    Ui_UserAgentsSettings* m_ui;
};
