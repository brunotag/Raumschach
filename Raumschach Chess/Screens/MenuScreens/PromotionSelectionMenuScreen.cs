using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Raumschach_Chess
{
    public class PromotionSelectionMenuScreen : RaumschachMenuScreen
    {
        private string moveToMake;

        private Square from;
        private Square to;

        #region Initialization
        /// <summary>
        /// Constructor fills in the menu contents.
        /// </summary>
        public PromotionSelectionMenuScreen(string move, Square from, Square to)
            : base("Promotion Menu")
        {
            moveToMake = move;
            this.from = from;
            this.to = to;

            // Create our menu entries.
            MenuEntry knightMenuEntry = new MenuEntry("Knight");
            MenuEntry unicornMenuEntry = new MenuEntry("Unicorn");
            MenuEntry bishopMenuEntry = new MenuEntry("Bishop");
            MenuEntry rookMenuEntry = new MenuEntry("Rook");
            MenuEntry queenMenuEntry = new MenuEntry("Queen");

            // Hook up menu event handlers.
            knightMenuEntry.Selected += knightMenuEntry_Selected;
            unicornMenuEntry.Selected += unicornMenuEntry_Selected;
            bishopMenuEntry.Selected += bishopMenuEntry_Selected;
            rookMenuEntry.Selected += rookMenuEntry_Selected;
            queenMenuEntry.Selected += queenMenuEntry_Selected;

            // Add entries to the menu.
            MenuEntries.Add(knightMenuEntry);
            MenuEntries.Add(unicornMenuEntry);
            MenuEntries.Add(bishopMenuEntry);
            MenuEntries.Add(rookMenuEntry);
            MenuEntries.Add(queenMenuEntry);

            this.transitionOffTime = TimeSpan.Zero;
        }

        #endregion

        void queenMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            ChessboardLogics.MakeMove(moveToMake + Piece.PiecesTypesLetters[PiecesTypes.Queen]);
            DoMove(sender, e);
        }

        void rookMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            ChessboardLogics.MakeMove(moveToMake + Piece.PiecesTypesLetters[PiecesTypes.Rook]);
            DoMove(sender, e);
        }

        void bishopMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            ChessboardLogics.MakeMove(moveToMake + Piece.PiecesTypesLetters[PiecesTypes.Bishop]);
            DoMove(sender, e);
        }

        void unicornMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            ChessboardLogics.MakeMove(moveToMake + Piece.PiecesTypesLetters[PiecesTypes.Unicorn]);
            DoMove(sender, e);
        }

        void knightMenuEntry_Selected(object sender, PlayerIndexEventArgs e)
        {
            ChessboardLogics.MakeMove(moveToMake + Piece.PiecesTypesLetters[PiecesTypes.Knight]);
            DoMove(sender, e);
        }

        private void DoMove(object sender, PlayerIndexEventArgs e)
        {
            Game.StatusCurrent.Result = (PossibleResult)ChessboardLogics.GetResult();
            to.OccupyingPiece = from.OccupyingPiece;
            from.OccupyingPiece = null;
            to.OccupyingPiece.PieceType = (PiecesTypes)Math.Abs(ChessboardLogics.GetPieceBySquare(to.index)) - 1;
            OnExit(sender, e);
        }
    }
}
