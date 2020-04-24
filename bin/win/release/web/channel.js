function onInvokeQtFunction(method, args) {
    if (1) {
        args = args || '';
        if (typeof CefClient == 'undefined') {
            if (window.bridges !== undefined) {
                window.bridges.invokeMethod(method, args);
            } else {
                var maxTry = 5;
                var timer = setInterval(function () {
                    if (window.bridges === undefined) {
                        maxTry--;
                    } else if (maxTry === 0) {
                        clearInterval(timer);
                        console.warn('onInvokeQtFunction(' + method + ') timeout, failed!');
                    } else {
                        clearInterval(timer);
                        window.bridges.invokeMethod(method, args);
                    }
                }, 500);
            }
        } else {
            CefClient.invokeMethod(method, args);
        }
    }
}
