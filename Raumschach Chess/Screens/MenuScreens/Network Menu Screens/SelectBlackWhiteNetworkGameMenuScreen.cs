using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public class SelectBlackWhiteNetworkGameMenuScreen : RaumschachMenuScreen
    {
        #region Fields

        MenuEntry BlackEntry;
        MenuEntry WhiteEntry;

        #endregion

        #region Initialization


        /// <summary>
        /// Constructor.
        /// </summary>
        public SelectBlackWhiteNetworkGameMenuScreen()
            : base("SelectBlackWhiteNetworkGameMenuScreen")
        {
            // Create our menu entries.
            BlackEntry = new MenuEntry(string.Empty);
            WhiteEntry = new MenuEntry(string.Empty);

            MenuEntry backMenuEntry = new MenuEntry("Back");

            // Hook up menu event handlers.
            BlackEntry.Selected += BlackEntry_Selected;
            WhiteEntry.Selected += WhiteEntry_Selected;
            backMenuEntry.Selected += OnExit;
            
            // Add entries to the menu.
            MenuEntries.Add(BlackEntry);
            MenuEntries.Add(WhiteEntry);
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
            BlackEntry.Text = "Use Black Pieces";
            WhiteEntry.Text = "Use White Pieces";
        }
        #endregion

        private static void ListenForConnectionAndStart(object selectBWNGMS)
        {
            SelectBlackWhiteNetworkGameMenuScreen selBWNGMS = (SelectBlackWhiteNetworkGameMenuScreen)selectBWNGMS;

            Lidgren.Network.NetConfiguration config = new Lidgren.Network.NetConfiguration("chessPlayConnection");
            config.MaxConnections = 1;
            config.TimeoutDelay = 15;
            config.Port = Utilities.PortNumber;
            Lidgren.Network.NetPeer peer = new Lidgren.Network.NetPeer(config);

            peer.Start();
            do { }
            while (                
                (peer.Connections.Count == 0) ||
                (peer.Connections[0].Status != Lidgren.Network.NetConnectionStatus.Connected)                
            );

            string remoteSide;
            if (selBWNGMS.Game.StatusCurrent.Sides[SideType.Black].PlayerType == PlayerType.Human)
                remoteSide = SideType.White.ToString();
            else
                remoteSide = SideType.Black.ToString();

            Utilities.WriteMessageToRemotePlayer(peer, remoteSide);

            //LoadingScreen.Load(selBWNGMS.ScreenManager, true, selBWNGMS.controllingPlayer,
            //    new GamePlayNetworkScreen(peer));
            selBWNGMS.ExitScreen();
        }

        #region Handle Input
        void BlackEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            Game.StatusCurrent.Sides[SideType.Black].PlayerType = PlayerType.Human;
            //Game.StatusCurrent.Sides[SideType.White].PlayerType = PlayerType.RemoteHuman;

            screenManager.AddScreen(
                    new BackgroundWorkScreen("Waiting for incoming connections...",
                        new Action<object>(ListenForConnectionAndStart),
                        this),
                        this.controllingPlayer
                        );
        }

        void WhiteEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            Game.StatusCurrent.Sides[SideType.White].PlayerType = PlayerType.Human;
            //Game.StatusCurrent.Sides[SideType.Black].PlayerType = PlayerType.RemoteHuman;

            screenManager.AddScreen(
                    new BackgroundWorkScreen("Waiting for incoming connections...",
                        new Action<object>(ListenForConnectionAndStart),
                        this),
                        this.controllingPlayer
                        );
        }
        #endregion
    }
}
