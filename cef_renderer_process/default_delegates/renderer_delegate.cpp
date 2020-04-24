#include "renderer_delegate.h"

#include "../web_define.h"

namespace DefaultDelegate
{
    void CreateBrowserDelegate(ClientRenderer::RendererDelegateSet& delegates)
    {
        delegates.insert(new RendererDelegate());
    }

    RendererDelegate::RendererDelegate()
    {

    }

    void RendererDelegate::OnWebKitInitialized(CefRefPtr<ClientRenderer> app)
    {
        CefMessageRouterConfig config;
        render_message_router_ = CefMessageRouterRendererSide::Create(config);
    }

    void RendererDelegate::OnContextCreated(CefRefPtr<ClientRenderer> app,
                                            CefRefPtr<CefBrowser> browser,
                                            CefRefPtr<CefFrame> frame,
                                            CefRefPtr<CefV8Context> context)
    {
        render_message_router_->OnContextCreated(browser, frame, context);

        int64 frameId = frame->GetIdentifier();
        auto it = frame_id_to_client_map_.find(frameId);

        if (it == frame_id_to_client_map_.end()) {
            // create and insert the CefClient Object into this frame.window object
            CefRefPtr<CefV8Value> objWindow = context->GetGlobal();
            CefRefPtr<Client> objClient = new Client(browser, frame);
            objWindow->SetValue(CEF_OBJECT_NAME, objClient->GetObject(), V8_PROPERTY_ATTRIBUTE_READONLY);
            frame_id_to_client_map_[frameId] = objClient;
        }
    }

    void RendererDelegate::OnContextReleased(CefRefPtr<ClientRenderer> app,
                                             CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             CefRefPtr<CefV8Context> context)
    {
        render_message_router_->OnContextReleased(browser, frame, context);

        int64 frameId = frame->GetIdentifier();
        auto it = frame_id_to_client_map_.find(frameId);

        if (it != frame_id_to_client_map_.end()) {
            frame_id_to_client_map_.erase(it);
        }
    }

    bool RendererDelegate::OnProcessMessageReceived(CefRefPtr<ClientRenderer> app,
                                                    CefRefPtr<CefBrowser> browser,
                                                    CefRefPtr<CefFrame> frame,
                                                    CefProcessId source_process,
                                                    CefRefPtr<CefProcessMessage> message)
    {
        if (render_message_router_->OnProcessMessageReceived(browser, frame, source_process, message)) {
            return true;
        }

        if (OnTriggerEventNotifyMessage(browser, frame, source_process, message)) {
            return true;
        }

        return false;
    }

    bool RendererDelegate::OnTriggerEventNotifyMessage(CefRefPtr<CefBrowser> browser,
                                                       CefRefPtr<CefFrame> frame,
                                                       CefProcessId source_process,
                                                       CefRefPtr<CefProcessMessage> message)
    {
        if (message->GetName() == TRIGGEREVENT_NOTIFY_MESSAGE) {
            CefRefPtr<CefListValue> messageArguments = message->GetArgumentList();
            if (messageArguments && (messageArguments->GetSize() >= 2)) {
                int idx = 0;
                if (CefValueType::VTYPE_STRING == messageArguments->GetType(idx)) {
                    CefString eventName = messageArguments->GetString(idx++);

                    if (CefValueType::VTYPE_DICTIONARY == messageArguments->GetType(idx)) {
                        CefRefPtr<CefDictionaryValue> dict = messageArguments->GetDictionary(idx++);

                    //	ExecuteEventListener(browser, frame, eventName, dict);
                        return true;
                    }
                }
            }
        }
        return false;
    }
}

