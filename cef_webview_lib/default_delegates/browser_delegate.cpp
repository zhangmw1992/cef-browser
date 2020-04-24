#include "browser_delegate.h"

namespace DefaultDelegate
{
    void CreateBrowserDelegate(ClientAppBrowser::DelegateSet& delegates)
    {
        delegates.insert(new BrowserDelegate());
    }

    BrowserDelegate::BrowserDelegate()
    {

    }

    void BrowserDelegate::OnContextInitialized(CefRefPtr<ClientAppBrowser> app)
    {

    }

    void BrowserDelegate::OnBeforeChildProcessLaunch(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefCommandLine> command_line)
    {

    }

    void BrowserDelegate::OnRenderProcessThreadCreated(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefListValue> extra_info)
    {

    }
}
