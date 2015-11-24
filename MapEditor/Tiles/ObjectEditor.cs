using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;


namespace MapEditor.Tiles
{
    class ObjectEditor
    {
        private BindingList<GameObject> _listItem;
        private static Brush brush = new SolidBrush(Color.FromArgb(125, 205, 205, 155));
        private static Pen pen = new Pen(brush);
        private static Brush brush_initBound = new SolidBrush(Color.FromArgb(175, 50, 50, 50));
        private static Brush brush_font = new SolidBrush(Color.FromArgb(220, 35, 35, 35));
        public ObjectEditor(BindingList<GameObject> sourceList)
        {
            ListItem = sourceList;
            MouseDown = new Point(-1, -1);

        }
        public void Bind(ListBox listbox)
        {
            BindingSource bs = new BindingSource(this, "ListItem");
            bs.AllowNew = true;
            listbox.DataSource = bs;
            listbox.DisplayMember = "Name";
            _listItem.ListChanged += (object sender, ListChangedEventArgs e) =>
                {
                    (listbox.DataSource as BindingSource).ResetBindings(true);
                };
        }

        // Lưu lại toạ độ khi chuột click. Dùng để khởi tạo đối tượng GameObject. kết hợp với MouseUp tạo thành Rectangle
        public Point MouseDown { get; set; }

        // Lưu lại toạ độ khi chuột release. Dùng để khởi tạo đối tượng GameObject. Kết hợp với MouseDown tạo thành Rectangle
        public Point MouseUp { get; set; }

        // Dùng bindinglist để bind với listbox.
        public BindingList<GameObject> ListItem
        {
            get { return _listItem; }
            set { _listItem = value; }
        }


        public void InitGameObject()
        {
            if (this.MouseDown.X == -1 && this.MouseDown.Y == -1)
                return;
            int x = Math.Min(MouseDown.X, MouseUp.X);
            int y = Math.Min(MouseDown.Y, MouseUp.Y);
            int width = Math.Max(MouseDown.X, MouseUp.X) - x;
            int height = Math.Max(MouseDown.Y, MouseUp.Y) - y;
            if (MainForm.Settings.UseTransform == true)
            {
                y += height;
            }

            if (width == 0 && height == 0)
                return;
            var obj = new GameObject(x, y, width, height);
            obj.Name = "object" + ListItem.Count.ToString();
            obj.Id = 0;
            ListItem.Add(obj);

            this.MouseDown = new Point(-1, -1);
        }

        public void draw(Graphics graphics)
        {
            foreach (GameObject GObject in ListItem)
            {
                if (GObject.Image == null)
                {
                    graphics.FillRectangle(brush_initBound, GObject.InitBound);
                }
                else
                {
                    graphics.DrawImage(GObject.Image, GObject.InitBound);
                }
                graphics.FillRectangle(brush, GObject.ActiveBound);

                graphics.DrawString(
                    GObject.Name,
                    SystemFonts.IconTitleFont,
                    brush_font,
                    GObject.ActiveBound.X + 3,
                    GObject.ActiveBound.Y + 3);
            }
        }
        public void draw(Graphics graphics, int worldheight)
        {

            foreach (GameObject GObject in ListItem)
            {
                var activebound = GObject.GetActiveBoundTransform(worldheight);
                var initbound =  GObject.GetInitBoundTransform(worldheight);
                if (GObject.Image == null)
                {
                    graphics.FillRectangle(brush_initBound, initbound);
                }
                else
                {
                    graphics.DrawImage(GObject.Image, initbound);
                }
                graphics.FillRectangle(brush, activebound);
                graphics.DrawString(
                    GObject.Name,
                    SystemFonts.IconTitleFont,
                    brush_font,
                    activebound.X + 3,
                    activebound.Y + 3);
            }
        }

        public static void FitTile(GameObject gameobject)
        {
            gameobject.setActiveBound(fitTile(gameobject.ActiveBound));
            gameobject.setInitBound(fitTile(gameobject.InitBound));
        }

        private static Rectangle fitTile(Rectangle rectangle)
        {
            int left = rectangle.X;
            int top = rectangle.Y;
            int right = rectangle.X + rectangle.Width;
            int bottom = rectangle.Y + rectangle.Height;
            int temp;
            var tileSize = MainForm.Settings.TileSize;
            bool usetransform = MainForm.Settings.UseTransform;
            temp = top % tileSize.Height;
            if (temp != 0)
            {
                if (usetransform)
                    top = top - temp + tileSize.Height;
                else
                    top -= temp;
            }

            temp = left % tileSize.Width;
            if (temp != 0)
                left -= temp;

            temp = bottom % tileSize.Height;
            if (temp != 0)
                bottom = bottom - temp + tileSize.Height;

            temp = right % tileSize.Width;
            if (temp != 0)
                right = right - temp + tileSize.Width;

            return new Rectangle(left, top, right - left, bottom - top);
        }


        public static void Save(XmlTextWriter writter, BindingList<GameObject> listObject, string path)
        {
            writter.WriteStartElement("Objects");
            {
                foreach (GameObject item in listObject)
                {
                    writter.WriteStartElement("Item");
                    {
                        writter.WriteAttributeString("Id", item.Id.ToString());
                        writter.WriteAttributeString("Name", item.Name);
                        writter.WriteAttributeString("X", item.InitBound.X.ToString());
                        writter.WriteAttributeString("Y", item.InitBound.Y.ToString());
                        writter.WriteAttributeString("Width", item.InitBound.Width.ToString());
                        writter.WriteAttributeString("Height", item.InitBound.Height.ToString());

                        if (Directory.Exists("res") == false)
                        {
                            Directory.CreateDirectory("res");
                        }
                        if (item.Image != null)
                        {
                            item.Image.Save("res\\" + item.Name + ".png");
                            writter.WriteAttributeString("Image", "res\\" + item.Name + ".png");
                        }
                        writter.WriteStartElement("ActiveBound");
                        {
                            writter.WriteAttributeString("X", item.ActiveBound.X.ToString());
                            writter.WriteAttributeString("Y", item.ActiveBound.Y.ToString());
                            writter.WriteAttributeString("Width", item.ActiveBound.Width.ToString());
                            writter.WriteAttributeString("Height", item.ActiveBound.Height.ToString());
                        }
                        writter.WriteEndElement();                          //ActiveBound
                        if (item.Parameters.Any())
                        {
                            writter.WriteStartElement("Params");
                            {
                                foreach (KeyValuePair<string, string> elem in item.Parameters.ToList())
                                {
                                    writter.WriteStartElement("Elem");
                                    writter.WriteAttributeString("Key", elem.Key);
                                    writter.WriteAttributeString("Value", elem.Value);
                                    writter.WriteEndElement();                  //Elem 
                                }
                            }
                            writter.WriteEndElement();                  //Params
                        }
                    }
                    writter.WriteEndElement();                              //Item
                }
            }
            writter.WriteEndElement();                                      // Objects
        }

        public static IList<GameObject> Load(XmlTextReader reader, string path)
        {
            List<GameObject> listGameObject = new List<GameObject>();
            reader.ReadStartElement("Objects");
            while (reader.NodeType != XmlNodeType.EndElement || reader.Name != "Objects")
            {
                reader.Read();
                if (reader.IsStartElement("Item"))
                {
                    GameObject g_obj = readGameObject(reader);
                    listGameObject.Add(g_obj);
                }
            }

            return listGameObject;
        }

        private static GameObject readGameObject(XmlTextReader reader)
        {
            GameObject obj = null;
           // reader.ReadStartElement("Item");
            int id = Int32.Parse(reader.GetAttribute("Id"));
            string name = reader.GetAttribute("Name");
            int x = Int32.Parse(reader.GetAttribute("X"));
            int y = Int32.Parse(reader.GetAttribute("Y"));
            int width = Int32.Parse(reader.GetAttribute("Width"));
            int height = Int32.Parse(reader.GetAttribute("Height"));
            string img = reader.GetAttribute("Image");
            obj = new GameObject(x, y, width, height);
            obj.Id = id;
            obj.Name = name;
            if (String.IsNullOrEmpty(img) == false)
            {
                if (File.Exists(img))
                {
                    obj.Image = Image.FromFile(img);
                }
            }
            while (reader.NodeType != XmlNodeType.EndElement || reader.Name != "Item")
            {
                reader.Read();
                if (reader.IsStartElement("ActiveBound"))
                {
                    x = Int32.Parse(reader.GetAttribute("X"));
                    y = Int32.Parse(reader.GetAttribute("Y"));
                    width = Int32.Parse(reader.GetAttribute("Width"));
                    height = Int32.Parse(reader.GetAttribute("Height"));
                    obj.ActiveBound = new Rectangle(x, y, width, height);

                }
                if (reader.IsStartElement("Params"))
                {
                    string key = String.Empty;
                    string value = String.Empty;
                    while (reader.NodeType != XmlNodeType.EndElement || reader.Name != "Params")
                    {
                        reader.Read();
                        if (reader.IsStartElement("Elem"))
                        {
                            key = reader.GetAttribute("Key");
                            value = reader.GetAttribute("Value");
                            obj.Parameters.Add(key, value);
                        }
                    }                   
                }
            }
            return obj;
        }
    }
}
