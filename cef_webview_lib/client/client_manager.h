#ifndef CLIENTMANAGER_H
#define CLIENTMANAGER_H

#include <QTimer>

#include "client_app_browser.h"

class ClientManager : public QObject
{
    Q_OBJECT

public:
    static ClientManager& instance();

    void initializeCef();
    void uninitializeCef();

private:
    explicit ClientManager(QObject *parent = nullptr);

private:
    CefRefPtr<ClientAppBrowser> m_clientAppBrowser;

    CefSettings m_cefSettings;

    int64_t m_browserRefCount;

    QTimer timer;
};

#endif // CLIENTMANAGER_H
