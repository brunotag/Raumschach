using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Lidgren.Network;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.IO;

namespace Raumschach_Chess
{
    public static class Utilities
    {
        public static int PortNumber = 11223;

        //public static void SetAlphaTransparency(bool set, Game game)
        //{
        //    if (((Raumschach)game).StatusCurrent.AlphaTransparency)
        //    {
        //        if (set)
        //        {
        //            //Activates Alpha Transparency

        //            game.GraphicsDevice.RenderState.AlphaBlendEnable = true;
        //            game.GraphicsDevice.RenderState.SourceBlend = Blend.One;
        //            game.GraphicsDevice.RenderState.DestinationBlend = Blend.One;
        //            game.GraphicsDevice.RenderState.BlendFunction = BlendFunction.Add;
        //        }
        //        else
        //        {
        //            game.GraphicsDevice.RenderState.AlphaBlendEnable = false;
        //        }
        //    }
        //}




        public static float CalculateModelRadius(Model model)
        {

            // Look up the absolute bone transforms for this model.
            Matrix[] boneTransforms = new Matrix[model.Bones.Count];

            model.CopyAbsoluteBoneTransformsTo(boneTransforms);

            // Compute an (approximate) model center position by
            // averaging the center of each mesh bounding sphere.
            Vector3 modelCenter = Vector3.Zero;

            foreach (ModelMesh mesh in model.Meshes)
            {
                BoundingSphere meshBounds = mesh.BoundingSphere;
                Matrix transform = boneTransforms[mesh.ParentBone.Index];
                Vector3 meshCenter = Vector3.Transform(meshBounds.Center, transform);

                modelCenter += meshCenter;
            }

            modelCenter /= model.Meshes.Count;

            float modelRadius = 0;

            foreach (ModelMesh mesh in model.Meshes)
            {
                BoundingSphere meshBounds = mesh.BoundingSphere;
                Matrix transform = boneTransforms[mesh.ParentBone.Index];
                Vector3 meshCenter = Vector3.Transform(meshBounds.Center, transform);

                float transformScale = transform.Forward.Length();

                float meshRadius = (meshCenter - modelCenter).Length() +
                                   (meshBounds.Radius * transformScale);

                modelRadius = Math.Max(modelRadius, meshRadius);
            }

            return modelRadius;
        }

        /// <summary>
        /// Checks whether a ray intersects a triangle. This uses the algorithm
        /// developed by Tomas Moller and Ben Trumbore, which was published in the
        /// Journal of Graphics Tools, volume 2, "Fast, Minimum Storage Ray-Triangle
        /// Intersection".
        /// 
        /// This method is implemented using the pass-by-reference versions of the
        /// XNA math functions. Using these overloads is generally not recommended,
        /// because they make the code less readable than the normal pass-by-value
        /// versions. This method can be called very frequently in a tight inner loop,
        /// however, so in this particular case the performance benefits from passing
        /// everything by reference outweigh the loss of readability.
        /// </summary>
        static void RayIntersectsTriangle(ref Ray ray,
                                          ref Vector3 vertex1,
                                          ref Vector3 vertex2,
                                          ref Vector3 vertex3, out float? result)
        {
            // Compute vectors along two edges of the triangle.
            Vector3 edge1, edge2;

            Vector3.Subtract(ref vertex2, ref vertex1, out edge1);
            Vector3.Subtract(ref vertex3, ref vertex1, out edge2);

            // Compute the determinant.
            Vector3 directionCrossEdge2;
            Vector3.Cross(ref ray.Direction, ref edge2, out directionCrossEdge2);

            float determinant;
            Vector3.Dot(ref edge1, ref directionCrossEdge2, out determinant);

            // If the ray is parallel to the triangle plane, there is no collision.
            if (determinant > -float.Epsilon && determinant < float.Epsilon)
            {
                result = null;
                return;
            }

            float inverseDeterminant = 1.0f / determinant;

            // Calculate the U parameter of the intersection point.
            Vector3 distanceVector;
            Vector3.Subtract(ref ray.Position, ref vertex1, out distanceVector);

            float triangleU;
            Vector3.Dot(ref distanceVector, ref directionCrossEdge2, out triangleU);
            triangleU *= inverseDeterminant;

            // Make sure it is inside the triangle.
            if (triangleU < 0 || triangleU > 1)
            {
                result = null;
                return;
            }

            // Calculate the V parameter of the intersection point.
            Vector3 distanceCrossEdge1;
            Vector3.Cross(ref distanceVector, ref edge1, out distanceCrossEdge1);

            float triangleV;
            Vector3.Dot(ref ray.Direction, ref distanceCrossEdge1, out triangleV);
            triangleV *= inverseDeterminant;

            // Make sure it is inside the triangle.
            if (triangleV < 0 || triangleU + triangleV > 1)
            {
                result = null;
                return;
            }

            // Compute the distance along the ray to the triangle.
            float rayDistance;
            Vector3.Dot(ref edge2, ref distanceCrossEdge1, out rayDistance);
            rayDistance *= inverseDeterminant;

            // Is the triangle behind the ray origin?
            if (rayDistance < 0)
            {
                result = null;
                return;
            }

            result = rayDistance;
        }


        /// <summary>
        /// Checks whether a ray intersects a model. This method needs to access
        /// the model vertex data, so the model must have been built using the
        /// custom TrianglePickingProcessor provided as part of this sample.
        /// Returns the distance along the ray to the point of intersection, or null
        /// if there is no intersection.
        /// </summary>
        public static float? RayIntersectsModel(Ray ray, Model model, Matrix modelTransform,
                                         out Vector3 vertex1, out Vector3 vertex2,
                                         out Vector3 vertex3)
        {
            vertex1 = vertex2 = vertex3 = Vector3.Zero;

            // The input ray is in world space, but our model data is stored in object
            // space. We would normally have to transform all the model data by the
            // modelTransform matrix, moving it into world space before we test it
            // against the ray. That transform can be slow if there are a lot of
            // triangles in the model, however, so instead we do the opposite.
            // Transforming our ray by the inverse modelTransform moves it into object
            // space, where we can test it directly against our model data. Since there
            // is only one ray but typically many triangles, doing things this way
            // around can be much faster.

            Matrix inverseTransform = Matrix.Invert(modelTransform);

            ray.Position = Vector3.Transform(ray.Position, inverseTransform);
            ray.Direction = Vector3.TransformNormal(ray.Direction, inverseTransform);

            // Look up our custom collision data from the Tag property of the model.
            Dictionary<string, object> tagData = (Dictionary<string, object>)model.Tag;

            if (tagData == null)
            {
                throw new InvalidOperationException(
                    "Model.Tag is not set correctly. Make sure your model " +
                    "was built using the custom TrianglePickingProcessor.");
            }

            // Start off with a fast bounding sphere test.
            BoundingSphere boundingSphere = (BoundingSphere)tagData["BoundingSphere"];

            if (boundingSphere.Intersects(ray) == null)
            {
                // If the ray does not intersect the bounding sphere, we cannot
                // possibly have picked this model, so there is no need to even
                // bother looking at the individual triangle data.

                return null;
            }
            else
            {
                // The bounding sphere test passed, so we need to do a full
                // triangle picking test.

                // Keep track of the closest triangle we found so far,
                // so we can always return the closest one.
                float? closestIntersection = null;

                // Loop over the vertex data, 3 at a time (3 vertices = 1 triangle).
                Vector3[] vertices = (Vector3[])tagData["Vertices"];

                for (int i = 0; i < vertices.Length; i += 3)
                {
                    // Perform a ray to triangle intersection test.
                    float? intersection;

                    RayIntersectsTriangle(ref ray,
                                          ref vertices[i],
                                          ref vertices[i + 1],
                                          ref vertices[i + 2],
                                          out intersection);

                    // Does the ray intersect this triangle?
                    if (intersection != null)
                    {
                        // If so, is it closer than any other previous triangle?
                        if ((closestIntersection == null) ||
                            (intersection < closestIntersection))
                        {
                            // Store the distance to this triangle.
                            closestIntersection = intersection;

                            // Transform the three vertex positions into world space,
                            // and store them into the output vertex parameters.
                            Vector3.Transform(ref vertices[i],
                                              ref modelTransform, out vertex1);

                            Vector3.Transform(ref vertices[i + 1],
                                              ref modelTransform, out vertex2);

                            Vector3.Transform(ref vertices[i + 2],
                                              ref modelTransform, out vertex3);
                        }
                    }
                }

                return closestIntersection;
            }
        }

        //public static void WriteMessageToRemotePlayer(NetPeer peer, GameOptions gameOpts)
        //{
        //    byte[] serializedGameOptions;
        //    MemoryStream ms = new MemoryStream();
        //    BinaryFormatter bf1 = new BinaryFormatter();
        //    bf1.Serialize(ms, gameOpts);
        //    serializedGameOptions =  ms.ToArray();

        //    NetBuffer buffer = new NetBuffer(serializedGameOptions);

        //    peer.SendToAll(buffer, NetChannel.ReliableInOrder1);
        //}


        public static void WriteMessageToRemotePlayer(NetPeer peer, String msg)
        {
            Lidgren.Network.NetBuffer buff = new Lidgren.Network.NetBuffer();
            buff.Write(msg);
            peer.SendToAll(buff, Lidgren.Network.NetChannel.ReliableInOrder1);
        }

        public static string GetMessageFromConnection(NetPeer peer, out NetMessageType type, out NetConnection senderConnection)
        {
            NetBuffer buffer = peer.CreateBuffer();
            String retVal = String.Empty;
            bool dataread = false;
            do
            {
                peer.ReadMessage(buffer, out type, out senderConnection);
                switch (type)
                {
                    case NetMessageType.ServerDiscovered:
                        /*
                        // just connect to any server found!
                        // make hail
                        NetBuffer buf = client.CreateBuffer();
                        buf.Write("Hail from " + Environment.MachineName);
                        client.Connect(buffer.ReadIPEndPoint(), buf.ToArray());
                         */
                        break;
                    case NetMessageType.ConnectionRejected:
                        //Console.WriteLine("Rejected: " + buffer.ReadString());
                        break;
                    case NetMessageType.DebugMessage:
                    case NetMessageType.VerboseDebugMessage:
                        //Console.WriteLine(buffer.ReadString());
                        break;
                    case NetMessageType.StatusChanged:
                        //Console.WriteLine("New status: " + client.Status + " (" + buffer.ReadString() + ")");
                        break;
                    case NetMessageType.Data:
                        // The other sent this data!
                        retVal = buffer.ReadString();
                        dataread = true;
                        break;
                }
            }
            while (!dataread);
            return retVal;
        }

    }
}
