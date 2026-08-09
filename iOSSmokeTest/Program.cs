// Proves that the package links on iOS, which nothing else in this repository can.
//
// The desktop legs load a library out of runtimes/<rid>/native at run time. iOS does not load
// anything: Apple only lets an application load dynamic libraries that ship inside its own bundle
// as frameworks, so JoltPhysicsC builds a static archive and the package's targets file links it
// into the executable. Two things have to be true for that to work, and neither is visible
// anywhere else:
//
//   the targets file adds the archive to the link with ForceLoad and SmartLink, and
//   Native.Dll compiled to "__Internal" rather than "JoltC",
//
// because once linked the symbols live in the executable and only that name reaches them. Get
// either wrong and the build still succeeds; the failure arrives at the first P/Invoke, on a
// device, in somebody else's application.
//
// A build is the assertion here rather than a run. The CI leg then opens the executable and
// requires JoltC_Init to be a defined symbol -- a T in nm, not merely a matching name, which is a
// distinction that cost three diagnostic rounds in Cesium.NET to learn to make.

using Foundation;
using UIKit;
using Evergine.Bindings.JoltPhysics;

namespace iOSSmokeTest;

[Register("AppDelegate")]
public class AppDelegate : UIApplicationDelegate
{
    public override UIWindow Window { get; set; }

    public override bool FinishedLaunching(UIApplication application, NSDictionary launchOptions)
    {
        // The same first calls the desktop leg makes. These are what force the linker to keep
        // the members carrying them.
        JoltPhysics.RegisterDefaultAllocator();
        JoltPhysics.Init();
        JoltPhysics.CreateFactory();
        JoltPhysics.RegisterTypes();

        System.IntPtr tempAllocator = JoltPhysics.TempAllocator_Create(4 * 1024 * 1024);
        System.IntPtr jobSystem = JoltPhysics.JobSystemThreadPool_Create(1024, 8, 0);

        bool ok = tempAllocator != System.IntPtr.Zero && jobSystem != System.IntPtr.Zero;

        Window = new UIWindow(UIScreen.MainScreen.Bounds)
        {
            RootViewController = new UIViewController
            {
                View = { BackgroundColor = ok ? UIColor.SystemGreen : UIColor.SystemRed },
            },
        };
        Window.MakeKeyAndVisible();
        return true;
    }
}

public static class Program
{
    public static void Main(string[] args) => UIApplication.Main(args, null, typeof(AppDelegate));
}
