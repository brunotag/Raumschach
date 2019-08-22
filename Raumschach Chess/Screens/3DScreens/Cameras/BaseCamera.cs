using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;

namespace Raumschach_Chess
{
    public abstract class BaseCamera
    {
        protected static BaseCamera activeCamera = null;

        // View and projection
        protected Matrix projection = Matrix.Identity;
        protected Matrix view = Matrix.Identity;

        //
        protected const float angle = 0.5f;
        protected float turnSpeed;

        protected Game Game;


        public BaseCamera(Game game)
        {
            if (ActiveCamera == null)
                ActiveCamera = this;
            this.Game = game;
        }

        public BaseCamera(float turnspeed, Game game):this(game)
        {
            this.turnSpeed = turnspeed;
        }

        public BaseCamera(float turnspeed, Game game, Vector3 initialPosition)
            : this(turnspeed,game)
        {
            view *= Matrix.CreateTranslation(initialPosition);
        }

        public BaseCamera(float turnspeed, Game game, Vector3 initialPosition, Vector3 initialTarget)
            : this(turnspeed,game)
        {
            view *= Matrix.CreateLookAt(initialPosition, initialTarget, Vector3.UnitY);
        }
        public static BaseCamera ActiveCamera
        {
            get { return activeCamera; }
            set { activeCamera = value; }
        }

        public Matrix Projection
        {
            get { return projection; }
        }

        public Matrix View
        {
            get { return view; }
        }

        public virtual void Update(GameTime gameTime)
        {
        }

        public virtual void LoadContent()
        {
            float ratio = (float)this.Game.GraphicsDevice.Viewport.Width / (float)this.Game.GraphicsDevice.Viewport.Height;
            projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.PiOver4, ratio, 10, 10000);
        }

        public virtual void Draw(GameTime gameTime)
        {
        }
    }
}
