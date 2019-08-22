using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public abstract class RaumschachGameScreen : GameScreen
    {
        protected Raumschach Game
        {
            get
            {
                return this.ScreenManager.Game as Raumschach;
            }
        }
    }
}
