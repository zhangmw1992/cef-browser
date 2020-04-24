#include "client_handler.h"
#include "client_window.h"

#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

#include "../web_define.h"

#include <QVariant>

ClientHandler::ClientHandler(ClientWindow *pWin)
    : pClient_Window_(pWin)
    , browser_count_(0)
    , is_closing_(false)
    , main_browser_(nullptr)
    , message_router_(nullptr)
{

}

bool ClientHandler::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message)
{
    CEF_REQUIRE_UI_THREAD();

    if (message_router_ && message_router_->OnProcessMessageReceived(browser, frame, source_process, message))
        return true;

    if (DispatchNotifyRequest(browser, source_process, message))
        return true;

    return false;
}

void ClientHandler::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                        CefRefPtr<CefFrame> frame,
                                        CefRefPtr<CefContextMenuParams> params,
                                        CefRefPtr<CefMenuModel> model)
{
    CEF_REQUIRE_UI_THREAD();

    model->Clear();
}

bool ClientHandler::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                         CefRefPtr<CefFrame> frame,
                                         CefRefPtr<CefContextMenuParams> params,
                                         int command_id,
                                         EventFlags event_flags)
{
    CEF_REQUIRE_UI_THREAD();

    return false;
}

void ClientHandler::OnAddressChange(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    const CefString& url)
{
    CEF_REQUIRE_UI_THREAD();
}

void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser,
                                  const CefString& title)
{
    CEF_REQUIRE_UI_THREAD();
}

bool ClientHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                     cef_log_severity_t level,
                                     const CefString& message,
                                     const CefString& source, int line)
{
    CEF_REQUIRE_UI_THREAD();

    return false;
}

bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
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
                                  bool* no_javascript_access)
{
    CEF_REQUIRE_UI_THREAD();

    // If the browser is closing, block the popup
    if (is_closing_)
        return true;

    // Redirect all popup page into the source frame forcefully
    frame->LoadURL(target_url);

    // Don't allow new window or tab
    return true;
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    // If the browser is closing, return immediately to release the new created browser
    if (is_closing_)
        return;

    if (!message_router_) {
        // Create the browser-side router for query handling.
        CefMessageRouterConfig config;
        message_router_ = CefMessageRouterBrowserSide::Create(config);
    }

    if (!main_browser_) {
        // If this is the main browser then keep it
        {
            std::unique_lock<std::mutex> lock(mtx_);
            // We need to keep the main child window, but not popup windows
            main_browser_ = browser;
        }

         // Set the cef window handle to the ClientWindow
        if (pClient_Window_) {
            pClient_Window_->setBrowserWindow(browser->GetHost()->GetWindowHandle());
        }
    } else if (browser->IsPopup()) {
        // Add to the list of popup browsers.
        popup_browser_list_.push_back(browser);

        // Give focus to the popup browser. Perform asynchronously because the
        // parent window may attempt to keep focus after launching the popup.
        CefPostTask(TID_UI, CefCreateClosureTask(base::Bind(&CefBrowserHost::SetFocus, browser->GetHost().get(), true)));
    }

    // Increase the browser count
    {
        std::unique_lock<std::mutex> lock(mtx_);
        browser_count_++;
    }
}

bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    // Closing the main window requires special handling. See the DoClose()
    // documentation in the CEF header for a detailed description of this process.
    if (main_browser_ && main_browser_->IsSame(browser))
        // Set a flag to indicate that the window close should be allowed.
        is_closing_ = true;

    // Allow the close. For windowed browsers this will result in the OS close
    // event being sent.
    return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    if (message_router_) {
        message_router_->OnBeforeClose(browser);
    }

    if (main_browser_ && main_browser_->IsSame(browser)) {
        // if the main browser is closing, we need to close all the pop up browsers.
        if (!popup_browser_list_.empty()) {
            for (auto& popup : popup_browser_list_) {
                if (popup) {
                    popup->StopLoad();
                    popup->GetHost()->CloseBrowser(true);
                }
            }
        }

        main_browser_->StopLoad();
        main_browser_ = nullptr;
    } else if (browser->IsPopup()) {
        // Remove from the browser popup list.
        for (auto it = popup_browser_list_.begin(); it != popup_browser_list_.end(); ++it) {
            if ((*it)->IsSame(browser)) {
                popup_browser_list_.erase(it);
                break;
            }
        }
    }
}

void ClientHandler::OnLoadStart(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                TransitionType transition_type)
{
    CEF_REQUIRE_UI_THREAD();

    if (pClient_Window_) {
        pClient_Window_->loadStart();
    }
}

void ClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                              CefRefPtr<CefFrame> frame,
                              int httpStatusCode)
{
    CEF_REQUIRE_UI_THREAD();

    if (pClient_Window_)
        pClient_Window_->loadEnd(httpStatusCode);
}

void ClientHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                ErrorCode errorCode,
                                const CefString& errorText,
                                const CefString& failedUrl)
{
    CEF_REQUIRE_UI_THREAD();

    if (errorCode == ERR_ABORTED)
        return;

    QString msg = QString::fromStdString(errorText.ToString());
    QString url = QString::fromStdString(failedUrl.ToString());

    if (pClient_Window_)
        pClient_Window_->loadError(errorCode, msg, url);
}

void ClientHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefDownloadItem> download_item,
                                      CefRefPtr<CefDownloadItemCallback> callback)
{
    CEF_REQUIRE_UI_THREAD();
}

void ClientHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefDownloadItem> download_item,
                                     const CefString &suggested_name,
                                     CefRefPtr<CefBeforeDownloadCallback> callback)
{
    CEF_REQUIRE_UI_THREAD();
}

CefRefPtr<CefBrowser> ClientHandler::GetBrowser() const
{
    std::lock_guard<std::mutex> lock_scope(mtx_);

    return main_browser_;
}

void ClientHandler::CloseAllBrowsers(bool force_close)
{
    // If all browsers had been closed, then return
    std::unique_lock<std::mutex> lock(mtx_);
    if (!browser_count_) {
        return;
    }

    // Flip the closing flag
    is_closing_ = true;

    // Close all popup browsers if any
    if (!popup_browser_list_.empty()) {
        for (auto it = popup_browser_list_.begin(); it != popup_browser_list_.end(); ++it)
            (*it)->GetHost()->CloseBrowser(force_close);
    }

    if (main_browser_)
        // Request that the main browser close.
        main_browser_->GetHost()->CloseBrowser(force_close);
}

bool ClientHandler::DispatchNotifyRequest(CefRefPtr<CefBrowser> browser,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message)
{
    if (message->GetName() != INVOKEMETHOD_NOTIFY_MESSAGE) {
        return false;
    }
<<<<<<< HEAD

    CefRefPtr<CefListValue> messageArguments = message->GetArgumentList();
    if (messageArguments && (messageArguments->GetSize() < 2)) {
=======
   
    CefRefPtr<CefListValue> messageArguments = message->GetArgumentList();
    if (!messageArguments && (messageArguments->GetSize() < 2)) {
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7
        return false;
    }

    size_t idx = 0;
    if (CefValueType::VTYPE_INT != messageArguments->GetType(idx)) {
        return false;
    }

    int browserId = browser->GetIdentifier();
    int frameId = messageArguments->GetInt(idx++);

    if (CefValueType::VTYPE_STRING != messageArguments->GetType(idx)) {
        return false;
    }

    CefString functionName = messageArguments->GetString(idx++);
    if (functionName != CEF_INVOKEMETHOD) {
        return false;
    }

    std::string method;
    if (CefValueType::VTYPE_STRING == messageArguments->GetType(idx)) {
        method = messageArguments->GetString(idx++).ToString();
    }

    QVariantList arguments;
    for (; idx < messageArguments->GetSize(); idx++) {
        if (CefValueType::VTYPE_NULL == messageArguments->GetType(idx))
            arguments += "0";
        else if (CefValueType::VTYPE_BOOL == messageArguments->GetType(idx))
            arguments.push_back(QVariant::fromValue(messageArguments->GetBool(idx)));
        else if (CefValueType::VTYPE_INT == messageArguments->GetType(idx))
            arguments.push_back(QVariant::fromValue(messageArguments->GetInt(idx)));
        else if (CefValueType::VTYPE_DOUBLE == messageArguments->GetType(idx))
            arguments.push_back(QVariant::fromValue(messageArguments->GetDouble(idx)));
        else if (CefValueType::VTYPE_STRING == messageArguments->GetType(idx))
            arguments.push_back(QVariant::fromValue(QString::fromStdString(messageArguments->GetString(idx).ToString())));
    }

    if (pClient_Window_) {
        pClient_Window_->invokeMethodNotify(browserId, frameId, QString::fromStdString(method), arguments);
        return true;
    }

    return false;
}

bool ClientHandler::DeleteCookies()
{
    CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);
    CefRefPtr<CefDeleteCookiesCallback> deleteCookieCallback;

    return CefPostTask(TID_IO, CefCreateClosureTask(base::Bind(base::IgnoreResult(&CefCookieManager::DeleteCookies), manager, CefString(""), CefString(""), deleteCookieCallback)));
}

bool ClientHandler::SetCookie(const CefString& name,
                              const CefString& value,
                              const CefString& domain,
                              const CefString& path)
{
    CefRefPtr<CefCookieManager> manager = CefCookieManager::GetGlobalManager(nullptr);

    CefCookie cookie;
    CefString(&cookie.name) = name;
    CefString(&cookie.value) = value;
    CefString(&cookie.domain) = domain;
    CefString(&cookie.path) = path;

    cookie.has_expires = false;
    CefRefPtr<CefSetCookieCallback> setCookieCallback;

    return CefPostTask(TID_IO, CefCreateClosureTask(base::Bind(base::IgnoreResult(&CefCookieManager::SetCookie), manager, L"", cookie, setCookieCallback)));

}
