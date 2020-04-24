#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <list>
#include <mutex>

#include "include/cef_client.h"
#include "include/wrapper/cef_message_router.h"

class ClientWindow;

// Client handler abstract base class. Provides common functionality shared by
// all concrete client handler implementations.
class ClientHandler : public CefClient
                    , public CefContextMenuHandler
                    , public CefDisplayHandler
                    , public CefLifeSpanHandler
                    , public CefLoadHandler
                    , public CefDownloadHandler
{
public:
    enum {
        MAIN_FRAME = (int64_t)0,
        ALL_FRAMES = (int64_t)-1,
    };

    // Constructor may be called on any thread.
    // |delegate| must outlive this object or DetachDelegate() must be called.
    ClientHandler(ClientWindow *pWin);

    // CefClient methods
    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE { return this; }

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() OVERRIDE { return this; }

    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) OVERRIDE;

    // CefContextMenuHandler methods
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model) OVERRIDE;

    virtual bool OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefContextMenuParams> params,
                                      int command_id,
                                      EventFlags event_flags) OVERRIDE;

    // CefDisplayHandler methods
    virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 const CefString& url) OVERRIDE;

    virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                               const CefString& title) OVERRIDE;

    virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                  cef_log_severity_t level,
                                  const CefString& message,
                                  const CefString& source, int line) OVERRIDE;

    // CefLifeSpanHandler methods
    virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               const CefString& target_url,
                               const CefString& target_frame_name,
                               CefLifeSpanHandler::WindowOpenDisposition target_disposition,
                               bool user_gesture,
                               const CefPopupFeatures& popupFeatures,
                               CefWindowInfo& windowInfo,
                               CefRefPtr<CefClient>& client,
                               CefBrowserSettings& settings,
                               CefRefPtr<CefDictionaryValue>& extra_info,
                               bool* no_javascript_access) OVERRIDE;

    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefLoadHandler methods
    void OnLoadError(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     ErrorCode errorCode,
                     const CefString& errorText,
                     const CefString& failedUrl) OVERRIDE;

    void OnLoadStart(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     TransitionType transition_type) OVERRIDE;

    void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                   CefRefPtr<CefFrame> frame,
                   int httpStatusCode) OVERRIDE;

    // CefDownloadHandler methods
    void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefDownloadItem> download_item,
                           CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE;

    void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefDownloadItem> download_item,
                          const CefString& suggested_name,
                          CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE;

    CefRefPtr<CefBrowser> GetBrowser() const;

    // Request that all existing browser windows close.
    void CloseAllBrowsers(bool force_close);

    bool SetCookie(const CefString& name,
                   const CefString& value,
                   const CefString& domain,
                   const CefString& path);

    bool DeleteCookies();

private:
    void CloseAllPopupBrowsers(bool force_close);

    bool DispatchNotifyRequest(CefRefPtr<CefBrowser> browser,
                               CefProcessId source_process,
                               CefRefPtr<CefProcessMessage> message);

private:
    ClientWindow *pClient_Window_;

    // The current number of browsers using this handler.
    int browser_count_;

    bool is_closing_;

    mutable std::mutex	mtx_;

    CefRefPtr<CefBrowser> main_browser_;

    // Handles the browser side of query routing. The renderer side is handled
    CefRefPtr<CefMessageRouterBrowserSide> message_router_;

    // List of existing browser windows. Only accessed on the CEF UI thread.
    std::list<CefRefPtr<CefBrowser>> popup_browser_list_;

    IMPLEMENT_REFCOUNTING(ClientHandler);
};

#endif // CLIENTHANDLER_H
