using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using xWinFormsLib;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Raumschach_Chess
{
    public partial class Raumschach
    {
        #region GamePlayScreen Forms Build

        void GameplayBuildForms(FormCollection formCollection)
        {

            //Create a new form
            formCollection.Add(new Form("frmGamePlayScreen", "Game Controls",
                new Vector2(320, 400),
                new Vector2(graphics.PreferredBackBufferWidth - 335, graphics.PreferredBackBufferHeight - 415),
                Form.BorderStyle.Sizable));
            //formCollection["frmGamePlayScreen"].Style = Form.BorderStyle.Fixed;
            formCollection["frmGamePlayScreen"].CloseButton = null;
            formCollection["frmGamePlayScreen"].HasMaximizeButton = false;

            //Add a Label
            formCollection["frmGamePlayScreen"].Controls.Add(
    new Label("lblTurn", new Vector2(15, 30), "It's white turn", Color.White, Color.Black, 200, Label.Align.Center
        ));
            formCollection["frmGamePlayScreen"].Controls.Add(
    new Label("lblComputerTime", new Vector2(15, 45), "", Color.White, Color.Black, 200, Label.Align.Center
        ));
            formCollection["frmGamePlayScreen"].Controls.Add(
    new Label("lblComputerMove", new Vector2(15, 60), "", Color.White, Color.Black, 200, Label.Align.Center
        ));

            formCollection["frmGamePlayScreen"].Controls.Add(
                new Label("lblRbgWhitePlayer", new Vector2(15, 80), "White", Color.White, Color.Black, 100, Label.Align.Left
                    ));
            formCollection["frmGamePlayScreen"].Controls.Add(new RadiuButtonGroup("rbgWhitePlayer", new RadioButton[] { 
                new RadioButton("rbgWhitePlayerHuman", new Vector2(15, 100), res.Human, true),
                new RadioButton("rbgWhitePlayerComputer", new Vector2(15, 120), res.Computer, false)
            }));
            formCollection["frmGamePlayScreen"].Controls.Add(new Textbox("txtTimeWhitePlayer",
                new Vector2(15, 140), 100, 40, ""));
            ((Textbox)formCollection["frmGamePlayScreen"]["txtTimeWhitePlayer"]).Scrollbar = Textbox.Scrollbars.None;
            formCollection["frmGamePlayScreen"].Controls.Add(new Textbox("txtDepthWhitePlayer",
                new Vector2(15, 160), 100, 40, ""));
            ((Textbox)formCollection["frmGamePlayScreen"]["txtDepthWhitePlayer"]).Scrollbar = Textbox.Scrollbars.None;

            formCollection["frmGamePlayScreen"].Controls.Add(
                new Label("lblRbgBlackPlayer", new Vector2(115, 80), "Black", Color.White, Color.Black, 100, Label.Align.Left
                    ));
            formCollection["frmGamePlayScreen"].Controls.Add(new RadiuButtonGroup("rbgBlackPlayer", new RadioButton[] { 
                new RadioButton("rbgBlackPlayerHuman", new Vector2(115, 100), res.Human, true),
                new RadioButton("rbgBlackPlayerComputer", new Vector2(115, 120), res.Computer, false)
            }));
            formCollection["frmGamePlayScreen"].Controls.Add(new Textbox("txtTimeBlackPlayer",
                new Vector2(115, 140), 100, 40, ""));
            ((Textbox)formCollection["frmGamePlayScreen"]["txtTimeBlackPlayer"]).Scrollbar = Textbox.Scrollbars.None;
            formCollection["frmGamePlayScreen"].Controls.Add(new Textbox("txtDepthBlackPlayer",
                new Vector2(115, 160), 100, 40, ""));
            ((Textbox)formCollection["frmGamePlayScreen"]["txtDepthBlackPlayer"]).Scrollbar = Textbox.Scrollbars.None;


            formCollection["frmGamePlayScreen"].Controls.Add(
                new Label("lblTime", new Vector2(215, 142), "Max Time [s]", Color.White, Color.Black, 95, Label.Align.Left
                    ));
            formCollection["frmGamePlayScreen"].Controls.Add(
                new Label("lblDepth", new Vector2(215, 164), "Max Depth [ply]", Color.White, Color.Black, 95, Label.Align.Left
                    ));

            formCollection["frmGamePlayScreen"].Controls.Add(new Slider("sldTranspSquares", new Vector2(15, 190), 200));
            ((Slider)formCollection["frmGamePlayScreen"]["sldTranspSquares"]).Value = 100;

            formCollection["frmGamePlayScreen"].Controls.Add(new Slider("sldTranspPieces", new Vector2(15, 205), 200));
            ((Slider)formCollection["frmGamePlayScreen"]["sldTranspPieces"]).Value = 100;

            formCollection["frmGamePlayScreen"].Controls.Add(
                new Button("btnOk", new Vector2(15, 280), 100, "Confirm", Color.White, Color.Black));
            //formCollection["form1"]["button1"].OnPress += Button1_OnPress;
            //formCollection["form1"]["button1"].OnRelease = Button1_OnRelease;
            formCollection["frmGamePlayScreen"].Controls.Add(
                new Button("btnCancel", new Vector2(115, 280), 100, "Cancel", Color.White, Color.Black));

            string[] arr =
                (from keyValuePair in predefinedColors
                 where (
                        ((keyValuePair.Value.R + keyValuePair.Value.G + keyValuePair.Value.B) >= 255) &&
                        ((keyValuePair.Value.R + keyValuePair.Value.G + keyValuePair.Value.B) <= 400)
                        )
                 select keyValuePair.Key).ToArray();

            Array.Sort(arr);

            formCollection["frmGamePlayScreen"].Controls.Add(new ComboBox("cmbColorWhitePieces",
            new Vector2(15, 225), 100, /*120,*/ arr));

            formCollection["frmGamePlayScreen"].Controls.Add(new ComboBox("cmbColorWhiteSquares",
                new Vector2(15, 245), 100, /*120,*/ arr));

            formCollection["frmGamePlayScreen"].Controls.Add(new ComboBox("cmbColorBlackPieces",
                new Vector2(115, 225), 100, /*120,*/ arr));

            formCollection["frmGamePlayScreen"].Controls.Add(new ComboBox("cmbColorBlackSquares",
                new Vector2(115, 245), 100, /*120,*/ arr));


            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorWhitePieces"]).Text =
                "Red";
            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorWhiteSquares"]).Text =
                "Red";
            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorBlackPieces"]).Text =
                "Blue";
            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorBlackSquares"]).Text = 
                "Blue";


            formCollection["frmGamePlayScreen"].Controls.Add(new ComboBox("cmbLoadGame",
                new Vector2(115, 300), 100, /*120,*/ loadedGames.Keys.ToArray()));

            formCollection["frmGamePlayScreen"].Controls.Add(
                new Button("btnLoad", new Vector2(15, 300), 100, "Load", Color.White, Color.Black));

            //Events Wiring
            formCollection["frmGamePlayScreen"]["btnOk"].OnPress += btnOk_OnRelease;
            formCollection["frmGamePlayScreen"]["btnCancel"].OnPress += btnCancel_OnRelease;
            formCollection["frmGamePlayScreen"]["btnLoad"].OnPress += btnLoad_OnRelease;
            //formCollection["frmGamePlayScreen"].OnShow += btnCancel_OnRelease; //TODO: delete?
            ((Slider)formCollection["frmGamePlayScreen"]["sldTranspSquares"]).OnValueChanged +=
                sldTranspSquares_OnValueChanged;
            ((Slider)formCollection["frmGamePlayScreen"]["sldTranspPieces"]).OnValueChanged +=
                sldTranspPieces_OnValueChanged;
            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorWhitePieces"]).OnSelectionChanged += 
                cmbColorWhitePieces_OnSelectionChanged;
            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorWhiteSquares"]).OnSelectionChanged += 
                cmbColorWhiteSquares_OnSelectionChanged;
            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorBlackPieces"]).OnSelectionChanged += 
                cmbColorBlackPieces_OnSelectionChanged;
            ((ComboBox)formCollection["frmGamePlayScreen"]["cmbColorBlackSquares"]).OnSelectionChanged +=
                cmbColorBlackSquares_OnSelectionChanged;
            
        }
#endregion 

        #region GamePlayScreen Forms Eventhandlers
        private void btnOk_OnRelease(object obj, EventArgs e)
        {
            try
            {
                int blackTimeSec = int.Parse(((Textbox)formCollection["frmGamePlayScreen"]["txtTimeBlackPlayer"]).Text);
                int whiteTimeSec = int.Parse(((Textbox)formCollection["frmGamePlayScreen"]["txtTimeWhitePlayer"]).Text);
                int blackDepth = int.Parse(((Textbox)formCollection["frmGamePlayScreen"]["txtDepthBlackPlayer"]).Text);
                int whiteDepth = int.Parse(((Textbox)formCollection["frmGamePlayScreen"]["txtDepthWhitePlayer"]).Text);


                string blackPlayerSel = ((RadiuButtonGroup)formCollection["frmGamePlayScreen"]["rbgBlackPlayer"]).Selected;
                string whitePlayerSel = ((RadiuButtonGroup)formCollection["frmGamePlayScreen"]["rbgWhitePlayer"]).Selected;
                
                MessageBox msgSure = new MessageBox(new Vector2(200, 100), "Confirm", res.AreYouSure, MessageBox.Type.MB_YESNO);
                msgSure.OnNo += btnCancel_OnRelease;
                msgSure.OnYes += delegate
                {
                    StatusCurrent.Sides[SideType.Black].MaxTime = new TimeSpan(0, 0, blackTimeSec);
                    StatusCurrent.Sides[SideType.White].MaxTime = new TimeSpan(0, 0, whiteTimeSec);
                    StatusCurrent.Sides[SideType.Black].MaxDepth = blackDepth;
                    StatusCurrent.Sides[SideType.White].MaxDepth = whiteDepth;

                    if (blackPlayerSel == res.Computer)
                        StatusCurrent.Sides[SideType.Black].PlayerType = PlayerType.Computer;
                    else
                        StatusCurrent.Sides[SideType.Black].PlayerType = PlayerType.Human;
                    if (whitePlayerSel == res.Computer)
                        StatusCurrent.Sides[SideType.White].PlayerType = PlayerType.Computer;
                    else
                        StatusCurrent.Sides[SideType.White].PlayerType = PlayerType.Human;

                };
                msgSure.Show();                
            }
            catch (Exception)
            {
                MessageBox msgError = new MessageBox(new Vector2(200, 100), "Error", res.ErrorNotValidInput, MessageBox.Type.MB_OK);
                msgError.Show();
            }                       
        }
        private void btnCancel_OnRelease(object obj, EventArgs e)
        {
            //(Re)load parameters
            if (this.StatusCurrent.Sides[SideType.Black].PlayerType == PlayerType.Computer)
                ((RadiuButtonGroup)formCollection["frmGamePlayScreen"]["rbgBlackPlayer"]).Selected = res.Computer;
            else ((RadiuButtonGroup)formCollection["frmGamePlayScreen"]["rbgBlackPlayer"]).Selected = res.Human;

            if (this.StatusCurrent.Sides[SideType.White].PlayerType == PlayerType.Computer)
                ((RadiuButtonGroup)formCollection["frmGamePlayScreen"]["rbgWhitePlayer"]).Selected = res.Computer;
            else ((RadiuButtonGroup)formCollection["frmGamePlayScreen"]["rbgWhitePlayer"]).Selected = res.Human;

            ((Textbox)formCollection["frmGamePlayScreen"]["txtTimeWhitePlayer"]).Text =
                this.StatusCurrent.Sides[SideType.White].MaxTime.TotalSeconds.ToString();
            ((Textbox)formCollection["frmGamePlayScreen"]["txtTimeBlackPlayer"]).Text =
                this.StatusCurrent.Sides[SideType.Black].MaxTime.TotalSeconds.ToString();

            ((Textbox)formCollection["frmGamePlayScreen"]["txtDepthWhitePlayer"]).Text =
                this.StatusCurrent.Sides[SideType.White].MaxDepth.ToString();
            ((Textbox)formCollection["frmGamePlayScreen"]["txtDepthBlackPlayer"]).Text =
                this.StatusCurrent.Sides[SideType.Black].MaxDepth.ToString();

        }
        private void btnLoad_OnRelease(object obj, EventArgs e)
        {
            string FEN = loadedGames[((ComboBox)formCollection["frmGamePlayScreen"]["cmbLoadGame"]).Text];

            ChessboardLogics.SetChessboardToFEN(FEN);
        }
        private void sldTranspSquares_OnValueChanged(object obj, EventArgs e)
        {
            float val = ((float)obj) / 100;
            StatusCurrent.TranspGradSquares = val;
        }
        private void sldTranspPieces_OnValueChanged(object obj, EventArgs e)
        {
            float val = ((float)obj) / 100;
            StatusCurrent.TranspGradPieces = val;
        }
        private void cmbColorWhitePieces_OnSelectionChanged(object obj, EventArgs e)
        {
            this.StatusCurrent.ColorWhitePieces = predefinedColors[(string)obj];
        }
        private void cmbColorWhiteSquares_OnSelectionChanged(object obj, EventArgs e)
        {
            this.StatusCurrent.ColorWhiteSquares = predefinedColors[(string)obj];
        }
        private void cmbColorBlackPieces_OnSelectionChanged(object obj, EventArgs e)
        {
            this.StatusCurrent.ColorBlackPieces = predefinedColors[(string)obj];
        }
        private void cmbColorBlackSquares_OnSelectionChanged(object obj, EventArgs e)
        {
            this.StatusCurrent.ColorBlackSquares = predefinedColors[(string)obj];
        }            
        #endregion

        Dictionary<string, Color> predefinedColors = new Dictionary<string, Color>();

        Dictionary<string, string> loadedGames;

        void BuildForms(FormCollection formCollection)
        {

            // Get all of the public static properties
            System.Reflection.PropertyInfo[] properties =
                typeof(Color).GetProperties(
                System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Static
                );

            foreach (System.Reflection.PropertyInfo propertyInfo in properties)
            {
                // Check to make sure the property has a get method, and returns type "Color"
                if (propertyInfo.GetGetMethod() != null && propertyInfo.PropertyType == typeof(Color))
                {
                    // Get the color returned by the property by invoking it
                    Color color = (Color)propertyInfo.GetValue(null, null);
                    predefinedColors.Add(propertyInfo.Name, color);
                }
            }

            loadedGames = FileHelper.LoadSaves();

            GameplayBuildForms(formCollection);
        }



        //    //Create a new form
        //    formCollection.Add(new Form(
        //        "form1", "Please, Insert IP Address Of The Server",
        //        new Vector2(400, 150),
        //        new Vector2(200, 300),
        //        Form.BorderStyle.Fixed)
        //        );
        //    formCollection["form1"].Style = Form.BorderStyle.Fixed;

        //    //Create a new textbox
        //    formCollection["form1"].Controls.Add(
        //        new Textbox(
        //            "textbox1",
        //            new Vector2(10, 50),
        //            200, 40,
        //            "",
        //            false)
        //            );
        //    ((Textbox)formCollection["form1"]["textbox1"]).Scrollbar = Textbox.Scrollbars.None;
        //    ((Textbox)formCollection["form1"]["textbox1"]).HasFocus = true;

        //    //Create OK button
        //    formCollection["form1"].Controls.Add(
        //        new Button(
        //            "btnOk",
        //            new Vector2(10, 90),
        //            40,
        //            "Ok",
        //            Color.White,
        //            Color.Black)
        //            );

        //    //Create CANCEL button
        //    formCollection["form1"].Controls.Add(
        //        new Button(
        //            "btnCancel",
        //            new Vector2(60, 90),
        //            40,
        //            "Cancel",
        //            Color.White,
        //            Color.Black)
        //            );
        //}

    }
}
