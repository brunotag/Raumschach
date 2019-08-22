using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;

namespace Raumschach_Chess
{
    [Serializable]
    public class GameOptions
    {

        [Serializable]
        public enum GameTypeOption
        {
            Local,
            Network
        }
       

        public string WhitePlayerName
        {
            get;
            set;
        }

        public string BlackPlayerName
        {
            get;
            set;
        }

        public Color DarkSquareColor
        {
            get;
            set;
        }

        public Color LightSquareColor
        {
            get;
            set;
        }
        private float squareSize;
        private Microsoft.Xna.Framework.Vector3 leftBottom;

        public float SquareSize
        {
            get
            {
                return squareSize;
            }
        }

        public Microsoft.Xna.Framework.Vector3 LeftBottom
        {
            get
            {
                return leftBottom;
            }
        }


        public GameTypeOption GameType
        {
            get;
            set;
        }

        public GameOptions()
        {
            WhitePlayerName = "Player1";
            BlackPlayerName = "Player2";
            DarkSquareColor = Color.Red;
            LightSquareColor = Color.Blue;
            this.squareSize = 0;
            this.leftBottom = Microsoft.Xna.Framework.Vector3.Zero;
            GameType = GameTypeOption.Local;

        }

        public GameOptions(float squareSize, Microsoft.Xna.Framework.Vector3 leftBottom):this()
        {
            this.squareSize = squareSize;
            this.leftBottom = leftBottom;
        }

    }
}
