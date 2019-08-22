using System;

namespace Raumschach_Chess
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            using (Raumschach game = new Raumschach())
            {
                game.Run();
            }
        }
    }
}

