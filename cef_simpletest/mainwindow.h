#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "web_view.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    WebView* view;
};
#endif // MAINWINDOW_H
