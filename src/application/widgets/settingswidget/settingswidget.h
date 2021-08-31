#pragma once

#include <QMap>
#include <QWidget>

class QCheckBox;
class QGroupBox;
class QGridLayout;
class QLabel;
class QSpinBox;
class QHBoxLayout;
class QVBoxLayout;
class QListWidget;
class QListWidgetItem;
class QStackedWidget;

class SettingsWidget : public QWidget {
    Q_OBJECT
public:
    explicit SettingsWidget(QWidget* parent = nullptr);
    void addItem(const QIcon& icon, const QString& text, QWidget* pageWidget);
    int itemIndex(const QString& text) const;
    bool setCurrentSettingsPage(const QString& text);

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    QHBoxLayout* m_mainLayout;
    QListWidget* m_categoriesListWidget;
    QStackedWidget* m_contentsStackedWidget;
    QStringList m_items;
    QMap<QString, QListWidgetItem*> m_itemsMap;
};
