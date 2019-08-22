using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public enum PossibleResult
    {
        BlackWins = 0,
        WhiteWins = 1,
        DrawByStalemate = 2,
        DrawByFifty = 3,
        StillUndetermined = 4
    }
}
