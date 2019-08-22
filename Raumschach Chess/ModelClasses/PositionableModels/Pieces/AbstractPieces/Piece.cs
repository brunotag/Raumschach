using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Raumschach_Chess
{
    public class Piece : PositionableModel
    {
        public char Letter
        {
            get
            {
                return PiecesTypesLetters[this.PieceType];
            }
        }

        public static Dictionary<PiecesTypes, Char> PiecesTypesLetters =
        new Dictionary<PiecesTypes, Char>(){
            {PiecesTypes.Pawn, 'P'},                                                           
            {PiecesTypes.Knight, 'N'},  
            {PiecesTypes.Unicorn, 'U'},  
            {PiecesTypes.Bishop, 'B'}, 
            {PiecesTypes.Rook, 'R'},  
            {PiecesTypes.Queen, 'Q'},  
            {PiecesTypes.King, 'K'}  
        };

        public static Dictionary<PiecesTypes, String> PiecesTypesModels =
        new Dictionary<PiecesTypes, String>(){
            {PiecesTypes.Pawn, "Pawn"},                                                           
            {PiecesTypes.Knight, @"Knight"},  
            {PiecesTypes.Unicorn, @"Unicorn"},  
            {PiecesTypes.Bishop, @"Bishop"}, 
            {PiecesTypes.Rook, @"Rook"},  
            {PiecesTypes.Queen, @"Queen"},  
            {PiecesTypes.King, @"King"}  
        };

        //based on 
        private Matrix wTrans;
        public override Matrix WorldTransform
        {
            get
            {
                return this.wTrans;
            }
        }

        private Matrix FinalWorldTransform
        {
            get
            {
                return OccupiedSquare.WorldTransform * Matrix.CreateTranslation(new Vector3(0, 26, 0));
            }
        }

        public Square OccupiedSquare
        {
            get
            {
                foreach (Square sq in squares)
                {
                    if (sq.OccupyingPiece != null)
                        if (sq.OccupyingPiece == this)
                            return sq;
                }
                return null;
            }
        }

        protected SideType side;
        public SideType Side
        {
            get
            {
                return side;
            }
            set
            {
                side = value;
            }
        }

        public bool IsSelected
        {
            get;
            set;
        }

        private Square[,,] squares;

        private PiecesTypes pieceType;
        public PiecesTypes PieceType
        {
            get
            {
                return pieceType;
            }
            set
            {
                pieceType = value;
                Model = game.Models[PiecesTypesModels[pieceType].ToLower()];
            }
        }

        public Piece(PiecesTypes type, Raumschach game, Square occ, SideType side, Square[,,] sqrs)
            : this(type, game, game.Content, occ, side, sqrs)
        {
        }

        //THE "false" SET THIS AS A NON TRANSPARENT POSITIONABLEMODEL
        public Piece(PiecesTypes type, Raumschach game, ContentManager cManager, Square occ, SideType side, Square[, ,] sqrs)
            : base(PiecesTypesModels[type], game, cManager, true)
        {
            occ.OccupyingPiece = this;
            squares = sqrs;
            this.side = side;
            this.PieceType = type;
            this.wTrans = FinalWorldTransform;
        }

        private bool animating = false;
        private Matrix zeroMatrix = new Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        private Matrix animDiff = new Matrix(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
        private Matrix wTransAtAnimStartTime;
        private TimeSpan animStartTime;

        public override void Update(GameTime gameTime)
        {
            if (!animating)
            {
                animDiff = FinalWorldTransform - wTrans;
                if (animDiff != zeroMatrix)
                {
                    animating = true;
                    animStartTime = gameTime.TotalGameTime;
                    wTransAtAnimStartTime = wTrans;
                }
            }
            else
            {       
                double msecsElaps = gameTime.TotalGameTime.TotalMilliseconds - animStartTime.TotalMilliseconds;
                if (msecsElaps >= 2000)
                {
                    animating = false;
                    wTrans = FinalWorldTransform;
                }
                else
                {
                    wTrans = wTransAtAnimStartTime + ((int)msecsElaps) * (animDiff / 2000);
                }
            }


            base.Update(gameTime);
        }

        protected override void SetBasicEffectSettings(BasicEffect eff)
        {
            GameStatus status = ((Raumschach)this.game).StatusCurrent;
            if (IsSelected)
                eff.EmissiveColor = status.ColorSelectedPieces.ToVector3();                    
            else
            {
                if (Side == SideType.Black)
                    eff.EmissiveColor = status.ColorBlackPieces.ToVector3();
                else
                    eff.EmissiveColor = status.ColorWhitePieces.ToVector3();
            }
            if (this.transparent)
                eff.Alpha = game.StatusCurrent.TranspGradPieces;
            else
                eff.Alpha = 1;

            base.SetBasicEffectSettings(eff);
        }


        public override string ToString()
        {
            return this.Letter.ToString();
        }

    }
}
