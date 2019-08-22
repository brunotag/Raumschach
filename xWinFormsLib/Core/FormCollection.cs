/*
xWinForms © 2007-2009
Eric Grossinger - ericgrossinger@gmail.com
Edited 24/12/2012 - layoric@gmail.com
*/
using System;
using System.Collections.Generic;
using System.Linq;
using WinForms = System.Windows.Forms;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Content;

namespace xWinFormsLib
{
    public class FormCollection
    {
        static private List<Form> _forms = new List<Form>();
        static private Form _activeForm;
        static private Form _topMostForm;
        //static bool hasMaximizedForm = false;

        //static private Menu menu = null;
        static private readonly SubMenu ContextMenu = null;
        static private SpriteBatch _spriteBatch;

        static private MouseCursor _cursor;
        static bool _isCursorVisible = true;

        static Label _tooltip = new Label("tooltip", Vector2.Zero, "", Color.Beige, Color.Black, 200, Label.Align.Left);

        public static IServiceProvider Services { get; private set; }
        public static GraphicsDeviceManager Graphics { get; private set; }
        public static ContentManager ContentManager { get; private set; }
        static public List<Form> Forms { get { return _forms; } set { _forms = value; } }
        static public Form ActiveForm { get { return _activeForm; } set { _activeForm = value; } }
        static public Form TopMostForm { get { return _topMostForm; } set { _topMostForm = value; } }
        static public MouseCursor Cursor { get { return _cursor; } set { _cursor = value; } }
        static public bool IsCursorVisible { get { return _isCursorVisible; } set { _isCursorVisible = value; } }
        static public Label Tooltip { get { return _tooltip; } set { _tooltip = value; } }
        //static public Menu Menu { get { return menu; } set { menu = value; } }
        public static bool Snapping { get; set; }
        public static Vector2 SnapSize { get; set; }

        private static WinForms.Form _window;
        public static WinForms.Form Window { get { return _window; } set { _window = value; } }

        public static int Count
        {
            get
            {
                return _forms.Count(t => !t.IsDisposed && t.Visible);
            }
        }

        public FormCollection(GameWindow window, IServiceProvider services, ref GraphicsDeviceManager graphics)
        {
            _window = (WinForms.Form)WinForms.Control.FromHandle(window.Handle);
            _window.SizeChanged += Form_SizeChanged;
            
            Services = services;
            Graphics = graphics;

            ContentManager = new ContentManager(services, "Content");
            _spriteBatch = new SpriteBatch(graphics.GraphicsDevice);

            _cursor = new MouseCursor(false, true, ContentManager);

            _tooltip.Font = ContentManager.Load<SpriteFont>(@"fonts\pescadero9");
            _tooltip.Initialize(ContentManager, graphics.GraphicsDevice);
            _tooltip.Visible = false;

            //contextMenu = new SubMenu(null);
            //contextMenu.Add(new MenuItem("mnuRestore", "Restore", null), null);
            //contextMenu.Add(new MenuItem("mnuMinimize", "Minimize", null), null);
            //contextMenu.Add(new MenuItem("mnuMaximize", "Maximize", null), null);
            //contextMenu.Add(new MenuItem("", "-", null), null);
            //contextMenu.Add(new MenuItem("mnuRestore", "Close", null), null);
        }

        static FormCollection()
        {
            SnapSize = new Vector2(10f, 10f);
            Snapping = true;
        }

        /// <summary>
        /// Returns a form by index
        /// </summary>
        /// <param name="index">List index</param>
        /// <returns></returns>
        public Form this[int index]
        {
            get { return _forms[index]; }
            set { _forms[index] = value; }
        }
        /// <summary>
        /// Returns a form by name
        /// </summary>
        /// <param name="name">Form name</param>
        /// <returns></returns>
        public Form this[string name]
        {
            get
            { return _forms.FirstOrDefault(t => t.Name == name); }
            set
            {
                for (int i = 0; i < _forms.Count; i++)
                    if (_forms[i].Name == name)
                    {
                        _forms[i] = value;
                        break;
                    }
            }
        }
        static public Form Form(string name)
        {
            return _forms.FirstOrDefault(t => t.Name == name);
        }

        /// <summary>
        /// Add a form to the collection
        /// </summary>
        /// <param name="form">Form</param>
        public void Add(Form form)
        {            
            _forms.Insert(0, form);
            //topMostForm = form;
            //form.Focus();
            //form.Update(null);
        }
        /// <summary>
        /// Remove a form from the collection
        /// </summary>
        /// <param name="form">Form</param>
        public void Remove(Form form)
        {
            form.Dispose();
            _forms.Remove(form);
        }
        /// <summary>
        /// Remove a form by name
        /// </summary>
        /// <param name="name">Form name</param>
        public void Remove(string name)
        {
            for (int i = 0; i < _forms.Count; i++)
                if (_forms[i].Name == name)
                {
                    _forms[i].Dispose();
                    _forms.RemoveAt(i);
                    break;
                }

        }

        /// <summary>
        /// Dispose of the form collection
        /// </summary>
        public void Dispose()
        {
            for (int i = _forms.Count - 1; i > -1; i--)
                _forms[i].Dispose();

            _forms.Clear();

            //if (menu != null)
            //    menu.Dispose();

            _tooltip.Dispose();
            _cursor.Dispose();
            ContentManager.Dispose();                        
        }

        /// <summary>
        /// Update enabled forms
        /// </summary>
        public void Update(GameTime gameTime)
        {
            //Update MouseHelper
            MouseHelper.Update();

            //Update Cursor
            _cursor.Update(gameTime);

            //Update active or topMost form
            if (_activeForm != null)
                _activeForm.Update(gameTime);
            else if (_topMostForm != null)
                _topMostForm.Update(gameTime);
            else if (_topMostForm == null && _forms.Count > 0)
            {
                for (int i = 0; i < _forms.Count; i++)
                    if (!_forms[i].IsDisposed)
                    {
                        _topMostForm = _forms[i];
                        _topMostForm.Focus();
                        break;
                    }
            }

            //Update other forms
            for (int i = 0; i < _forms.Count; i++)
                if (!_forms[i].IsDisposed && _forms[i].Enabled && 
                    _forms[i] != _activeForm && _forms[i] != _topMostForm)
                    _forms[i].Update(gameTime);

            //Update Top Menu
            //if (activeForm == null && menu != null && menu.Visible)
            //    if (topMostForm == null || topMostForm.State != xWinFormsLib.Form.WindowState.Maximized)
            //        menu.Update(gameTime);

            //Update Context Menu
            if (ContextMenu != null && ContextMenu.State != SubMenu.MenuState.Closed && ContextMenu.Visible)
                ContextMenu.Update(gameTime);
        }

        /// <summary>
        /// Render forms content
        /// </summary>
        public void Render()
        {
            Graphics.GraphicsDevice.Clear(Color.Black);

            //if (menu != null && !menu.IsDisposed && menu.Visible && !hasMaximizedForm)
            //    menu.Render();

            //if (contextMenu != null && !contextMenu.IsDisposed && contextMenu.Visible)
            //    contextMenu.Render();

            for (int i = 0; i < _forms.Count; i++)
                if (!_forms[i].IsDisposed && _forms[i].Visible)
                    _forms[i].Render();
        }

        /// <summary>
        /// Draw visible forms
        /// </summary>
        public void Draw()
        {
            #region Draw TopMenu

            //if (menu != null && !menu.IsDisposed && menu.Visible)
            //    menu.Draw();

            #endregion

            #region Draw Forms

            for (int i = _forms.Count - 1; i > -1; i--)
                if (!_forms[i].IsDisposed && _forms[i].Visible && _forms[i] != _topMostForm &&
                    _forms[i].State != xWinFormsLib.Form.WindowState.Minimized)
                    _forms[i].Draw();

            if (_topMostForm != null && !_topMostForm.IsDisposed && _topMostForm.Visible)
                _topMostForm.Draw();

            #endregion

            #region Draw Minimized Forms
            for (int i = _forms.Count - 1; i >= 0; i--)
                if (_forms[i].State == xWinFormsLib.Form.WindowState.Minimized && 
                    !_forms[i].IsDisposed && _forms[i].Visible && _forms[i] != _topMostForm)
                    _forms[i].Draw();
            #endregion

            #region Draw Context Menu
            if (ContextMenu != null && !ContextMenu.IsDisposed && ContextMenu.Visible && ContextMenu.State != SubMenu.MenuState.Closed)
            {
                _spriteBatch.Begin(SpriteSortMode.Texture, null);
                ContextMenu.Draw(_spriteBatch);
                _spriteBatch.End();
            }
            #endregion

            #region Draw Cursor
            if (_isCursorVisible)
                _cursor.Draw();
            #endregion
        }

        public static Vector2? GetMinimizedPosition(Form form)
        {
            if (_forms.Any(t => t != form && t.IsMinimizing))
            {
                return null;
            }

            //using MinimumSize from the Form Class (100 by 40)
            for (int y = Graphics.GraphicsDevice.Viewport.Height - 20; y > 0; y -= 20)
            {
                for (int x = 0; x < Graphics.GraphicsDevice.Viewport.Width - 99; x += 100)
                {
                    bool isOccupied = _forms.Any(t => t != form && !t.IsDisposed && t.Visible && t.Position.X == x && t.Position.Y == y);

                    if (!isOccupied)
                        return new Vector2(x, y);
                }
            }

            return Vector2.Zero;
        }

        public static Vector2 GetMaximizedSize(Form form)
        {
            Vector2 maxSize = new Vector2(Graphics.GraphicsDevice.Viewport.Width, Graphics.GraphicsDevice.Viewport.Height);

            for (int i = 0; i < _forms.Count; i++)
                if (_forms[i] != form && !_forms[i].IsDisposed && _forms[i].Visible && _forms[i].State == xWinFormsLib.Form.WindowState.Minimized)
                    if (_forms[i].Top < maxSize.Y)
                        maxSize.Y = _forms[i].Top;

            return maxSize;
        }

        public static void ShowContextMenu()
        {
            ContextMenu.Open(MouseHelper.Cursor.Position);
        }

        public static void CloseOpenedMenus()
        {
            //if (menu != null)
            //    for (int i = 0; i < menu.Items.Count; i++)
            //        if (menu.Items[i].SubMenu != null && menu[i].SubMenu.State != SubMenu.MenuState.Closed)
            //            menu.Items[i].SubMenu.Close();

            for (int i = 0; i < _forms.Count; i++)
                if (_forms[i].Menu != null)
                    for (int j = 0; j < _forms[i].Menu.Items.Count; j++)
                        if (_forms[i].Menu.Items[j].SubMenu != null && _forms[i].Menu.Items[j].SubMenu.State != SubMenu.MenuState.Closed)
                            _forms[i].Menu.Items[j].SubMenu.Close();
        }

        public static void FocusNext()
        {
        }

        public static bool IsObstructed(Control control, Point location)
        {
            for (int i = 0; i < _forms.Count; i++)
            {
                if (control.Owner != null && _forms[i] != control.Owner)
                {
                    if (_forms[i].area.Contains(control.area) || _forms[i].area.Intersects(control.area))
                    {
                        if (_forms[i].area.Contains(location))
                            return true;
                    }
                }
                else if (control.Owner == null)
                {
                    if (_forms[i].area.Contains(control.area) || _forms[i].area.Intersects(control.area))
                    {
                        if (_forms[i].area.Contains(location))
                            return true;
                    }
                }
            }

            return false;
        }

        private void Form_SizeChanged(object obj, EventArgs e)
        {
            Rectangle area = 
                new Rectangle(0, 0, Graphics.GraphicsDevice.Viewport.Width, Graphics.GraphicsDevice.Viewport.Height);

            foreach (Form form in _forms)
            {
                //If a form is ouf of the working area,
                //we need to put it back where the user can see it.
                if (!area.Contains(form.area))
                {
                    if (form.Position.X + form.Width > Graphics.GraphicsDevice.Viewport.Width)
                        form.X = Graphics.GraphicsDevice.Viewport.Width - form.Size.X;
                    //else if (form.Position.X + form.Width < 0)
                    //    form.X = 0;
                    
                    if (form.Position.Y + form.Height > Graphics.GraphicsDevice.Viewport.Height)
                        form.Y = Graphics.GraphicsDevice.Viewport.Height - form.Size.Y;
                    //else if (form.Position.Y + form.Width < 0)
                    //    form.Y = 0;
                }

                //If a form was maximized
                if (form.State == xWinFormsLib.Form.WindowState.Maximized)
                {
                    //resize it
                    form.Width = Graphics.GraphicsDevice.Viewport.Width;
                    form.Height = Graphics.GraphicsDevice.Viewport.Height;

                    //if the window was previously maximized,
                    //we need to reposition the form.
                    if (_window.WindowState == System.Windows.Forms.FormWindowState.Normal)
                    {
                        form.X = 0;
                        form.Y = 0;
                    }
                }
            }
        }
    }
}
