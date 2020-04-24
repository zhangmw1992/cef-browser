#include "client_manager.h"

#ifdef Q_OS_MAC
#include "include/wrapper/cef_library_loader.h"
#else
#include <qcoreapplication.h>
#include <qdir.h>

#include "../web_define.h"
#endif // Q_OS_MAC

ClientManager::ClientManager(QObject *parent)
    : QObject(parent)
    , m_browserRefCount(0)
{

}

ClientManager& ClientManager::instance()
{
    static ClientManager s_instance;
    return s_instance;
}

void ClientManager::initializeCef()
{
    // This is not the first time initialization
    if (++m_browserRefCount > 1)
        return;

#ifdef Q_OS_MAC
    CefScopedLibraryLoader library_loader;
    if (!library_loader.LoadInHelper())
        return;

    connect(&timer, &QTimer::timeout, this, []() { CefDoMessageLoopWork(); });
    timer.start(10);
#endif

#ifdef Q_OS_WIN
    // Enable High-DPI support on Windows 7 or newer.
    CefEnableHighDPISupport();

    QDir ExeDir = QCoreApplication::applicationDirPath();
    QString strExePath = ExeDir.filePath(RENDER_PROCESS_NAME);
    QString strResPath = ExeDir.filePath(RESOURCE_DIRECTORY_NAME);

    QDir LocDir(strResPath);
    QString strLocPath = LocDir.filePath(LOCALES_DIRECTORY_NAME);

    CefString(&m_cefSettings.browser_subprocess_path) = QDir::toNativeSeparators(strExePath).toStdString();
    CefString(&m_cefSettings.resources_dir_path) = QDir::toNativeSeparators(strResPath).toStdString();
    CefString(&m_cefSettings.locales_dir_path) = QDir::toNativeSeparators(strLocPath).toStdString();
    CefString(&m_cefSettings.user_agent) = QString(CEF_USER_AGENT).toStdString();

    m_cefSettings.multi_threaded_message_loop = true;
#endif

    m_cefSettings.no_sandbox = true;

#ifndef NDEBUG
    m_cefSettings.log_severity = LOGSEVERITY_DEFAULT;
    m_cefSettings.remote_debugging_port = 7777;
#else
    m_cefSettings.log_severity = LOGSEVERITY_DISABLE;
#endif

    m_clientAppBrowser = new ClientAppBrowser();

    CefMainArgs main_args;

    // Initialize CEF.
    CefInitialize(main_args, m_cefSettings, m_clientAppBrowser, nullptr);
}

void ClientManager::uninitializeCef()
{
    // This is not the last time release
    if (--m_browserRefCount > 0)
        return;

#ifdef Q_OS_MAC
    timer.stop();
#endif

    // The last time release
    // TO-DO (sheen) when we reach here, it is possible there are pending
    // IO requests, and they will fire the DCHECK when complete or aborted
    CefShutdown();

    // Destroy the application
    m_clientAppBrowser = nullptr;
}
