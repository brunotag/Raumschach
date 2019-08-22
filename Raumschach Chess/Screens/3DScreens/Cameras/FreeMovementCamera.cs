using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;

namespace Raumschach_Chess
{
    public class FreeMovementCamera : BaseCamera
    {
        protected float speed;
        public FreeMovementCamera(Game game):base(game)
        {
        }

        public FreeMovementCamera(float speed, float turnspeed, Game game):base(turnspeed, game)
        {
            this.speed = speed;
        }

        public FreeMovementCamera(float speed, float turnspeed, Game game, Vector3 initialPosition)
            : base(turnspeed,game,initialPosition)
        {
            this.speed = speed;
        }

        public FreeMovementCamera(float speed, float turnspeed, Game game, Vector3 initialPosition, Vector3 initialTarget)
            : base(turnspeed, game, initialPosition,initialTarget)
        {
            this.speed = speed;
        }

        public override void Update(GameTime gameTime)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            KeyboardState keyboard = Keyboard.GetState();

            float deltaPitch = 0;
            float deltaYaw = 0;
            float deltaRoll = 0;
            float distance = 0;

            if (keyboard.IsKeyDown(Keys.Up))
                deltaPitch -= angle * turnSpeed * delta;

            if (keyboard.IsKeyDown(Keys.Down))
                deltaPitch += angle * turnSpeed * delta;

            if (keyboard.IsKeyDown(Keys.Left))
                deltaYaw -= angle * turnSpeed * delta;
            if (keyboard.IsKeyDown(Keys.Right))
                deltaYaw += angle * turnSpeed * delta;

            if (keyboard.IsKeyDown(Keys.Q))
                deltaRoll -= angle * turnSpeed * delta;
            if (keyboard.IsKeyDown(Keys.W))
                deltaRoll += angle * turnSpeed * delta;

            if (keyboard.IsKeyDown(Keys.A))
                distance += speed * delta;
            if (keyboard.IsKeyDown(Keys.Z))
                distance -= speed * delta;

            if (keyboard.IsKeyDown(Keys.Escape))
                Game.Exit();

            Matrix Rotx = Matrix.CreateRotationX(MathHelper.ToRadians(deltaPitch));
            Matrix Roty = Matrix.CreateRotationY(MathHelper.ToRadians(deltaYaw));
            Matrix Rotz = Matrix.CreateRotationZ(MathHelper.ToRadians(deltaRoll));

            view *= Rotx * Roty * Rotz;
            Matrix matrisce = Matrix.CreateTranslation(new Vector3(2, 3, 4) * 1000);
            view *= Matrix.CreateTranslation(Vector3.UnitZ * distance);
        }

    }
        
}
