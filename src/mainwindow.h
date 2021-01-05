#pragma once

#include <QMainWindow>

class QAction;
class QLabel;
// class QList<float>;
class QMenu;
class QProgressBar;
class QPushButton;
class QSpinBox;
class QStandardItemModel;
class QStatusBar;
class QTableView;
class QToolBar;
class QHBoxLayout;
class QVBoxLayout;
class QWidget;

class Table;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void importUrls();
    void exportResults();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

    void centerWindow();

private:
    void createActions();
    void createMenuBar();
    void createToolBar();
    void createWidgets();
    void createStatusBar();
    void createConnections();

    // Actions
    QAction *m_importUrlsAction;
    QAction *m_exportResultsAction;
    QAction *m_quitAction;
    QAction *m_clearTableAction;
    QAction *m_removeDuplicatesAction;
    QAction *m_selectAllAction;
    QAction *m_removeSelectedAction;
    QAction *m_invertSelectionAction;
    QAction *m_centerWindowAction;
    QAction *m_aboutAction;

    // MenuBar
    QMenu *m_fileMenu;
    QMenu *m_editMenu;
    QMenu *m_selectionMenu;
    QMenu *m_windowMenu;
    QMenu *m_helpMenu;

    // ToolBar
    QToolBar *m_toolBar;

    QWidget *m_mainWidget;
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_bottomLayout;
//     QTableView *m_resultsTableView;
    Table *m_resultsTable;
    QList<float> m_columnRatios;
    QLabel *m_threadsLabel;
    QLabel *m_timeoutLabel;
    QSpinBox *m_threadsSpinBox;
    QSpinBox *m_timeoutSpinBox;
    QPushButton *m_startPushButton;
    QPushButton *m_stopPushButton;
    QProgressBar *m_progressBar;
    QStatusBar *m_statusBar;
    QLabel *m_activeThreadsLabel;

    QStandardItemModel *m_resultsModel;
};
