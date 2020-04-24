#include "web_view.h"

#include <QGridLayout>
#include <QPointer>

#include "./client/client_handler.h"
#include "./client/client_window.h"

class WebView::Implementation
{
public:
    explicit Implementation(const QString& url, QWindow* parent)
        : pClientWindow_(nullptr)
        , pClientHandler_(nullptr)
    {
        // Here we must create a QWidget as a wrapper to encapsulate the QWindow
        pClientWindow_ = new ClientWindow(parent);
        pClientWindow_->create();

        // Set window info
        CefWindowInfo window_info;

#ifdef Q_OS_WIN
        RECT rc = { 0 };
        window_info.SetAsChild((CefWindowHandle)pClientWindow_->winId(), rc);
#elif Q_OS_MAC
        window_info.SetAsChild((CefWindowHandle)pClientWindow_->winId(), 0, 0, 960, 640);
#endif

        CefBrowserSettings browserSettings;
        browserSettings.plugins = STATE_DEFAULT;	// disable all plugins

        pClientHandler_ = new ClientHandler(pClientWindow_);

        // Create the main browser window.
        CefBrowserHost::CreateBrowser(window_info, pClientHandler_, url.toStdString(),	browserSettings, nullptr, CefRequestContext::GetGlobalContext());
    }

    ~Implementation()
    {
        if (pClientHandler_) {
            // Wait for all browser to close
            pClientHandler_->CloseAllBrowsers(true);
            pClientHandler_ = nullptr;
        }
    }

    ClientWindow* cefWindow()
    {
        return pClientWindow_;
    }

    void navigateToUrl(const QString& url)
    {
        if (pClientHandler_) {
            CefString strUrl;
            strUrl.FromString(url.toStdString());

            pClientHandler_->GetBrowser()->GetMainFrame()->LoadURL(strUrl);
        }
    }

    void runJsFunctions(const QString &data)
    {
        if (pClientHandler_) {
            CefRefPtr<CefBrowser> browser = pClientHandler_->GetBrowser();
            if (browser) {
                CefRefPtr<CefFrame> frame = browser->GetMainFrame();
<<<<<<< HEAD
                if (frame)
                    frame->ExecuteJavaScript(CefString(data.toStdString()), "", 0);
=======
                if (frame) {
                    frame->ExecuteJavaScript(CefString(data.toStdString()), "", 0);
                }
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7
            }
        }
    }

    bool setCookie(const QString &name, const QString &value, const QString &domain, const QString &path)
    {
        if (pClientHandler_) {
            CefString _name;
            CefString _value;
            CefString _domain;
            CefString _path;

            _name.FromString(name.toStdString());
            _value.FromString(value.toStdString());
            _domain.FromString(domain.toStdString());
            _path.FromString(path.toStdString());

            return pClientHandler_->SetCookie(_name, _value, _domain, _path);
        }

        return false;
    }

    bool deleteCookies()
    {
        if (pClientHandler_) {
            return pClientHandler_->DeleteCookies();
        }

        return false;
    }

    void reload()
    {
        if (pClientHandler_) {
            CefRefPtr<CefBrowser> browser = pClientHandler_->GetBrowser();
            if (browser) {
                browser->Reload();
            }
        }
    }

    void stopLoad()
    {
        if (pClientHandler_) {
            CefRefPtr<CefBrowser> browser = pClientHandler_->GetBrowser();
            if (browser) {
                browser->StopLoad();
            }
        }
    }

    void notifyMoveOrResizeStarted()
    {
        if (pClientHandler_) {
            CefRefPtr<CefBrowser> browser = pClientHandler_->GetBrowser();
            if (browser) {
                CefRefPtr<CefBrowserHost> host = browser->GetHost();
                if (host)
                    host->NotifyMoveOrResizeStarted();
            }
        }
    }

    void onToplevelWidgetMoveOrResize()
    {
        notifyMoveOrResizeStarted();
    }

private:
    QPointer<ClientWindow>   pClientWindow_;
    CefRefPtr<ClientHandler> pClientHandler_;
};

WebView::WebView(const QString url, QWidget* parent)
    : QWidget(parent)
    , pImpl_(nullptr)
{
    pImpl_ = std::make_unique<Implementation>(url, windowHandle());

    QWidget* windowContainer = createWindowContainer(pImpl_->cefWindow(), this);

    QGridLayout* layout = new QGridLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(windowContainer);

    connect(pImpl_->cefWindow(), &ClientWindow::loadStart, this, &WebView::loadStart);
    connect(pImpl_->cefWindow(), &ClientWindow::loadError, this, &WebView::loadError);
    connect(pImpl_->cefWindow(), &ClientWindow::loadEnd, this, &WebView::loadEnd);
    connect(pImpl_->cefWindow(), &ClientWindow::invokeMethodNotify, this, &WebView::invokeMethodNotify);

    // If we're already part of a window, we'll install our event handler
    // If our parent changes later, this will be handled in WebView::changeEvent()
    if (this->window())
        this->window()->installEventFilter(this);
}

WebView::~WebView()
{
    if (pImpl_) {
        pImpl_ = nullptr;
    }
}

void WebView::navigateToUrl(const QString& url)
{
    if (pImpl_) {
        pImpl_->navigateToUrl(url);
    }
}

void WebView::runJsFunctions(const QString &data)
{
    if (pImpl_) {
        pImpl_->runJsFunctions(data);
    }
}

bool WebView::setCookie(const QString &name, const QString &value, const QString &domain, const QString &path)
{
    if (pImpl_) {
        return pImpl_->setCookie(name, value, domain, path);
    }

    return false;
}

bool WebView::deleteCookies()
{
    if (pImpl_) {
        return pImpl_->deleteCookies();
    }

    return false;
}

void WebView::reload()
{
    if (pImpl_) {
        pImpl_->reload();
    }
}

void WebView::stopLoad()
{
    if (pImpl_) {
        pImpl_->stopLoad();
    }
}

void WebView::changeEvent(QEvent * event)
{
    if (QEvent::ParentAboutToChange == event->type())
    {
        if (this->window())
            this->window()->removeEventFilter(this);
    }
    else if (QEvent::ParentChange == event->type())
    {
        if (this->window())
            this->window()->installEventFilter(this);
    }

    QWidget::changeEvent(event);
}

bool WebView::eventFilter(QObject *watched, QEvent *event)
{
    if (pImpl_ && watched == this->window())
    {
        if (QEvent::Resize == event->type() || QEvent::Move == event->type())
            pImpl_->onToplevelWidgetMoveOrResize();
    }

    return QWidget::eventFilter(watched, event);
}
