using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using xWinFormsLib;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using System.Text.RegularExpressions;

namespace Raumschach_Chess
{
    public class InsertIPAddressNetworkGameScreen : RaumschachMenuScreen
    {
        private MessageBox msgboxErrorIPAddr = null;
        private string ipToUse;

        private void LoadFormCollection()
        {
            ((Button)Game.FormCollection["form1"]["btnOk"]).OnRelease = null;
            ((Button)Game.FormCollection["form1"]["btnCancel"]).OnRelease = null;

            ((Button)Game.FormCollection["form1"]["btnOk"]).OnRelease += OnRelease_btnOk;
            ((Button)Game.FormCollection["form1"]["btnCancel"]).OnRelease += OnRelease_btnCancel;

            Game.FormCollection["form1"].Show();
        }

        public InsertIPAddressNetworkGameScreen(string title)
            : base(title)
        {
        }

        private static void TryToConnect(object insertIPScreen)
        {
            InsertIPAddressNetworkGameScreen insIPScreen = (InsertIPAddressNetworkGameScreen)insertIPScreen;

            Lidgren.Network.NetConfiguration config = new Lidgren.Network.NetConfiguration("chessPlayConnection");
            config.MaxConnections = 1;
            config.TimeoutDelay = 15;
            Lidgren.Network.NetPeer peer = new Lidgren.Network.NetPeer(config);            
            try
            {
                Lidgren.Network.NetConnection connection = peer.Connect(insIPScreen.ipToUse, Utilities.PortNumber);
                while (connection.Status == Lidgren.Network.NetConnectionStatus.Connecting) ;
                if (connection.Status == Lidgren.Network.NetConnectionStatus.Disconnected)
                    throw new Lidgren.Network.NetException("error");

                Lidgren.Network.NetConnection conn;
                Lidgren.Network.NetMessageType type;
                String side
                    = Utilities.GetMessageFromConnection(peer, out type, out conn);

                //if (side == SideType.White.ToString())
                //{
                //    insIPScreen.Game.OptionsCurrent.BlackPlayer = GameOptions.PlayerOption.RemoteHuman;
                //    insIPScreen.Game.OptionsCurrent.WhitePlayer = GameOptions.PlayerOption.LocalHuman;
                //}
                //else
                //{
                //    insIPScreen.Game.OptionsCurrent.BlackPlayer = GameOptions.PlayerOption.LocalHuman;
                //    insIPScreen.Game.OptionsCurrent.WhitePlayer = GameOptions.PlayerOption.RemoteHuman;
                //}

                //LoadingScreen.Load(insIPScreen.ScreenManager, true, insIPScreen.controllingPlayer,
                //    new GamePlayNetworkScreen(peer));
                insIPScreen.ExitScreen();
            }
            catch (Lidgren.Network.NetException e)
            {
                insIPScreen.ShowMessageBox("Unable to connect to " + insIPScreen.ipToUse);
            }
        }

        void OnRelease_btnOk(object sender, EventArgs e)
        {
            String insertedIP = ((Textbox)Game.FormCollection["form1"]["textbox1"]).Text;
            if (IsValidIP(insertedIP))
            {
                ipToUse = insertedIP;
                screenManager.AddScreen(
                    new BackgroundWorkScreen("Trying to connect to host...",
                        new Action<object>(TryToConnect),
                        this),
                        this.controllingPlayer
                        );
            }
            else
            {
                ShowMessageBox(insertedIP + "\nis not a valid IP Address!\nInsert a valid IP Address!");
            }
        }

        private void ShowMessageBox(string message)
        {
            msgboxErrorIPAddr =
    new MessageBox(
        new Vector2(300, 200),
        new Vector2(200, 250),
        "Error!",
        message,
        MessageBox.Type.MB_OK);
            msgboxErrorIPAddr.OnOk +=
                delegate
                {
                    //((Textbox)Game.FormCollection["form1"]["textbox1"]).Text = String.Empty;
                    msgboxErrorIPAddr.Close();
                    msgboxErrorIPAddr = null;
                };
            msgboxErrorIPAddr.Show();
        }
        //with this trick we've wired the "Enter" key to the "Ok" button of both our windows
        protected override void OnSelectEntry(int entryIndex, PlayerIndex playerIndex)
        {
            if (msgboxErrorIPAddr == null)
                OnRelease_btnOk(this, new EventArgs());
            else
                msgboxErrorIPAddr.OnOk.Invoke(this, new EventArgs());
        }

        /// <summary>
        /// method to validate an IP address
        /// using regular expressions. The pattern
        /// being used will validate an ip address
        /// with the range of 1.0.0.0 to 255.255.255.255
        /// </summary>
        /// <param name="addr">Address to validate</param>
        /// <returns></returns>
        private bool IsValidIP(string addr)
        {
            //create our match pattern
            string pattern = 
@"\b(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\b";
 
            //create our Regular Expression object
            Regex check = new Regex(pattern);
            //boolean variable to hold the status
            bool valid = false;
            //check to make sure an ip address was provided
            if (addr == String.Empty)
            {
                //no address provided so return false
                valid = false;
            }
            else
            {
                //address provided so use the IsMatch Method
                //of the Regular Expression object
                valid = addr == check.Match(addr, 0).ToString();
            }
            //return the results
            return valid;
        }

        void OnRelease_btnCancel(object sender, EventArgs e)
        {
            OnExit(sender, new PlayerIndexEventArgs(this.ControllingPlayer.Value));
        }

        public override void Update(GameTime gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
        {
            base.Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
        }

        public override void Draw(GameTime gameTime)
        {
            //Render the form collection (required before drawing)
            Game.FormCollection.Render();

            //GraphicsDevice.Clear(Color.CornflowerBlue);

            //Draw the form collection
            Game.FormCollection.Draw();
            base.Draw(gameTime);
        }

        public override void LoadContent()
        {
            LoadFormCollection();
            base.LoadContent();
        }
    }
}
