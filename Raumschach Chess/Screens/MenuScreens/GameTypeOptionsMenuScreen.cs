using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public class GameTypeOptionsMenuScreen :RaumschachMenuScreen
    {
        #region Fields

        MenuEntry BlackPlayerEntry;
        MenuEntry WhitePlayerEntry;

        static PlayerType NewBlackPlayerOptions;
        static PlayerType NewWhitePlayerOptions;

        #endregion

        #region Initialization


        /// <summary>
        /// Constructor.
        /// </summary>
        public GameTypeOptionsMenuScreen()
            : base("GameTypeOptionsScreen")
        {
            // Create our menu entries.
            BlackPlayerEntry = new MenuEntry(string.Empty);
            WhitePlayerEntry = new MenuEntry(string.Empty);

            MenuEntry backMenuEntry = new MenuEntry("Back");
            MenuEntry okMenuEntry = new MenuEntry("Ok!");

            // Hook up menu event handlers.
            BlackPlayerEntry.Selected += BlackPlayerEntry_Selected;
            WhitePlayerEntry.Selected += WhitePlayerEntry_Selected;
            backMenuEntry.Selected += OnExit;
            okMenuEntry.Selected += OkMenuEntry_Selected;
            
            // Add entries to the menu.
            MenuEntries.Add(BlackPlayerEntry);
            MenuEntries.Add(WhitePlayerEntry);
            MenuEntries.Add(okMenuEntry);
            MenuEntries.Add(backMenuEntry);
        }


        public override void LoadContent()
        {            
            NewBlackPlayerOptions = Game.StatusCurrent.Sides[SideType.Black].PlayerType;
            NewWhitePlayerOptions = Game.StatusCurrent.Sides[SideType.White].PlayerType;
            SetMenuEntryText();
            base.LoadContent();
        }

        /// <summary>
        /// Fills in the latest values for the options screen menu text.
        /// </summary>
        void SetMenuEntryText()
        {
            BlackPlayerEntry.Text = "Black Player: " +
                Enum.GetName(NewBlackPlayerOptions.GetType(), NewBlackPlayerOptions);
            WhitePlayerEntry.Text = "White Player: " +
                Enum.GetName(NewWhitePlayerOptions.GetType(), NewWhitePlayerOptions);
        }

        void OkMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            Game.StatusCurrent.Sides[SideType.Black].PlayerType = NewBlackPlayerOptions;
            Game.StatusCurrent.Sides[SideType.White].PlayerType = NewWhitePlayerOptions;
            OnExit(sender, e);
            
            LoadingScreen.Load(ScreenManager, true, e.PlayerIndex,new GamePlayScreen());
            
            //ScreenManager.AddScreen(new GamePlayScreen(), e.PlayerIndex);

            //GamePlayScreen gps = new GamePlayScreen();
            //gps.DebugMode = true;
            //screenManager.AddScreen(gps, 0);
        }


        #endregion

        #region Handle Input

        void WhitePlayerEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            int[] arr = (int[]) (Enum.GetValues(NewWhitePlayerOptions.GetType()));
            Array.Sort (arr);
            NewWhitePlayerOptions++;

            if ((int)NewWhitePlayerOptions > arr[arr.Length - 1])
                NewWhitePlayerOptions = (PlayerType) arr[0];

            SetMenuEntryText();
        }

        void BlackPlayerEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            int[] arr = (int[])(Enum.GetValues(NewBlackPlayerOptions.GetType()));
            Array.Sort(arr);
            NewBlackPlayerOptions++;

            if ((int)NewBlackPlayerOptions > arr[arr.Length - 1])
                NewBlackPlayerOptions = (PlayerType)arr[0];
            SetMenuEntryText();
        }

        #endregion
    }
}
