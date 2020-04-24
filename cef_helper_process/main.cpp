#ifdef Q_OS_MAC

#include "include/cef_app.h"
#include "include/wrapper/cef_library_loader.h"
<<<<<<< HEAD

=======
>>>>>>> 022c1d0ac275f23cd9950833de38c3d886d675a7
#include "include/cef_sandbox_mac.h"

// Entry point function for sub-processes.
int main(int argc, char* argv[])
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

    // Execute the sub-process.
    return CefExecuteProcess(main_args, nullptr, nullptr);
}

#endif // Q_OS_MAC
