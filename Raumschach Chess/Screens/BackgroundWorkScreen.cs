using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using System.Threading;

namespace Raumschach_Chess
{
    public class BackgroundWorkScreen : RaumschachGameScreen
    {
        private string message;
        private object state;
        private Action<object> callback;

        public BackgroundWorkScreen(string message, Action<object> callback, object state )
        {
            this.message = message;
            this.callback = callback;
            this.state = state;
            ThreadPool.QueueUserWorkItem(DoWork, this);
        }

        private static void DoWork(object bgWorkScreen)
        {
            BackgroundWorkScreen bgws = (BackgroundWorkScreen)bgWorkScreen;
            bgws.callback.Invoke(bgws.state);
            bgws.ExitScreen();
        }

        public override void Draw(Microsoft.Xna.Framework.GameTime gameTime)
        {
            SpriteBatch spriteBatch = ScreenManager.SpriteBatch;
            SpriteFont font = ScreenManager.Font;

            // Center the text in the viewport.
            Viewport viewport = ScreenManager.GraphicsDevice.Viewport;
            Vector2 viewportSize = new Vector2(viewport.Width, viewport.Height);
            Vector2 textSize = font.MeasureString(message);
            Vector2 textPosition = (viewportSize - textSize) / 2;

            Color color = new Color(255, 255, 255, TransitionAlpha);

            // Draw the text.
            spriteBatch.Begin();
            spriteBatch.DrawString(font, message, textPosition, color);
            spriteBatch.End();
            base.Draw(gameTime);
        }
    }
}
