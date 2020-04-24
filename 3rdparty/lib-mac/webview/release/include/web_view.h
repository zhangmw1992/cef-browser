#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWidget>
#include <QVariant>

#include "web_view_global.h"

class WEBVIEW_EXPORT WebView : public QWidget
{
    Q_OBJECT

public:
    WebView(const QString url, QWidget* parent = nullptr);
    ~WebView();

    void navigateToUrl(const QString& url);

    void runJsFunctions(const QString& data);

    bool setCookie(const QString& name, const QString& value, const QString& domain, const QString& path);
    bool deleteCookies();

    void reload();
    void stopLoad();

signals:
    void loadStart();
    void loadEnd(int httpStatusCode);
    void loadError(int errorCode, const QString& errorMsg, const QString& failedUrl);

    void invokeMethodNotify(int browserId, int frameId, const QString& method, const QVariantList& arguments);

protected:
    virtual void changeEvent(QEvent * event) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    class Implementation;
    std::unique_ptr<Implementation> pImpl_;
};

#endif // WEBVIEW_H
