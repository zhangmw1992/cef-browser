#include "client_window.h"
#include "client_manager.h"

ClientWindow::ClientWindow(QWindow* parent)
    : QWindow(parent)
    , hwndCefBrowser_(nullptr)
{
    setFlags(Qt::FramelessWindowHint);

    ClientManager::instance().initializeCef();
}

ClientWindow::~ClientWindow()
{
    if (hwndCefBrowser_)
        hwndCefBrowser_ = nullptr;

    ClientManager::instance().uninitializeCef();
}

void ClientWindow::setBrowserWindow(CefWindowHandle wnd)
{
    hwndCefBrowser_ = wnd;

    synBrowserWindow();
}

void ClientWindow::synBrowserWindow()
{
#ifdef Q_OS_WIN
    if (hwndCefBrowser_)
        ::MoveWindow(hwndCefBrowser_, 0, 0, width(), height(), TRUE);
#endif
}

void ClientWindow::exposeEvent(QExposeEvent* e)
{
    synBrowserWindow();

    return QWindow::exposeEvent(e);
}

void ClientWindow::resizeEvent(QResizeEvent* e)
{
    synBrowserWindow();

    return QWindow::resizeEvent(e);
}
