#ifndef BROWSERDELEGATE_H
#define BROWSERDELEGATE_H

#include "include/wrapper/cef_message_router.h"

#include "../client/client_app_browser.h"

namespace DefaultDelegate
{
    void CreateBrowserDelegate(ClientAppBrowser::DelegateSet& delegates);

    class BrowserDelegate : public ClientAppBrowser::Delegate
    {
    public:
        BrowserDelegate();

        virtual void OnContextInitialized(CefRefPtr<ClientAppBrowser> app) OVERRIDE;

        virtual void OnBeforeChildProcessLaunch(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefCommandLine> command_line) OVERRIDE;

        virtual void OnRenderProcessThreadCreated(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefListValue> extra_info) OVERRIDE;

    private:
        IMPLEMENT_REFCOUNTING(Delegate);
    };
}


#endif // BROWSERDELEGATE_H
