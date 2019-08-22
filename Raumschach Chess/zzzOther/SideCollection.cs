using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace Raumschach_Chess
{
    public class SideCollection : ReadOnlyCollectionBase
    {
        public SideCollection()
        {
            InnerList.Add(new Side(SideType.White));
            InnerList.Add(new Side(SideType.Black));
        }

        private Dictionary<SideType, int> sideToInt = new Dictionary<SideType, int>()
        {
            {SideType.White, 0},
            {SideType.Black, 1}
        };

        public Side this[SideType index]
        {
            get
            {
                return ((Side)InnerList[sideToInt[index]]);
            }
        }


    }
}
