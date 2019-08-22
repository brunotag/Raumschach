using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace Raumschach_Chess
{
    public class Square : PositionableModel
    {
        public int index;

        public struct ChessboardPosition
        {
            public int x;
            public int y;
            public int z;


            public ChessboardPosition(int x, int y, int z)
            {
                this.x = x;
                this.y = y;
                this.z = z;
            }

            public static bool operator == (ChessboardPosition a, ChessboardPosition b)
            {
                if (a.x == b.x) if (a.y == b.y) if (a.z == b.z)
                    return true;
                //else 
                return false;
            }

            public static bool operator !=(ChessboardPosition a, ChessboardPosition b)
            {
                if (!(a == b))
                    return true;
                //else
                return false;
            }

            public override int GetHashCode()
            {
                return x ^ y ^ z;
            }

            public override bool Equals(object obj)
            {
                if (obj is ChessboardPosition)
                    if (this == (ChessboardPosition)obj) return true;
                return false;
            }

            public override string ToString()
            {

                StringBuilder builder = new StringBuilder();
                builder.Append((char)('A' + y));
                builder.Append((char)('e' - x));
                builder.Append((char)('1' + z));

                return builder.ToString();
            }
        }

        public Square(Raumschach game, ChessboardPosition pos)
            : this(game, game.Content, pos)
        {
        }

        public Square(Raumschach game, ContentManager cManager, ChessboardPosition pos)
            : this(game, Matrix.Identity, cManager, pos)
        {
        }
        public Square(Raumschach game, Matrix worldTrans, ChessboardPosition pos)
            : this(game, worldTrans, game.Content, pos)
        {
        }
        public Square(Raumschach game, Matrix worldTrans, ContentManager cManager, ChessboardPosition pos)
            : this(game, worldTrans, cManager, null, pos)
        {
        }

        //THE "true" SET THIS AS A TRANSPARENT POSITIONABLEMODEL
        public Square(Raumschach game, Matrix worldTrans, ContentManager cManager, Piece piece, ChessboardPosition pos)
            : base("square", game, worldTrans, cManager, true)
        {
            if (piece != null) this.OccupyingPiece = piece;
            this.Coordinates = pos;
        }

        public bool IsSelected
        {
            get;
            set;
        }
        public ChessboardPosition Coordinates { get; set; }
        public Color Colour {
            get
            {
                if (!this.IsSelected)
                    return DefaultColor;
                else return game.StatusCurrent.ColorSelectedSquares;
            }
        }
        public Color DefaultColor 
        {
            get
            {
                if (index % 2 == 0)
                    return game.StatusCurrent.ColorWhiteSquares;
                else
                    return game.StatusCurrent.ColorBlackSquares;
            }
        }
        private Piece occPiece = null;
        public Piece OccupyingPiece 
        {
            get
            {
                return occPiece;
            }
            set
            {
                occPiece = value;
            }
        }


        protected override void SetBasicEffectSettings(BasicEffect eff)
        {
            eff.EmissiveColor = Colour.ToVector3() * 0.7f;
            if (this.transparent)
                eff.Alpha = game.StatusCurrent.TranspGradSquares;
            else
                eff.Alpha = 1;

            base.SetBasicEffectSettings(eff);
        }

        public override string ToString()
        {
            return this.Coordinates.ToString();
        }

    }
}
