using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using System.IO;
using Microsoft.Xna.Framework.Storage;

namespace Raumschach_Chess
{
    public static class FileHelper
    {
        public static Dictionary<string, string> LoadSaves()
        {
            if (!File.Exists(res.FileWithSaves)) return new Dictionary<string, string>();
            using (StreamReader reader = new StreamReader(TitleContainer.OpenStream(res.FileWithSaves)))
            {
                String[] s;
                Dictionary<string, string> retval = new Dictionary<string, string>();
                while (reader.Peek() >= 0)
                {
                    s = reader.ReadLine().Split('|');
                    retval.Add(s[0], s[1]);
                }
                return retval;
            }
        }
        public static void SaveGame(string name, string FEN)
        {
            using (var fStream = TitleContainer.OpenStream(res.FileWithSaves))
            using (StreamWriter writer = new StreamWriter(fStream))
            using (StreamReader reader = new StreamReader(fStream))
            {
                reader.ReadToEnd();
                writer.WriteLine(name + '|' + FEN);
            }
        }

        #region Get text lines  
        /// <summary>  
        /// Returns the number of text lines we have in the file.  
        /// </summary>  
        /// <param name="filename">Filename</param>  
        /// <returns>Array of strings.</returns>  
        static public string[] GetLines(string filename)
        {
            try
            {
                StreamReader reader = new StreamReader(
                    new FileStream(filename, FileMode.Open, FileAccess.Read),
                    System.Text.Encoding.UTF8);
                // Generic version  
                List<string> lines = new List<string>();
                do
                {
                    lines.Add(reader.ReadLine());
                } while (reader.Peek() > -1);
                reader.Close();
                return lines.ToArray();
            }
            catch (FileNotFoundException)
            {
                // Failed to find a file.  
                return null;
            }
            catch (DirectoryNotFoundException)
            {
                // Failed to find a directory.  
                return null;
            }
            catch (IOException)
            {
                // Something else must have happened.  
                return null;
            }
        }
        #endregion

        #region Write Helpers  
        /// <summary>  
        /// Write Vector3 to stream.  
        /// </summary>  
        /// <param name="writer">Writer</param>  
        /// <param name="vector">Vector3</param>  
        public static void WriteVector3(BinaryWriter writer, Vector3 vector)
        {
            if (writer == null)
                throw new ArgumentNullException("writer");
            writer.Write(vector.X);
            writer.Write(vector.Y);
            writer.Write(vector.Z);
        }

        /// <summary>  
        /// Write Vector4 to stream.  
        /// </summary>  
        /// <param name="writer">Writer</param>  
        /// <param name="vec">Vector4</param>  
        public static void WriteVector4(BinaryWriter writer, Vector4 vector)
        {
            if (writer == null)
                throw new ArgumentNullException("writer");

            writer.Write(vector.X);
            writer.Write(vector.Y);
            writer.Write(vector.Z);
            writer.Write(vector.W);
        }

        /// <summary>  
        /// Write Matrix to stream.  
        /// </summary>  
        /// <param name="writer">Writer</param>  
        /// <param name="matrix">Matrix</param>  
        public static void WriteMatrix(BinaryWriter writer, Matrix matrix)
        {
            if (writer == null)
                throw new ArgumentNullException("writer");

            writer.Write(matrix.M11);
            writer.Write(matrix.M12);
            writer.Write(matrix.M13);
            writer.Write(matrix.M14);
            writer.Write(matrix.M21);
            writer.Write(matrix.M22);
            writer.Write(matrix.M23);
            writer.Write(matrix.M24);
            writer.Write(matrix.M31);
            writer.Write(matrix.M32);
            writer.Write(matrix.M33);
            writer.Write(matrix.M34);
            writer.Write(matrix.M41);
            writer.Write(matrix.M42);
            writer.Write(matrix.M43);
            writer.Write(matrix.M44);
        }
        #endregion

        #region Read Helpers  
        /// <summary>  
        /// Read Vector3 from stream.  
        /// </summary>  
        /// <param name="reader">Reader</param>  
        /// <returns>Vector3</returns>  
        public static Vector3 ReadVector3(BinaryReader reader)
        {
            if (reader == null)
                throw new ArgumentNullException("reader");

            return new Vector3(
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle());
        }

        /// <summary>  
        /// Read Vector4 from stream.  
        /// </summary>  
        /// <param name="reader">Reader</param>  
        /// <returns>Vector4</returns>  
        public static Vector4 ReadVector4(BinaryReader reader)
        {
            if (reader == null)
                throw new ArgumentNullException("reader");

            return new Vector4(
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle());
        }

        /// <summary>  
        /// Read Matrix from stream.  
        /// </summary>  
        /// <param name="reader">Reader</param>  
        /// <returns>Matrix</returns>  
        public static Matrix ReadMatrix(BinaryReader reader)
        {
            if (reader == null)
                throw new ArgumentNullException("reader");

            return new Matrix(
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle(),
                reader.ReadSingle());
        }
        #endregion
    }
}
