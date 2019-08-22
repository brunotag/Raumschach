using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using System.Threading;

namespace Raumschach_Chess
{
    public partial class GamePlayScreen : RaumschachGameScreen
    {
        private Piece SelectedPiece = null;
        private bool IsComputerMoving = false;
        private TimeSpan ComputerStartedThinkingTime;
        private Side MovingSide;

        private BaseCamera camera;
        private Square[, ,] squares = new Square[5, 5, 5];

        private PositionableModel GetClosestClickedModel()
        {
            PositionableModel closestModel = null;
            Cursor cursor = this.ScreenManager.Game.Components.OfType<Cursor>().First();
            Ray cursorRay =
                cursor.CalculateCursorRay(BaseCamera.ActiveCamera.Projection, BaseCamera.ActiveCamera.View);

            // Keep track of the closest object we have seen so far, so we can
            // choose the closest one if there are several models under the cursor.
            float closestIntersection = float.MaxValue;

            List<PositionableModel> piecesAndSelectedSquares = new List<PositionableModel>();
            foreach (Square sq in squares)
            {
                if (sq.Colour == Game.StatusCurrent.ColorSelectedSquares)
                    piecesAndSelectedSquares.Add(sq);
                if (sq.OccupyingPiece != null)
                    piecesAndSelectedSquares.Add(sq.OccupyingPiece);
            }

            // Loop over all our models.
            foreach (PositionableModel mdl in piecesAndSelectedSquares)
            {
                Vector3 vertex1, vertex2, vertex3;

                // Perform the ray to model intersection test.
                float? intersection = Utilities.RayIntersectsModel(cursorRay, mdl.Model,
                                                         mdl.WorldTransform,
                                                         out vertex1, out vertex2,
                                                         out vertex3);
                // Do we have a per-triangle intersection with this model?
                // If so, is it closer than any other model we might have
                // previously intersected?

                if ((intersection != null) && (intersection < closestIntersection))
                {
                    // Store information about this model.
                    closestIntersection = intersection.Value;
                    closestModel = mdl;
                }
            }
            return closestModel;
        }
        
        public void UpdateSquares()
        {
            //int[] board = new int[125];
            //ChessboardLogics.GetChessboard(out board);

            for (int i = 0; i < 125; i++)
            {
                Square sq = squares[
                    4 - (i % 5),
                    4 - (int)(i / 25),
                    4 - (i / 5) % 5
                    ];

                sq.index = i;

                int piece = ChessboardLogics.GetPieceBySquare(i);
                if (piece < 8)
                {
                    PiecesTypes pt = (PiecesTypes)(Math.Abs(piece)-1);
                    SideType st;
                    if (piece < 0)
                        st = SideType.Black;
                    else
                        st = SideType.White;

                    new Piece(pt, Game, sq, st, squares);
                }
                else sq.OccupyingPiece = null;
            }
        }


        public override void LoadContent()
        {
            if (Game.OptionsCurrent.SquareSize <= 0)
            {
                Model pm = Game.Content.Load<Model>(@"Models\Square");
                float squareSize = Utilities.CalculateModelRadius(pm) * 1.13f;

                Game.StatusCurrent.GameOpts = new GameOptions(squareSize, Vector3.Zero);
            }

            //initialise the camera
            camera = new CubicalCamera(50f, Game, 5);

            //initialises the squares
            bool red = false;
            for (int i = 0; i < 5; i++)
            {
                float yPos = Game.OptionsCurrent.SquareSize * (i);
                for (int j = 0; j < 5; j++)
                {
                    float zPos = Game.OptionsCurrent.SquareSize * j;
                    for (int k = 0; k < 5; k++)
                    {
                        float xPos = Game.OptionsCurrent.SquareSize * k;

                        Color color;
                        if (red) color = Game.StatusCurrent.ColorBlackSquares;
                        else color = Game.StatusCurrent.ColorWhiteSquares;
                        red = !red;

                        Square pm2 = new Square((Raumschach)this.Game,
                            new Square.ChessboardPosition(k, i, j))
                            ;

                        pm2.WorldTransform = Matrix.CreateTranslation(
                            Game.OptionsCurrent.LeftBottom + new Vector3(xPos, yPos, zPos)
                            );

                        squares[k, i, j] = pm2;

                    }
                }
            }
            Game.Logics.ResetMatchParameters();
            ChessboardLogics.Init(10000, 8);

            UpdateSquares();

            camera.LoadContent();

            Game.FormCollection["frmGamePlayScreen"].Show();

            base.LoadContent();
        }

        private void DrawSq(Square sq, GameTime gameTime)
        {
            sq.Draw(gameTime);
            if (sq.OccupyingPiece != null)
                sq.OccupyingPiece.Draw(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            switch(Game.StatusCurrent.TransparentSide){
                case TraspSide.InFrontOfBlack:
                    for (int i = 0; i < 5; i++)
                        for (int j = 0; j < 5; j++)
                            for (int k = 0; k < 5; k++)
                            {
                                DrawSq(squares[i, j, k],gameTime);
                            }
                    break;
                case TraspSide.InFrontOfWhite:
                    for (int i = 4; i >= 0; i--)
                        for (int j = 0; j < 5; j++)
                            for (int k = 4; k >= 0; k--)                            
                            {
                                DrawSq(squares[i, j, k], gameTime);
                            }
                    break;
                case TraspSide.RightOfWhite:
                    for (int i = 4; i >= 0; i--)
                        for (int j = 0; j < 5; j++)
                            for (int k = 4; k >= 0; k--)
                            {
                                DrawSq(squares[i, j, k], gameTime);
                            }
                    break;
                case TraspSide.RightOfBlack:
                    for (int i = 0; i < 5; i++)
                        for (int j = 0; j < 5; j++)
                            for (int k = 4; k >= 0; k--)
                            {
                                DrawSq(squares[i, j, k], gameTime);
                            }
                    break;
            }
            camera.Draw(gameTime);
            base.Draw(gameTime);
        }

        private bool IsTKeyDown = false;
        public override void HandleInput(InputState input)
        {
            KeyboardState keybState = Keyboard.GetState();
            if ((keybState.IsKeyUp(Keys.T) && (IsTKeyDown)))
                IsTKeyDown = false;
            if (((keybState.IsKeyDown(Keys.T)) 
                &&
                ((keybState.IsKeyDown(Keys.LeftControl)) || (keybState.IsKeyDown(Keys.RightControl))))
                &&
                (!IsTKeyDown))
            {
                IsTKeyDown = true;
                
                //facciamo in modo che se si tira indietro tutti i giocatori diventino umani
                foreach (Side s in Game.StatusCurrent.Sides)
                    s.PlayerType = PlayerType.Human;

                //ricarichiamo la form in modo che mostri i dati aggiornati
                Game.FormCollection["frmGamePlayScreen"].Show();
                
                ChessboardLogics.Takeback();
                UpdateSquares();
            }

            if (!ChessboardLogics.IsComputerThinking() && (Mouse.GetState().LeftButton == ButtonState.Pressed))
            {
                PositionableModel closestClickedModel = GetClosestClickedModel();

                if (closestClickedModel != null)
                {
                    if (closestClickedModel.GetType() == typeof(Square)) //it is a selected square for sure
                    {
                        Square sq = closestClickedModel as Square;
                        string move = SelectedPiece.OccupiedSquare.ToString() +
                            sq.ToString();

                        if (ChessboardLogics.IsPromo(move))
                        {
                            screenManager.AddScreen(
                                new PromotionSelectionMenuScreen(move, SelectedPiece.OccupiedSquare, sq),
                                this.controllingPlayer
                                );
                        }
                        else
                        {
                            ChessboardLogics.MakeMove(move);
                            Game.StatusCurrent.Result = (PossibleResult)ChessboardLogics.GetResult();
                            //move the piece
                            SelectedPiece.OccupiedSquare.OccupyingPiece = null;
                            sq.OccupyingPiece = SelectedPiece;
                            Game.FormCollection["frmGamePlayScreen"]["lblTurn"].Text = "It's " + Game.StatusCurrent.SideToMove + " turn";

                            //UpdateSquares();
                            //if (Game.StatusCurrent.SideToMove == SideType.White)
                            //    Game.StatusCurrent.SideToMove = SideType.Black;
                            //else
                            //    Game.StatusCurrent.SideToMove = SideType.White;
                        }
                        SelectedPiece.IsSelected = false;
                        SelectedPiece = null;
                        foreach (Square sqr in squares)
                            sqr.IsSelected = false;

                    }
                    else //else it's a piece
                    {
                        Piece clickedPiede = closestClickedModel as Piece;
                        if (clickedPiede.Side == Game.StatusCurrent.SideToMove)
                        {
                            if (SelectedPiece != null) SelectedPiece.IsSelected = false;
                            //select all moveable squares
                            SelectedPiece = closestClickedModel as Piece;
                            SelectedPiece.IsSelected = true;
                            string from = SelectedPiece.OccupiedSquare.ToString();

                            foreach (Square sq in squares)
                            {
                                string to = sq.ToString();
                                if (ChessboardLogics.CanMove(from + to) != 0)
                                    sq.IsSelected = true;
                                else
                                    sq.IsSelected = false;
                            }
                        }
                    }
                }
            }

            base.HandleInput(input);
        }

        bool gameFinished = false;

        public override void Update(GameTime gameTime, bool otherScreenHasFocus, bool coveredByOtherScreen)
        {
            if ((!gameFinished)&&(Game.StatusCurrent.Result != PossibleResult.StillUndetermined))
            { 
                foreach(Side s in Game.StatusCurrent.Sides)
                    s.PlayerType = PlayerType.Human;
                
                String message;
                switch(Game.StatusCurrent.Result){
                    case PossibleResult.BlackWins: message = res.BlackWins;
                        break;
                    case PossibleResult.WhiteWins: message = res.WhiteWins;
                        break;
                    case PossibleResult.DrawByFifty: message = res.DrawByFifty;
                        break;
                    //case PossibleResult.DrawByStalemate: 
                    default:
                    message = res.DrawByStalemate;
                        break;
                }
                
                gameFinished = true;
                
                ThreadPool.QueueUserWorkItem(CloseGame, message);

                //ScreenManager.AddScreen(new MessageBoxScreen(message, false), controllingPlayer);
                //this.ExitScreen();
            }

            if (Game.StatusCurrent.Sides[Game.StatusCurrent.SideToMove].PlayerType == PlayerType.Computer)
            {
                if (!IsComputerMoving)
                {
                    IsComputerMoving = true;
                    MovingSide = Game.StatusCurrent.Sides[Game.StatusCurrent.SideToMove];
                    ComputerStartedThinkingTime = gameTime.TotalGameTime;
                    ThreadPool.QueueUserWorkItem(ExecuteComputerMove);
                }
                else
                { 
                    int remainingComputerSecs = (int)
                        (ComputerStartedThinkingTime +
                        MovingSide.MaxTime -
                        gameTime.TotalGameTime).TotalSeconds;
                    Game.FormCollection["frmGamePlayScreen"]["lblComputerTime"].Text =
                        "Computer will move in " + remainingComputerSecs + " secs";
                }
            }

            foreach (Square sq in squares)
            {
                if (sq.OccupyingPiece != null)
                    sq.OccupyingPiece.Update(gameTime);
            }

            camera.Update(gameTime);
            base.Update(gameTime, otherScreenHasFocus, coveredByOtherScreen);
        }

        void CloseGame(object state)
        {
            Thread.Sleep(4000);

            this.ExitScreen();
            MessageBoxScreen mbs = new MessageBoxScreen((string)state);
            mbs.Accepted += delegate
            {
                ScreenManager.AddScreen(new MainMenuScreen(), this.controllingPlayer);
            };
            mbs.Cancelled += delegate
            {
                this.Game.Exit();
            };
            ScreenManager.AddScreen(mbs, this.controllingPlayer);
        }

        void ExecuteComputerMove(object state)
        {
            if (SelectedPiece != null)
            {
                SelectedPiece.IsSelected = false;
                SelectedPiece = null;
                foreach (Square sqr in squares)
                    sqr.IsSelected = false;
            }

            string move = ChessboardLogics.GetAndMakeNextComputerMove();

            Game.StatusCurrent.Result = (PossibleResult)ChessboardLogics.GetResult();

            string strFrom = move.Substring(0, 3);
            string strTo = move.Substring(3, 3);
            string strPromo = String.Empty;
            
            try{strPromo = move.Substring(6,1);}catch(ArgumentOutOfRangeException){};

            Square from = GetSquareFromString(strFrom);
            Square to = GetSquareFromString(strTo);

            //move the piece                        
            Piece p = from.OccupyingPiece;
            from.OccupyingPiece = null;
            to.OccupyingPiece = p;
            to.OccupyingPiece.PieceType = (PiecesTypes)Math.Abs(ChessboardLogics.GetPieceBySquare(to.index)) - 1;
            //UpdateSquares();

            Game.FormCollection["frmGamePlayScreen"]["lblTurn"].Text = 
                "It's " + Game.StatusCurrent.SideToMove + " turn";
            Game.FormCollection["frmGamePlayScreen"]["lblComputerMove"].Text = 
                "Computer has moved "+strFrom + strTo;
            Game.FormCollection["frmGamePlayScreen"]["lblComputerTime"].Text = String.Empty;

            IsComputerMoving = false;
        }

        private Square GetSquareFromString(string sqr)
        {
            foreach (Square sq in squares)
                if (sq.ToString() == sqr)
                    return sq;
            return null;
        }
    }    


}
