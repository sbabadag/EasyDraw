
#pragma once

#include <QMainWindow>
#include "occview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void createMenus();
    void createToolBars();

private:
    OccView* m_occView;
};