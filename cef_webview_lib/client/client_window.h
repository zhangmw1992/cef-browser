#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QWindow>
#include <QVariant>

#include "client_handler.h"

class ClientWindow : public QWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWindow* parent = nullptr);
    ~ClientWindow();

    void setBrowserWindow(CefWindowHandle wnd);

signals:
    void loadStart();
    void loadEnd(int httpStatusCode);
    void loadError(int errorCode, const QString& errorMsg, const QString& failedUrl);

    void invokeMethodNotify(int browserId, int frameId, const QString method, const QVariantList arguments);

protected:
    virtual void exposeEvent(QExposeEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

    void synBrowserWindow();

private:
    CefWindowHandle hwndCefBrowser_;
};

#endif // CLIENTWINDOW_H
