#include "client.h"

#include "../web_define.h"

Client::V8Handler::V8Handler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
    : browser_(browser)
    , frame_(frame)
{

}

bool Client::V8Handler::Execute(const CefString& function,
                                CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& exception)
{
    if (function == CEF_INVOKEMETHOD)
        ExecuteInvokeMethod(function, object, arguments, retval, exception);
    else
        return false;

    return true;
}

void Client::V8Handler::ExecuteInvokeMethod(const CefString& function,
                                            CefRefPtr<CefV8Value> object,
                                            const CefV8ValueList& arguments,
                                            CefRefPtr<CefV8Value>& retval,
                                            CefString& exception)
{
    CefRefPtr<CefProcessMessage> msg = CefProcessMessage::Create(INVOKEMETHOD_NOTIFY_MESSAGE);

    CefRefPtr<CefListValue> args = msg->GetArgumentList();
    int frameId = (int)frame_->GetIdentifier();

    int idx = 0;
    args->SetInt(idx++, frameId);
    args->SetString(idx++, function);

    for (std::size_t i = 0; i < arguments.size(); i++)
    {
        if (arguments[i]->IsBool())
            args->SetBool(idx++, arguments[i]->GetBoolValue());
        else if (arguments[i]->IsInt())
            args->SetInt(idx++, arguments[i]->GetIntValue());
        else if (arguments[i]->IsDouble())
            args->SetDouble(idx++, arguments[i]->GetDoubleValue());
        else if (arguments[i]->IsString())
            args->SetString(idx++, arguments[i]->GetStringValue());
        else
            args->SetNull(idx++);
    }

    bool bRet = false;
    if (frame_) {
        frame_->SendProcessMessage(PID_BROWSER, msg);
        bRet = true;
    }

    retval = CefV8Value::CreateUndefined();
}

Client::Client(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
    : object_(CefV8Value::CreateObject(nullptr, nullptr))
    , browser_(browser)
    , frame_(frame)
{
    // create function handler
    CefRefPtr<V8Handler> handler = new V8Handler(browser_, frame_);

    // create function function
    CefRefPtr<CefV8Value> funcInvokeMethod = CefV8Value::CreateFunction(CEF_INVOKEMETHOD, handler);
    // add this function to window object
    object_->SetValue(CEF_INVOKEMETHOD, funcInvokeMethod, V8_PROPERTY_ATTRIBUTE_READONLY);
}

CefRefPtr<CefV8Value> Client::GetObject()
{
    return object_;
}
