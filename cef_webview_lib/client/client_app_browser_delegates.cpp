#include "client_app_browser.h"

#include "../default_delegates/browser_delegate.h"

void ClientAppBrowser::CreateDelegates(DelegateSet& delegates)
{
    DefaultDelegate::CreateBrowserDelegate(delegates);
}
