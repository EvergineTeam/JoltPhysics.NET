using System.Runtime.InteropServices;

namespace Evergine.Bindings.JoltPhysics
{
    /// <summary>
    /// The name and calling convention every <see cref="DllImportAttribute"/> in the generated
    /// code resolves against.
    /// </summary>
    /// <remarks>
    /// <para>
    /// On iOS the library is not loaded, it is linked. Apple only lets an application load
    /// dynamic libraries that ship inside its own bundle as frameworks, so JoltPhysicsC builds a
    /// static archive for that platform and the package links it into the application at build
    /// time. Once linked, the symbols live in the executable itself, and the name that reaches
    /// them is <c>__Internal</c> rather than the library's own.
    /// </para>
    /// <para>
    /// Everywhere else the library is loaded at run time out of <c>runtimes/&lt;rid&gt;/native</c>
    /// and the name is the file's. browser-wasm is also linked rather than loaded, but there the
    /// module name comes from the file name, which is why the package ships the wasm archive as
    /// JoltC.a and this constant stays put.
    /// </para>
    /// <para>
    /// Modelled on Evergine.Bindings.Vuforia, the one package in this fleet confirmed to work on
    /// iOS inside a real Evergine project. Vuforia uses StdCall; JoltC is Cdecl and stays Cdecl.
    /// </para>
    /// </remarks>
    internal static class Native
    {
#if __IOS__
        public const string Dll = "__Internal";
#else
        public const string Dll = "JoltC";
#endif
        public const CallingConvention Conv = CallingConvention.Cdecl;
    }
}
