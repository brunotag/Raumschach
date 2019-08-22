using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace Raumschach_Chess
{
    public class PositionableModel 
    {
        private Model model;
        private ContentManager contentManager;
        protected bool transparent;
        protected Raumschach game;

        public virtual Matrix WorldTransform
        {
            get;
            set;
        }

        public Model Model
        {
            get { return model; }
            set { model = value; }
        }

   
        #region Constructors

        public PositionableModel(String modelName, Raumschach game, bool transparent)
            : this(modelName, game, Matrix.Identity, game.Content, transparent)
        {
        }

        public PositionableModel(String modelName, Raumschach game, ContentManager cManager, bool transparent)
            : this(modelName, game, Matrix.Identity, cManager, transparent)
        {
        }

        public PositionableModel(String modelName, Raumschach game, Matrix worldTrans, bool transparent)
            : this(modelName, game, worldTrans, game.Content, transparent)
        {
        }

        public PositionableModel(String modelName, Raumschach game, Matrix worldTrans, ContentManager cManager, bool transparent)
            
        {
            this.WorldTransform = worldTrans;
            this.contentManager = cManager;
            this.transparent = transparent;
            this.model = game.Models[modelName.ToLower()];
            this.game = game;
        }

        #endregion

        public virtual void Draw(GameTime gameTime)
        {
            //Utilities.SetAlphaTransparency(this.transparent, this.game);
               
            Matrix[] transforms = new Matrix[model.Bones.Count];
            model.CopyAbsoluteBoneTransformsTo(transforms);
            foreach (ModelMesh mesh in Model.Meshes)
            {
                Matrix world = transforms[mesh.ParentBone.Index] * WorldTransform;

                foreach (BasicEffect eff in mesh.Effects)
                {
                    eff.World = world;
                    eff.Projection = BaseCamera.ActiveCamera.Projection;
                    eff.View = BaseCamera.ActiveCamera.View;
                    SetBasicEffectSettings(eff);
                }
                mesh.Draw();
            }

            //Utilities.SetAlphaTransparency(false, game);
        }

        public virtual void Update(GameTime gameTime)
        {
        }

        protected virtual void SetBasicEffectSettings(BasicEffect eff)
        {
            eff.LightingEnabled = true;

            eff.DirectionalLight0.Enabled = true;
            eff.DirectionalLight0.Direction = game.StatusCurrent.DirectionalLight0Direction;
            eff.DirectionalLight0.DiffuseColor = Color.White.ToVector3() * 0.5f;
            eff.DirectionalLight0.SpecularColor = Color.White.ToVector3();

            eff.DirectionalLight1.Enabled = true;
            eff.DirectionalLight1.Direction = game.StatusCurrent.DirectionalLight1Direction;
            eff.DirectionalLight1.DiffuseColor = Color.Yellow.ToVector3() * 0.5f;
            eff.DirectionalLight1.SpecularColor = Color.Yellow.ToVector3();

            //eff.TextureEnabled = true;
            //eff.Texture = ((Raumschach)this.Game).MyTexture;

            //eff.EmissiveColor = col.ToVector3() * 0.7f;

        }
       
    }
}
