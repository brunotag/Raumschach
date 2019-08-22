using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public struct PGNTags
    {

        public static Dictionary<PossibleResult, string> resultTranslation = new Dictionary<PossibleResult, string>()
        {
            {PossibleResult.WhiteWins,"1-0"},
            {PossibleResult.BlackWins, "0-1"},
            {PossibleResult.DrawByFifty, "1/2-1/2"},
            {PossibleResult.DrawByStalemate, "1/2-1/2"},
            {PossibleResult.StillUndetermined, String.Empty}
        };

        public string Event;
        public string Site;
        public DateTime Date;
        public int? Round;
        public string White;
        public string Black;
        public PossibleResult? Result;

    }
}
