using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using System.Runtime.InteropServices;
namespace MyDLLDebugger
{

    public class Program
    {
        static String s =
            "rnknr/ppppp/5/5/5-buqbu/ppppp/5/5/5-5/5/5/5/5-5/5/5/PPPPP/BUQBU-5/5/5/PPPPP/RNKNR w 0 0";

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        [return: System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)]
        public static extern String GetCompMoveFromFEN(
            int sideToMove,
            int maxTime,
            [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
                String FENRepresentation
            );

        [DllImport("RaumschachChessEngine.dll")]
        public static extern void DisplayHelloFromDLL();
        
        static void Main(string[] args)
        {
            String move = GetCompMoveFromFEN(0,8000,s);
            Console.WriteLine(move);
            Console.ReadLine();
        }

        
    }


    /*
    public class Program
    {
        // Use DllImport to import the Win32 MessageBox function.
        [DllImport("user32.dll")]
        public static extern int MessageBox(IntPtr hWnd, String text, String caption, uint type);

        static void Main()
        {
            // Call the MessageBox function using platform invoke.
            MessageBox(new IntPtr(0), "Hello World!", "Hello Dialog", 0);
        }
    }
    */
}
