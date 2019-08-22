using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using Microsoft.Xna.Framework.Net;
using Microsoft.Xna.Framework.Storage;
using xWinFormsLib;

namespace Raumschach_Chess
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public partial class Raumschach : Microsoft.Xna.Framework.Game
    {

        GraphicsDeviceManager graphics;

        private xWinFormsLib.FormCollection formCollection;
        public xWinFormsLib.FormCollection FormCollection
        {
            get
            {
                return formCollection;
            }
        }

        public SpriteBatch spriteBatch;
        public SpriteFont spriteFont;
        private Vector2 fontPos;

        private Dictionary<string, Model> models = new Dictionary<string, Model>();
        public Dictionary<string, Model> Models
        {
            get
            {
                return models;
            }
        }

        private ScreenManager screenManager;
        public ScreenManager ScreenManager
        {
            get
            {
                return screenManager;
            }
        }


        private GameStatus status = new GameStatus();
        public GameStatus StatusCurrent
        {
            get
            {
                return status;
            }
        }

        private ChessboardLogics logics;
        public ChessboardLogics Logics
        {
            get
            {
                return logics;
            }
        }

        public GameOptions OptionsCurrent
        {
            get
            {
                return status.GameOpts;
            }
        }

        public Effect AmbientEffect { get; set; }
        public Texture2D MyTexture { get; set; }

        public Raumschach()
        {
            //this.TargetElapsedTime = new TimeSpan(0, 0, 0, 0, 20);
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";

            logics = new ChessboardLogics(this);
            //Full Screen Mode
            this.graphics.PreferredBackBufferWidth = 1280;
            this.graphics.PreferredBackBufferHeight = 720;

            //this.graphics.IsFullScreen = true;


            // Create the screen manager component.
            screenManager = new ScreenManager(this);

            Components.Add(screenManager);


            screenManager.AddScreen(new MainMenuScreen(), null);      
            //GamePlayScreen gps = new GamePlayScreen();
            //gps.DebugMode = true;
            //screenManager.AddScreen(gps, 0);
            
        }


        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        /// 
        
        protected override void Initialize()
        {
            this.Components.Add(new Cursor(this));
            this.IsMouseVisible = false;


            //this.Components.Add(new FreeMovementCamera(900f, 90f, this, new Vector3(0,0,4000), Vector3.Zero));
            //this.Components.Add(new CubicalCamera(200f, this, chessbrd,5));

            fontPos = new Vector2(1.0f, 1.0f);           

            this.StatusCurrent.GameOpts = new GameOptions();
            formCollection = new xWinFormsLib.FormCollection
    (this.Window, Services, ref graphics);
            
            base.Initialize();            
        }        

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {


            BuildForms(formCollection);

            if ((this.OptionsCurrent == null)||(this.OptionsCurrent.SquareSize<=0))
            {
                Model pm = Content.Load<Model>(@"Models\Square");
                float squareSize = Utilities.CalculateModelRadius(pm) * 1.13f;

                this.StatusCurrent.GameOpts = new GameOptions(squareSize, Vector3.Zero);
            }

            models.Add("square", Content.Load<Model>(@"Models\Square"));
            models.Add("pawn", Content.Load<Model>(@"Models\Pawn2"));
            models.Add("knight", Content.Load<Model>(@"Models\Knight"));
            models.Add("unicorn", Content.Load<Model>(@"Models\Unicorn"));
            models.Add("bishop", Content.Load<Model>(@"Models\Bishop"));
            models.Add("rook", Content.Load<Model>(@"Models\Rook"));
            models.Add("queen", Content.Load<Model>(@"Models\Queen"));
            models.Add("king", Content.Load<Model>(@"Models\King"));

            spriteBatch = new SpriteBatch(GraphicsDevice);
            spriteFont = Content.Load<SpriteFont>(@"Fonts\DemoFont");
            AmbientEffect = Content.Load<Effect>(@"Effects\Texturized");
            MyTexture = Content.Load<Texture2D>(@"Textures\MyTexture");
        }


        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            try
            {
                formCollection.Dispose();
            }
            catch (NullReferenceException)
            {
            }
        }

        
        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            //Update the form collection
            FormCollection.Update(gameTime);

            KeyboardState keybState = Keyboard.GetState();
            if (keybState.IsKeyDown(Keys.LeftShift)||keybState.IsKeyDown(Keys.RightShift))
            {
                //il giro in 4 secondi => PI/2 al secondo
                float rads = (float) (gameTime.ElapsedGameTime.TotalSeconds * (Math.PI/2));

                if (keybState.IsKeyDown(Keys.A))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction, Matrix.CreateRotationY(rads));
                if (keybState.IsKeyDown(Keys.D))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction,Matrix.CreateRotationY(-rads));

                if (keybState.IsKeyDown(Keys.W))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction, Matrix.CreateRotationX(rads));
                if (keybState.IsKeyDown(Keys.S))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction, Matrix.CreateRotationX(-rads));
            }
            if (keybState.IsKeyDown(Keys.LeftShift) || keybState.IsKeyDown(Keys.RightShift))
            {
                //il giro in 4 secondi => PI/2 al secondo
                float rads = (float)(gameTime.ElapsedGameTime.TotalSeconds * (Math.PI / 2));

                if (keybState.IsKeyDown(Keys.A))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction, Matrix.CreateRotationY(rads));
                if (keybState.IsKeyDown(Keys.D))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction, Matrix.CreateRotationY(-rads));

                if (keybState.IsKeyDown(Keys.W))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction, Matrix.CreateRotationX(rads));
                if (keybState.IsKeyDown(Keys.S))
                    StatusCurrent.DirectionalLight0Direction =
                     Vector3.Transform(StatusCurrent.DirectionalLight0Direction, Matrix.CreateRotationX(-rads));
            }
            else
            {
                if (keybState.IsKeyDown(Keys.F1))
                    StatusCurrent.TransparentSide = TraspSide.InFrontOfWhite;
                if (keybState.IsKeyDown(Keys.F2))
                    StatusCurrent.TransparentSide = TraspSide.RightOfWhite;
                if (keybState.IsKeyDown(Keys.F3))
                    StatusCurrent.TransparentSide = TraspSide.InFrontOfBlack;
                if (keybState.IsKeyDown(Keys.F4))
                    StatusCurrent.TransparentSide = TraspSide.RightOfBlack;
                //if (keybState.IsKeyDown(Keys.F11))
                //    StatusCurrent.AlphaTransparency = false;
                //if (keybState.IsKeyDown(Keys.F12))
                //    StatusCurrent.AlphaTransparency = true;
            }

            //Matrix.cre

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            graphics.GraphicsDevice.Clear(Color.Black);

            FormCollection.Render();
            //Draw the form collection

            GraphicsDevice.DepthStencilState = DepthStencilState.DepthRead;

            base.Draw(gameTime);

            FormCollection.Draw();


            #region dati di debug

            //Vector2 mousePosition = new Vector2(Mouse.GetState().X, Mouse.GetState().Y);
            //System.Text.StringBuilder buffer = new System.Text.StringBuilder();
            //CubicalCamera camera = (CubicalCamera)BaseCamera.ActiveCamera;

            //if (camera == null) return;

            //buffer.AppendFormat("Floor = {0}\n", camera.Floor);
            //buffer.AppendFormat("X = {0}\n", camera.finalPosition.X / camera.step);
            //buffer.AppendFormat("Y = {0}\n", camera.finalPosition.Y / camera.step);
            //buffer.AppendFormat("Z = {0}\n", camera.finalPosition.Z / camera.step);
            //buffer.AppendFormat("Mouse = {0} {1}\n", mousePosition.X, mousePosition.Y);
            //buffer.AppendFormat("View =\n| {0} {1} {2} {3} |\n",
            //    BaseCamera.ActiveCamera.View.M11,
            //    BaseCamera.ActiveCamera.View.M12,
            //    BaseCamera.ActiveCamera.View.M13,
            //    BaseCamera.ActiveCamera.View.M14);
            //buffer.AppendFormat("| {0} {1} {2} {3} |\n",
            //    BaseCamera.ActiveCamera.View.M21,
            //    BaseCamera.ActiveCamera.View.M22,
            //    BaseCamera.ActiveCamera.View.M23,
            //    BaseCamera.ActiveCamera.View.M24);
            //buffer.AppendFormat("| {0} {1} {2} {3} |\n",
            //    BaseCamera.ActiveCamera.View.M31,
            //    BaseCamera.ActiveCamera.View.M32,
            //    BaseCamera.ActiveCamera.View.M33,
            //    BaseCamera.ActiveCamera.View.M34);
            //buffer.AppendFormat("| {0} {1} {2} {3} |\n",
            //    BaseCamera.ActiveCamera.View.M41,
            //    BaseCamera.ActiveCamera.View.M42,
            //    BaseCamera.ActiveCamera.View.M43,
            //    BaseCamera.ActiveCamera.View.M44);

            //Vector3 cameraPos = GraphicsDevice.Viewport.Unproject(
            //    BaseCamera.ActiveCamera.View.Translation,
            //    BaseCamera.ActiveCamera.Projection,
            //    BaseCamera.ActiveCamera.View,
            //    Matrix.Identity);

            //buffer.AppendFormat("View.Translation: {0},{1},{2} \n",
            //    BaseCamera.ActiveCamera.View.Translation.X,
            //    BaseCamera.ActiveCamera.View.Translation.Y,
            //    BaseCamera.ActiveCamera.View.Translation.Z
            //    );
            //buffer.AppendFormat("View.Translation Unprojected: {0},{1},{2} \n",
            //    cameraPos.X,
            //    cameraPos.Y,
            //    cameraPos.Z
            //);
            //Vector3 translationReproj = GraphicsDevice.Viewport.Project(
            //    cameraPos,
            //    BaseCamera.ActiveCamera.Projection,
            //    BaseCamera.ActiveCamera.View,
            //    Matrix.Identity
            //    );
            //buffer.AppendFormat("View.Translation Reprojected: {0},{1},{2} \n",
            //    translationReproj.X,
            //    translationReproj.Y,
            //    translationReproj.Z
            //    );

            //spriteBatch.Begin(SpriteBlendMode.AlphaBlend, SpriteSortMode.Deferred, SaveStateMode.SaveState);
            //spriteBatch.DrawString(spriteFont, buffer.ToString(), fontPos, Color.Yellow);
            //spriteBatch.End();

            #endregion

        }




    }
}

