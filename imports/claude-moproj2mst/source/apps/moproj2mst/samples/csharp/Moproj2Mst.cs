// P/Invoke wrapper for moproj2mst.dll (x64 only: build the caller as x64, not AnyCPU/x86).
// Works on .NET Framework 4.x and .NET 6+. Put moproj2mst.dll next to the application.
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace Moproj2Mst
{
    public enum M2MResult
    {
        Ok = 0,
        BadArgument = 1,      // M2M_E_ARG
        CannotOpen = 2,       // M2M_E_OPEN
        Parse = 3,            // M2M_E_PARSE
        Input = 4,            // M2M_E_INPUT
        NoOperations = 5,     // M2M_E_NO_OPERATIONS
        Write = 6,            // M2M_E_WRITE
        Unsupported = 7       // M2M_E_UNSUPPORTED
    }

    [Flags]
    public enum M2MFlags : uint
    {
        None = 0,
        DoeTemp = 0x1,        // M2M_FLAG_DOE_TEMP (not supported yet)
        Strict = 0x2          // M2M_FLAG_STRICT: warnings become errors
    }

    public static class Converter
    {
        const string Dll = "moproj2mst.dll";

        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        static extern int m2m_api_version();

        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        static extern int m2m_set_codepage(uint codepage);

        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl)]
        static extern int m2m_set_gui_unit(uint unit);

        [DllImport(Dll, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        static extern int m2m_convert_w(string moprojPath, string outMstPath, uint flags,
                                        StringBuilder msg, UIntPtr msgChars);

        /// <summary>C ABI version of the loaded DLL (1).</summary>
        public static int ApiVersion => m2m_api_version();

        /// <summary>ANSI code page for text in the MST (default 1252).</summary>
        public static M2MResult SetCodepage(uint codepage) => (M2MResult)m2m_set_codepage(codepage);

        /// <summary>
        /// GUI start-up unit for unit-dependent defaults: 0 = derive it as DEF_GUI_MO does
        /// (default), 1 = SI, 2 = English.
        /// </summary>
        public static M2MResult SetGuiUnit(uint unit) => (M2MResult)m2m_set_gui_unit(unit);

        /// <summary>
        /// Converts a .moproj into the .MST DEF_GUI_MO_64 saves. outMstPath null: next to the
        /// project. message: the error text, or warnings (one per line) when the result is Ok.
        /// </summary>
        public static M2MResult Convert(string moprojPath, string outMstPath, M2MFlags flags, out string message)
        {
            var msg = new StringBuilder(4096);
            int rc = m2m_convert_w(moprojPath, outMstPath, (uint)flags, msg, (UIntPtr)msg.Capacity);
            message = msg.ToString();
            return (M2MResult)rc;
        }
    }
}
