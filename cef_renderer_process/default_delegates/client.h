#ifndef CLIENT_H
#define CLIENT_H

#include <map>
#include <list>

#include "include/cef_v8.h"

class Client : public CefBaseRefCounted
{
    class V8Handler : public CefV8Handler
    {
    public:
        V8Handler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);

        virtual bool Execute(const CefString& function,
                             CefRefPtr<CefV8Value> object,
                             const CefV8ValueList& arguments,
                             CefRefPtr<CefV8Value>& retval,
                             CefString& exception) override;

    protected:
        void ExecuteInvokeMethod(const CefString& function,
                                 CefRefPtr<CefV8Value> object,
                                 const CefV8ValueList& arguments,
                                 CefRefPtr<CefV8Value>& retval,
                                 CefString& exception);

    private:
        CefRefPtr<CefBrowser>	browser_;
        CefRefPtr<CefFrame>		frame_;

    private:
        IMPLEMENT_REFCOUNTING(V8Handler);
    };

public:
    Client(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame);

    CefRefPtr<CefV8Value> GetObject();

private:
    CefRefPtr<CefV8Value>	object_;
    CefRefPtr<CefBrowser>	browser_;
    CefRefPtr<CefFrame>		frame_;

private:
    IMPLEMENT_REFCOUNTING(Client);
};


#endif // CLIENT_H
