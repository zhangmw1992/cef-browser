#include "mainwindow.h"
<<<<<<< HEAD
#include "web_view.h"
=======
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7

#include <QHBoxLayout>
#include <QCoreApplication>
#include <qdir.h>
#include <qpushbutton.h>

<<<<<<< HEAD
MainWindow::MainWindow(QWidget* parent)
=======
MainWindow::MainWindow(QWidget *parent)
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7
    : QWidget(parent)
{
    resize(960, 640);

    QDir dir = QCoreApplication::applicationDirPath();
<<<<<<< HEAD
    QString url = "file:\\" + QDir::toNativeSeparators(dir.filePath("web\\test.html"));
=======
    QString url = QDir::toNativeSeparators(dir.filePath("web\\test.html"));
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7

    view = new WebView(url, this);
    connect(view, &WebView::invokeMethodNotify, this, [&](int browserId, int frameId, const QString& method, const QVariantList& arguments) {
        if (method == "requestClose") {
            this->close();
        }
<<<<<<< HEAD
        });
=======
    });
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7

    QPushButton* btn = new QPushButton("Clicked");
    btn->setFixedHeight(30);

    connect(btn, &QPushButton::clicked, this, [&]() {
        view->runJsFunctions(QString("setInteractiveLog('%1')").arg("123"));
<<<<<<< HEAD
        });

    QHBoxLayout* layout = new QHBoxLayout(this);
=======
    });

    QHBoxLayout *layout = new QHBoxLayout(this);
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7
    layout->addWidget(view);
    layout->addWidget(btn);
    layout->setContentsMargins(0, 0, 0, 0);
}

MainWindow::~MainWindow()
{

}
