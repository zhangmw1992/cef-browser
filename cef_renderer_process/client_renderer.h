#ifndef CLIENTRENDERER_H
#define CLIENTRENDERER_H

#include <set>

#include "include/cef_app.h"

class ClientRenderer : public CefApp
                     , public CefRenderProcessHandler
{
public:
    // Interface for renderer delegates. All Delegates must be returned via
    // CreateDelegates. Do not perform work in the Delegate
    // constructor. See CefRenderProcessHandler for documentation.
    class RendererDelegate : public virtual CefBaseRefCounted
    {
    public:
        virtual void OnRenderThreadCreated(CefRefPtr<ClientRenderer> app,
                                           CefRefPtr<CefListValue> extra_info)
        {}

        virtual void OnWebKitInitialized(CefRefPtr<ClientRenderer> app)
        {}

        virtual void OnBrowserCreated(CefRefPtr<ClientRenderer> app,
                                      CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefDictionaryValue> extra_info)
        {}

        virtual void OnBrowserDestroyed(CefRefPtr<ClientRenderer> app,
                                        CefRefPtr<CefBrowser> browser)
        {}

        virtual CefRefPtr<CefLoadHandler> GetLoadHandler(CefRefPtr<ClientRenderer> app)
        {
            return nullptr;
        }

        virtual void OnContextCreated(CefRefPtr<ClientRenderer> app,
                                      CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefV8Context> context)
        {}

        virtual void OnContextReleased(CefRefPtr<ClientRenderer> app,
                                       CefRefPtr<CefBrowser> browser,
                                       CefRefPtr<CefFrame> frame,
                                       CefRefPtr<CefV8Context> context)
        {}

        virtual void OnUncaughtException(CefRefPtr<ClientRenderer> app,
                                         CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefV8Context> context,
                                         CefRefPtr<CefV8Exception> exception,
                                         CefRefPtr<CefV8StackTrace> stackTrace)
        {}

        virtual void OnFocusedNodeChanged(CefRefPtr<ClientRenderer> app,
                                          CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefRefPtr<CefDOMNode> node)
        {}

        virtual bool OnProcessMessageReceived(CefRefPtr<ClientRenderer> app,
                                              CefRefPtr<CefBrowser> browser,
                                              CefRefPtr<CefFrame> frame,
                                              CefProcessId source_process,
                                              CefRefPtr<CefProcessMessage> message)
        {
            return false;
        }
    };

    typedef std::set<CefRefPtr<RendererDelegate> > RendererDelegateSet;

    ClientRenderer();

private:
    // Creates all of the Delegate objects.
    static void CreateRendererDelegates(RendererDelegateSet& delegates);

    // CefApp methods.
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

    // CefRenderProcessHandler methods.
    virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) override;

    virtual void OnWebKitInitialized() override;

    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDictionaryValue> extra_info) override;

    virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override;

    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefV8Context> context) override;

    virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefV8Context> context) override;

    virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context,
                                     CefRefPtr<CefV8Exception> exception,
                                     CefRefPtr<CefV8StackTrace> stackTrace) override;

    virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefDOMNode> node) override;

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) override;

private:
    // Set of supported RenderDelegates. Only used in the renderer process.
    RendererDelegateSet render_delegates_;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(ClientRenderer);
};

#endif // CLIENTRENDERER_H
