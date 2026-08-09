// Exists so the package has to be consumed by something that really builds for Android.
//
// It is not run. No GitHub runner can execute an APK without an emulator, and standing one up
// per release was decided against. What the CI leg asserts instead is that libJoltC.so ends up
// inside the APK under lib/armeabi-v7a/ and lib/arm64-v8a/ -- which is the class of mistake
// this package actually shipped for months on wasm and iOS: a native library present in the
// package and absent from where the platform looks for it. A build that merely compiles would
// not notice either.
//
// The calls below are here so the binding assembly is genuinely referenced rather than trimmed
// away as unused. They run on a device and never in CI.

using Android.App;
using Android.OS;
using Android.Widget;
using Evergine.Bindings.JoltPhysics;

namespace AndroidSmokeTest;

[Activity(Label = "Jolt smoke", MainLauncher = true)]
public class MainActivity : Activity
{
    protected override void OnCreate(Bundle savedInstanceState)
    {
        base.OnCreate(savedInstanceState);

        // The same first calls the desktop leg makes. Reaching them at all means the native
        // library was found and loaded out of the APK.
        JoltPhysics.RegisterDefaultAllocator();
        JoltPhysics.Init();
        JoltPhysics.CreateFactory();
        JoltPhysics.RegisterTypes();

        System.IntPtr tempAllocator = JoltPhysics.TempAllocator_Create(4 * 1024 * 1024);
        System.IntPtr jobSystem = JoltPhysics.JobSystemThreadPool_Create(1024, 8, 1);

        bool ok = tempAllocator != System.IntPtr.Zero && jobSystem != System.IntPtr.Zero;

        var text = new TextView(this)
        {
            Text = ok
                ? $"JoltC loaded: allocator=0x{tempAllocator:x} jobs=0x{jobSystem:x}"
                : "JoltC did not load",
        };

        SetContentView(text);
    }
}
