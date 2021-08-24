#pragma once

#include <QList>
#include <QWidget>

class QAction;

/// Sidebar widget with user defined actions
/// Based on https://stackoverflow.com/a/44327462
class SideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarWidget ( QWidget *parent = nullptr );
    /// Adds an action to sidebar
    void addAction ( QAction* action, bool selected = false );
    /// Returns pointer to a created action from given text and icon
    QAction* addAction ( const QString& text, const QIcon& icon );
    QSize minimumSizeHint() const override;
    void setCurrentAction(int index);

Q_SIGNALS:
    void currentActionIndexChanged(int index);

protected:
    void paintEvent ( QPaintEvent *event ) override;
    void mouseMoveEvent ( QMouseEvent *event ) override;
    void mousePressEvent ( QMouseEvent * event ) override;
    void leaveEvent ( QEvent *event ) override;

    /**
     * Get the document with the URL \p url.
     * if multiple documents match the searched url, return the first found one...
     * \param url the document's URL
     * \return the document with the given \p url or NULL, if none found
     */
    QAction* actionAt ( const QPoint& point ) const;

private:
    QList<QAction*> m_actions;
    QAction* m_checkedAction;
    QAction* m_hoveredAction;
};



