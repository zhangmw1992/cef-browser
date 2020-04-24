#include "client_app_browser.h"

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/wrapper/cef_helpers.h"

ClientAppBrowser::ClientAppBrowser()
{

}

void ClientAppBrowser::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
    // Pass additional command-line flags to the browser process.
    if (process_type.empty()) {
        command_line->AppendSwitchWithValue("renderer-process-limit", "1");

        // http https mixed
        command_line->AppendSwitch("allow-running-insecure-content");

        // proxy
        command_line->AppendSwitch("no-proxy-server");

        // security
        command_line->AppendSwitch("disable-web-security");

        // media
        command_line->AppendSwitchWithValue("enable-media-stream", "1");
    }
}

void ClientAppBrowser::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();

    // create all browser delegates
    CreateDelegates(delegates_);

    DelegateSet::iterator it = delegates_.begin();
    for (; it != delegates_.end(); ++it)
        (*it)->OnContextInitialized(this);
}

void ClientAppBrowser::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
    DelegateSet::iterator it = delegates_.begin();
    for (; it != delegates_.end(); ++it)
        (*it)->OnBeforeChildProcessLaunch(this, command_line);
}

void ClientAppBrowser::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
    CEF_REQUIRE_IO_THREAD();

    DelegateSet::iterator it = delegates_.begin();
    for (; it != delegates_.end(); ++it)
        (*it)->OnRenderProcessThreadCreated(this, extra_info);
}
