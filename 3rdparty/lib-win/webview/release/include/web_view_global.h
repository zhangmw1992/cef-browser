#ifndef WEB_VIEW_GLOBAL_H
#define WEB_VIEW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WEBVIEW_LIBRARY)
#  define WEBVIEW_EXPORT Q_DECL_EXPORT
#else
#  define WEBVIEW_EXPORT Q_DECL_IMPORT
#endif

#endif // WEB_VIEW_GLOBAL_H
