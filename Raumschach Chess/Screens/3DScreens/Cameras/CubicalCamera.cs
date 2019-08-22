using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Raumschach_Chess
{
    public class CubicalCamera : BaseCamera
    {
        public int CubeEdges
        {
            get{ return cubeEdges;}
        }

        private int floor = 0;
        public int Floor
        {
            get { return floor; }
            set {
                int reference = CubeEdges;
                if (value <= -1) value = reference + value % reference;
                floor = value % reference;
            }
        }

        private int horizontalPosition = 0;
        public int HorizontalPosition
        {
            get { return horizontalPosition; }
            set { 
                int reference = CubeEdges * 4;
                if (value <= -1) value = reference + value % reference;
                horizontalPosition = value % reference; }
        }

        public float ZoomAdder
        {
            get;
            set;
        }

        public float step;
        private int cubeEdges;
        private Vector3 defaultPosition;
        
        private Vector3 finalTargetPosition;
        private Vector3 initialTargetPosition;

        private bool isMoving = false;
        private TimeSpan initialMovementTime;
        private float secondsToDoMovement = 1;
        public Vector3 finalPosition;
        private Vector3 initialPosition;

        public CubicalCamera(float turnspeed, Game game, int cubeEdges)
            :base(turnspeed, game)
        {
            this.cubeEdges = cubeEdges;
            ZoomAdder = 1;
        }

        public override void Update(GameTime gameTime)
        {
            float delta = (float)gameTime.ElapsedGameTime.TotalSeconds;

            KeyboardState keyboard = Keyboard.GetState();

            if (keyboard.IsKeyDown(Keys.Escape))
                Game.Exit();

            float deltaPitch = 0;
            float deltaYaw = 0;
            
            //float deltaRoll = 0;


            //se non ci si sta muovendo si inizia una fase di movimento
            if (!this.isMoving)
            {
                if (keyboard.IsKeyDown(Keys.W))
                    deltaPitch -= angle * turnSpeed * delta;
                if (keyboard.IsKeyDown(Keys.S))
                    deltaPitch += angle * turnSpeed * delta;
                if (keyboard.IsKeyDown(Keys.A))
                    deltaYaw -= angle * turnSpeed * delta;
                if (keyboard.IsKeyDown(Keys.D))
                    deltaYaw += angle * turnSpeed * delta;
                /*
if (keyboard.IsKeyDown(Keys.Q))
    deltaRoll -= angle * turnSpeed * delta;
if (keyboard.IsKeyDown(Keys.s))
    deltaRoll += angle * turnSpeed * delta;
 */

                if (keyboard.IsKeyDown(Keys.PageDown))
                {
                    ZoomAdder += delta*100; StartMovement(0.1f); initialMovementTime = gameTime.TotalGameTime;
                }
                if (keyboard.IsKeyDown(Keys.PageUp))
                {
                    ZoomAdder -= delta * 100; StartMovement(0.1f); initialMovementTime = gameTime.TotalGameTime;
                }

                if (keyboard.IsKeyDown(Keys.Space))
                {
                    view = Matrix.CreateLookAt(finalPosition, finalTargetPosition, Vector3.Up);
                }

                if (keyboard.IsKeyDown(Keys.Up))
                {
                    Floor++; StartMovement(1); initialMovementTime = gameTime.TotalGameTime;
                }
                if (keyboard.IsKeyDown(Keys.Down))
                {
                    Floor--; StartMovement(1); initialMovementTime = gameTime.TotalGameTime;
                }
                if (keyboard.IsKeyDown(Keys.Left))
                {
                    HorizontalPosition--; StartMovement(1); initialMovementTime = gameTime.TotalGameTime;
                }
                if (keyboard.IsKeyDown(Keys.Right))
                {
                    HorizontalPosition++; StartMovement(1); initialMovementTime = gameTime.TotalGameTime;
                }
            }
            else
            {//altrimenti si continua il movimento precedente, passo passo attorno al cubo                
                float amount = (float)(gameTime.TotalGameTime.TotalSeconds - 
                    initialMovementTime.TotalSeconds) / secondsToDoMovement;

                if (amount >= 1)
                {
                    view = Matrix.CreateLookAt(finalPosition, finalTargetPosition, Vector3.Up);
                    this.isMoving = false;
                }
                else
                    view = Matrix.CreateLookAt(
                        initialPosition + (finalPosition - initialPosition) * amount,
                        initialTargetPosition + (finalTargetPosition - initialTargetPosition) * amount, 
                        Vector3.Up);
            }            

            //aggiungo il contributo della rotazione su sé stessa della camera
            //soloo se non è premuto shift

            if (keyboard.IsKeyDown(Keys.LeftShift) || keyboard.IsKeyDown(Keys.RightShift))
                return;
            Matrix Rotx = Matrix.CreateRotationX(MathHelper.ToRadians(deltaPitch));
            Matrix Roty = Matrix.CreateRotationY(MathHelper.ToRadians(deltaYaw));
            //Matrix Rotz = Matrix.CreateRotationZ(MathHelper.ToRadians(deltaRoll));

            view *= Rotx * Roty; // * Rotz;
            //view *= Matrix.CreateTranslation(Vector3.UnitZ * distance);
        }

        public override void LoadContent()
        {
            step = ((Raumschach)Game).OptionsCurrent.SquareSize;
            defaultPosition = ((Raumschach)Game).OptionsCurrent.LeftBottom +new Vector3(-step, step / 2, -step);            

            finalPosition = defaultPosition;

            view = Matrix.CreateLookAt(defaultPosition, new Vector3(- 2*defaultPosition.X,0,-2*defaultPosition.Z) , Vector3.Up);

            //StartMovement();
            HorizontalPosition--; StartMovement(1); initialMovementTime = TimeSpan.Zero;            

            base.LoadContent();
        }
        float x, y, z;
        float xFact, zFact;
        
        private void StartMovement(float secs)
        {

            xFact = zFact = 0;

            float xTarget, zTarget;
            y = floor;
            if (HorizontalPosition < cubeEdges)
            {
                z = zTarget = horizontalPosition + 1f;
                x = -2;
                xTarget = x + 100;
                xFact = ZoomAdder*(-1);

            }
            else if ((HorizontalPosition >= cubeEdges) && (HorizontalPosition < 2 * cubeEdges))
            {
                z = cubeEdges + 3f;
                x = xTarget = horizontalPosition - 4f;
                zTarget = z - 100;
                zFact = ZoomAdder;
            }
            else if ((HorizontalPosition >= 2 * cubeEdges) && (HorizontalPosition < 3 * cubeEdges))
            {
                z = zTarget = 3 * cubeEdges - horizontalPosition ;
                x = cubeEdges + 3f;
                xTarget = x - 100;
                xFact = ZoomAdder;
            }
            else
            {
                z = -2;
                x = xTarget = 4 * cubeEdges - horizontalPosition ;
                zTarget = z + 100;
                zFact = ZoomAdder * (-1);
            }

            initialPosition = finalPosition;
            finalPosition = new Vector3(step * ( x + xFact/3), step * y, step * (z + zFact/3)) + defaultPosition;
            initialTargetPosition = finalTargetPosition;
            finalTargetPosition = new Vector3(step * xTarget, step * y, step * zTarget) + defaultPosition;

            isMoving = true;
        }
        
        public override void Draw(GameTime gameTime)
        {
            //Only for debugging purpose
            //StringBuilder sBuild = new StringBuilder();

            //sBuild.Append("x = ");
            //sBuild.Append(x.ToString());
            //sBuild.Append("\nz = ");
            //sBuild.Append(z.ToString());
            //sBuild.Append("\nxFact = ");
            //sBuild.Append(xFact.ToString());
            //sBuild.Append("\nzFact = ");
            //sBuild.Append(zFact.ToString());

            //((Raumschach)this.Game).spriteBatch.Begin(SpriteBlendMode.AlphaBlend, SpriteSortMode.Deferred, SaveStateMode.SaveState);
            //((Raumschach)this.Game).spriteBatch.DrawString(((Raumschach)this.Game).spriteFont, sBuild.ToString(), new Vector2(1.0f, 1.0f), Color.Yellow);
            //((Raumschach)this.Game).spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
