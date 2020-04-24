#include "client_renderer.h"
#include <qsystemdetection.h>

#ifdef Q_OS_MAC
#include "include/wrapper/cef_library_loader.h"
#include "include/cef_sandbox_mac.h"

int main(int argc, char *argv[])
{
    // Initialize the macOS sandbox for this helper process.
    CefScopedSandboxContext sandbox_context;
    if (!sandbox_context.Initialize(argc, argv))
        return 1;

    // Load the CEF framework library at runtime instead of linking directly
    // as required by the macOS sandbox implementation.
    CefScopedLibraryLoader library_loader;
    if (!library_loader.LoadInHelper())
        return 1;

    // Provide CEF with command-line arguments.
    CefMainArgs main_args(argc, argv);
#else
#include <tchar.h>

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nCmdShow)
{
    CefEnableHighDPISupport();

    CefMainArgs main_args(hInstance);
#endif

    CefRefPtr<ClientRenderer> app(new ClientRenderer);

    // Execute the sub-process.
    return CefExecuteProcess(main_args, app.get(), nullptr);
}
