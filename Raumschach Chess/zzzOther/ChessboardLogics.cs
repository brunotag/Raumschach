using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public class ChessboardLogics
    {
        private Raumschach Game;

        public ChessboardLogics(Raumschach game)
        {
            this.Game = game;
        }

        public void ResetMatchParameters()
        {
            Game.StatusCurrent.SideToMove = SideType.White;
            Game.StatusCurrent.HalfMoveFiftyCounter = 0;
            Game.StatusCurrent.Result = PossibleResult.StillUndetermined;
        }



        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        [return: System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
        public static extern String GetMoveFromFEN(                       
            int sideToMove,
            int maxTime,
            [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
                String FENRepresentation
            );

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int MakeMove(
            [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
                String move
        );

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        [return: System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
        public static extern String MakeMoveAndGetNext(
            [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
                String move,
            int sideToMove
            );

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void Takeback();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void Init(
            int maxTime,
            int maxDepth
            );

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void GetChessboard(                
            out int[] board                
        );

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetPieceBySquare(int i);

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int CanMove(String move);

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern String GetAndMakeNextComputerMove();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern String GetNextComputerMove();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern bool IsPromo(
            [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
            string move
            );

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetResult();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void SetChessboardToFEN(
            [System.Runtime.InteropServices.MarshalAs(System.Runtime.InteropServices.UnmanagedType.LPStr)] 
            string FEN
            );

        #region Status Getter Setter

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetSideToMove();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void SetSideToMove(int sideToMove);

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetFifty();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void SetFifty(int fifty);

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetMaxTimeWhite();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void SetMaxTimeWhite(int mTime);

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetMaxTimeBlack();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void SetMaxTimeBlack(int mTime);

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetMaxDepthWhite();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void SetMaxDepthWhite(int mDepth);

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern int GetMaxDepthBlack();

        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern void SetMaxDepthBlack(int mDepth);
        
        [System.Runtime.InteropServices.DllImport("RaumschachChessEngine.dll")]
        public static extern bool IsComputerThinking();


        #endregion


        internal static PiecesTypes GetPieceBySquare(Square from)
        {
            throw new NotImplementedException();
        }
    }
}
