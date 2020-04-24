#include "client_renderer.h"

#include "./default_delegates/renderer_delegate.h"

#include "include/wrapper/cef_helpers.h"
#include "include/cef_browser.h"

ClientRenderer::ClientRenderer()
{

}

void ClientRenderer::CreateRendererDelegates(RendererDelegateSet& delegates)
{
    DefaultDelegate::CreateBrowserDelegate(delegates);
}

CefRefPtr<CefRenderProcessHandler> ClientRenderer::GetRenderProcessHandler()
{
    return this;
}

void ClientRenderer::OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info)
{
    CEF_REQUIRE_RENDERER_THREAD();

    CreateRendererDelegates(render_delegates_);

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnRenderThreadCreated(this, extra_info);
}

void ClientRenderer::OnWebKitInitialized()
{
    CEF_REQUIRE_RENDERER_THREAD();

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnWebKitInitialized(this);
}

void ClientRenderer::OnBrowserCreated(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefDictionaryValue> extra_info)
{
    CEF_REQUIRE_RENDERER_THREAD();

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnBrowserCreated(this, browser, extra_info);
}

void ClientRenderer::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_RENDERER_THREAD();

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnBrowserDestroyed(this, browser);
}

CefRefPtr<CefLoadHandler> ClientRenderer::GetLoadHandler()
{
    CefRefPtr<CefLoadHandler> load_handler;
    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end() && !load_handler.get(); ++it)
        load_handler = (*it)->GetLoadHandler(this);

    return load_handler;
}

void ClientRenderer::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefV8Context> context)
{
    CEF_REQUIRE_RENDERER_THREAD();

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnContextCreated(this, browser, frame, context);
}

void ClientRenderer::OnContextReleased(CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefV8Context> context)
{
    CEF_REQUIRE_RENDERER_THREAD();

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnContextReleased(this, browser, frame, context);
}

void ClientRenderer::OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefV8Context> context,
                                         CefRefPtr<CefV8Exception> exception,
                                         CefRefPtr<CefV8StackTrace> stackTrace)
{
    CEF_REQUIRE_RENDERER_THREAD();

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnUncaughtException(this, browser, frame, context, exception, stackTrace);
}

void ClientRenderer::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefDOMNode> node)
{
    CEF_REQUIRE_RENDERER_THREAD();

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end(); ++it)
        (*it)->OnFocusedNodeChanged(this, browser, frame, node);
}

bool ClientRenderer::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message)
{
    CEF_REQUIRE_RENDERER_THREAD();
    DCHECK_EQ(source_process, PID_BROWSER);

    bool handled = false;

    RendererDelegateSet::iterator it = render_delegates_.begin();
    for (; it != render_delegates_.end() && !handled; ++it)
        handled = (*it)->OnProcessMessageReceived(this, browser, frame, source_process, message);

    return handled;
}

