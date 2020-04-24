#ifndef RENDERERDELEGATE_H
#define RENDERERDELEGATE_H

#include <unordered_map>

#include "include/wrapper/cef_message_router.h"

#include "client.h"
#include "../client_renderer.h"

namespace DefaultDelegate
{
    void CreateBrowserDelegate(ClientRenderer::RendererDelegateSet& delegates);

    class RendererDelegate : public ClientRenderer::RendererDelegate
    {
        typedef std::unordered_map<int64, CefRefPtr<Client>> FrameID2ClientMap;

    public:
        RendererDelegate();

        virtual void OnWebKitInitialized(CefRefPtr<ClientRenderer> app);

        virtual void OnContextCreated(CefRefPtr<ClientRenderer> app,
                                      CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefV8Context> context);

        virtual void OnContextReleased(CefRefPtr<ClientRenderer> app,
                                       CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefV8Context> context);

        virtual bool OnProcessMessageReceived(CefRefPtr<ClientRenderer> app,
                                              CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message);

    protected:
        bool OnTriggerEventNotifyMessage(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefProcessId source_process,
                                         CefRefPtr<CefProcessMessage> message);

    private:
        CefRefPtr<CefMessageRouterRendererSide> render_message_router_;

        FrameID2ClientMap frame_id_to_client_map_;

    private:
        IMPLEMENT_REFCOUNTING(RendererDelegate);
    };
}

#endif // RENDERERDELEGATE_H
