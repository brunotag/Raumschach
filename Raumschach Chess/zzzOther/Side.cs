using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public class Side
    {
        public SideType SideType
        {
            get;
            set;
        }

        public TimeSpan MaxTime
        {
            get
            {
                if (this.SideType == SideType.White)
                    return new TimeSpan(0, 0, 0, 0, ChessboardLogics.GetMaxTimeWhite());
                else
                    return new TimeSpan(0, 0, 0, 0, ChessboardLogics.GetMaxTimeBlack());
            }
            set
            {
                if (this.SideType == SideType.White)
                    ChessboardLogics.SetMaxTimeWhite((int)value.TotalMilliseconds);
                else
                    ChessboardLogics.SetMaxTimeBlack((int)value.TotalMilliseconds);
                    
            }
        }

        public int MaxDepth
        {
            get
            {
                if (this.SideType == SideType.White)
                    return ChessboardLogics.GetMaxDepthWhite();
                else
                    return ChessboardLogics.GetMaxDepthBlack();
            }
            set
            {
                if (this.SideType == SideType.White)
                    ChessboardLogics.SetMaxDepthWhite(value);
                else
                    ChessboardLogics.SetMaxDepthBlack(value);

            }
        }

        public PlayerType PlayerType
        {
            get;
            set;
        }

        public Side(SideType type)
        {
            SideType = type;
            if (type == SideType.White)
                PlayerType = PlayerType.Human;
            else
                PlayerType = PlayerType.Computer;
        }
    }
}
