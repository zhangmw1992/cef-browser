#ifndef WEB_DEFINE_H
#define WEB_DEFINE_H

#define RENDER_PROCESS_NAME                 ("cef_renderer.exe")
#define RESOURCE_DIRECTORY_NAME             ("resources")
#define LOCALES_DIRECTORY_NAME              ("locales")

#define CEF_USER_AGENT					    ("Mozilla/5.0 (Windows NT 6.2;WOW64) AppleWebKit/537.36 (KHTML, like Gecko) 2623 Chrome/79.0.3945.88 Safari/537.36")

#define CEF_OBJECT_NAME				        ("CefClient")

/*
* CefClient.invokeMethod("method_name", ...)
*/
#define CEF_INVOKEMETHOD				    ("invokeMethod")

/*
* this message is send from render process to browser process
* and is processed in the Qt UI thread
*
* format:
*		msg.name:
*		msg.arg[0]: frame id
*		msg.arg[1]: function name
*		msg.arg[2~...]: function parameters
*/
#define INVOKEMETHOD_NOTIFY_MESSAGE		    ("CefClient#InvokeMethodNotify")

/*
* this message is send from browser process to render process
* and is processed in the CefRenderer_Main thread
*
* format:
*		msg.name:
*		msg.arg[0]: frame id
*		msg.arg[1]: function name
*		msg.arg[2~...]: function parameters
*
*/
#define TRIGGEREVENT_NOTIFY_MESSAGE		    ("CefClient#TriggerEventNotify")

#endif // WEB_DEFINE_H
