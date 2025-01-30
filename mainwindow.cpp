#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_occView = new OccView(this);
    setCentralWidget(m_occView);

    createMenus();
    createToolBars();

    setWindowTitle(tr("Qt OpenCASCADE Viewer"));
    resize(1024, 768);
}

MainWindow::~MainWindow()
{
}

void MainWindow::createMenus()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    
    viewMenu->addAction(tr("&Fit All"), m_occView, &OccView::fitAll);
}

void MainWindow::createToolBars()
{
    QToolBar *viewToolBar = addToolBar(tr("View"));
    viewToolBar->addAction(tr("Fit All"), m_occView, &OccView::fitAll);
}
