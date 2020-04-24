#include "mainwindow.h"

#include <QHBoxLayout>
#include <QCoreApplication>
#include <qdir.h>
#include <qpushbutton.h>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    resize(960, 640);

    QDir dir = QCoreApplication::applicationDirPath();

#ifdef Q_OS_WIN
    QString url = "file:\\" + QDir::toNativeSeparators(dir.filePath("web\\test.html"));
#else
    QString url = "file" + QDir::toNativeSeparators(dir.filePath("web/test.html"));
#endif

    view = new WebView(url, this);
    connect(view, &WebView::invokeMethodNotify, this, [&](int browserId, int frameId, const QString& method, const QVariantList& arguments) {
        if (method == "requestClose") {
            this->close();
        }
    });

    QPushButton* btn = new QPushButton("Clicked");
    btn->setFixedHeight(30);

    connect(btn, &QPushButton::clicked, this, [&]() {
        view->runJsFunctions(QString("setInteractiveLog('%1')").arg("123"));
    });

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(view);
    layout->addWidget(btn);
    layout->setContentsMargins(0, 0, 0, 0);
}

MainWindow::~MainWindow()
{

}
