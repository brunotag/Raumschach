using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public class GameTypeNetworkMenuScreen : RaumschachMenuScreen
    {
        #region Fields

        MenuEntry CreateEntry;
        MenuEntry JoinEntry;

        #endregion

        #region Initialization


        /// <summary>
        /// Constructor.
        /// </summary>
        public GameTypeNetworkMenuScreen()
            : base("GameTypeNetworkMenuScreen")
        {
            // Create our menu entries.
            CreateEntry = new MenuEntry(string.Empty);
            JoinEntry = new MenuEntry(string.Empty);

            MenuEntry backMenuEntry = new MenuEntry("Back");

            // Hook up menu event handlers.
            CreateEntry.Selected += CreateEntry_Selected;
            JoinEntry.Selected += JoinEntry_Selected;
            backMenuEntry.Selected += OnExit;
            
            // Add entries to the menu.
            MenuEntries.Add(CreateEntry);
            MenuEntries.Add(JoinEntry);
            MenuEntries.Add(backMenuEntry);
        }


        public override void LoadContent()
        {            
            SetMenuEntryText();
            base.LoadContent();
        }

        /// <summary>
        /// Fills in the latest values for the options screen menu text.
        /// </summary>
        void SetMenuEntryText()
        {
            CreateEntry.Text = "Create New Multiplayer Game";
            JoinEntry.Text = "Join Multiplayer Game";
        }


        #endregion

        #region Handle Input

        void CreateEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            ScreenManager.AddScreen(new SelectBlackWhiteNetworkGameMenuScreen(), e.PlayerIndex);
        }

        void JoinEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            ScreenManager.AddScreen(new InsertIPAddressNetworkGameScreen("Ip Address"), e.PlayerIndex);
        }

        #endregion
    }
}
