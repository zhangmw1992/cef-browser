#ifndef CLIENTAPPBROWSER_H
#define CLIENTAPPBROWSER_H

#include <set>

#include "include/cef_app.h"

class ClientAppBrowser : public CefApp
                       , public CefBrowserProcessHandler
{
public:
    // Interface for browser delegates. All Delegates must be returned via
    // CreateDelegates. Do not perform work in the Delegate
    // constructor. See CefBrowserProcessHandler for documentation.
    class Delegate : public virtual CefBaseRefCounted {
    public:
        virtual void OnContextInitialized(CefRefPtr<ClientAppBrowser> app) {}

        virtual void OnBeforeChildProcessLaunch(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefCommandLine> command_line) {}

        virtual void OnRenderProcessThreadCreated(CefRefPtr<ClientAppBrowser> app, CefRefPtr<CefListValue> extra_info) {}
    };

    typedef std::set<CefRefPtr<Delegate>> DelegateSet;

    ClientAppBrowser();

private:
    // Creates all of the Delegate objects
    static void CreateDelegates(DelegateSet& delegates);

    // CefApp methods.
    virtual void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE;
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() OVERRIDE { return this; }

    // CefBrowserProcessHandler methods.
    virtual void OnContextInitialized() OVERRIDE;
    virtual void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) OVERRIDE;
    virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) OVERRIDE;

private:
    // Set of supported BrowserDelegates. Only used in the browser process.
    DelegateSet delegates_;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(ClientAppBrowser);
};

#endif // CLIENTAPPBROWSER_H
