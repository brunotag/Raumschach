using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public class EndgameMenuScreen : RaumschachMenuScreen
    {
        public EndgameMenuScreen(string message) : base(message)
        {
            MenuEntry exitGameMenuEntry = new MenuEntry("Exit");
            MenuEntry newGameMenuEntry = new MenuEntry("New Game");

            exitGameMenuEntry.Selected += exitGameMenuEntry_Selected;
            newGameMenuEntry.Selected += newGameMenuEntry_Selected;

            MenuEntries.Add(newGameMenuEntry);
            MenuEntries.Add(exitGameMenuEntry);
        }

        void newGameMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            foreach (GameScreen screen in ScreenManager.GetScreens())
            {
                screen.ExitScreen();
            }
            screenManager.AddScreen(new MainMenuScreen(), this.controllingPlayer);
        }

        void exitGameMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            Game.Exit();
        }
    }
}
