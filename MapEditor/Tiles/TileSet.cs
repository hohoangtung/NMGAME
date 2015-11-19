using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Collections.Specialized;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;

namespace MapEditor.Tiles
{
    public class TileSet : INotifyPropertyChanged, INotifyCollectionChanged
    {
        // CONSTRUCTOR 
        // ----------------------------------------------------------

        public TileSet()
        {
            this.ListTiles = new ObservableCollection<Tile>();
            this.PropertyChanged += TileSet_PropertyChanged;
        }

        // PRIVATE ATTRIBUTE
        // ---------------------------------------------------------

        private int _widthtile;                 // kích thước tile
        private int _hieghttile;                // kích thước tile
        private string _filename;
        private int _columns;
        private int _rows;
        private IList<Tile> _listTiles;

        // PRIVATE METHOD
        private void setProperty<T>(ref T obj, T value, string propertyName)
        {
            if (Object.Equals(obj, value))
                return;
            obj = value;
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
        private void TileSet_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (e.PropertyName == "Widthtile")
            {
                if (Image != null)
                    this.Columns = (Widthtile == 0) ? 1 : (Image.Width / Widthtile);
            }
            else if (e.PropertyName == "Hieghttile")
            {
                if (Image != null)
                    this.Rows = (Hieghttile == 0) ? 1 : (Image.Height / Hieghttile);
            }
            else if (e.PropertyName == "Image")
            {
                this.Columns = (Widthtile == 0) ? 1 : (Image.Width / Widthtile);
                this.Rows = (Hieghttile == 0) ? 1 : (Image.Height / Hieghttile);
            }
        }

        // PUBLIC PROPERTY
        // ---------------------------------------------------------

        public string FileName {
            get
            {
                return _filename;
            }
            set
            {
                setProperty(ref _filename, value, "FileName");
            }
        }
        public int Widthtile
        {
            get { return _widthtile; }
            set { this.setProperty(ref _widthtile, value, "Widthtile");}
        }
        public int Hieghttile
        {
            get { return _hieghttile; }
            set {this.setProperty(ref _hieghttile, value, "Hieghttile");}
        }
        public int Columns
        {
            get { return _columns; }
            set {setProperty(ref _columns,value, "Columns");}
        }
        public int Rows
        {
            get { return _rows; }
            set { setProperty(ref _rows, value, "Rows");}
        }
        public Image Image { get; set; }

        public ObservableCollection<Tile> ListTiles
        {
            get { return _listTiles as ObservableCollection<Tile>; }
            set
            {
                if (_listTiles != value )
                {
                    if (_listTiles != null)
                    (_listTiles as ObservableCollection<Tile>).CollectionChanged -= this.CollectionChanged;
                    value.CollectionChanged += CollectionChanged;
                    setProperty(ref _listTiles, value, "ListTiles");
                }
            }
        }
        // PUBLIC METHOD
        // ---------------------------------------------------------
        public event NotifyCollectionChangedEventHandler CollectionChanged;
        public event PropertyChangedEventHandler PropertyChanged;

        // lưu bằng xml
        public static void Save(TileSet tileset, string filename)
        {
            XmlTextWriter wr = new XmlTextWriter(filename, Encoding.UTF8);
            wr.Formatting = Formatting.Indented;
            wr.WriteStartDocument();
            TileSet.Save(wr, tileset, filename);
            wr.WriteEndDocument();
            wr.Close();
        }
        public static void Save(XmlTextWriter writer, TileSet tileset, string filename)
        {
            writer.WriteStartElement("TileSet");
            {
                writer.WriteAttributeString("Columns", tileset.Columns.ToString());
                writer.WriteAttributeString("Rows", tileset.Rows.ToString());
                writer.WriteAttributeString("Widthtile", tileset.Widthtile.ToString());
                writer.WriteAttributeString("Heighttile", tileset.Hieghttile.ToString());

                // Thay vì lưu fullpath thì lưu kiểu related path để cố định luôn luôn 2 file ảnh và file xml đi chung với nhau.
                string imagefilename = tileset.FileName.Substring(tileset.FileName.LastIndexOf(@"\") + 1);
                //writer.WriteElementString("FileName", imagefilename);
                writer.WriteAttributeString("FileName", imagefilename);

                string imagefullpath = filename.Substring(0, filename.LastIndexOf('\\')) + @"\\" + imagefilename;
                if (System.IO.File.Exists(imagefullpath) == false)
                {
                    System.IO.File.Copy(tileset.FileName, imagefullpath);
                }
                writer.WriteStartElement("Tiles");
                {
                    foreach (var item in tileset.ListTiles)
                    {
                        writer.WriteStartElement("Tile");
                        {
                            writer.WriteAttributeString("Id", item.Id.ToString());
                            writer.WriteAttributeString("Name", item.Name);
                            writer.WriteStartElement("Rect");
                            {
                                writer.WriteAttributeString("X", item.SrcRect.X.ToString());
                                writer.WriteAttributeString("Y", item.SrcRect.Y.ToString());
                                writer.WriteAttributeString("Width", item.SrcRect.Width.ToString());
                                writer.WriteAttributeString("Height", item.SrcRect.Height.ToString());
                            }
                            writer.WriteEndElement();   // Tect
                        }
                        writer.WriteEndElement();   // Tile
                    }
                }
                writer.WriteEndElement();   // Tiles
            }
            writer.WriteEndElement();       // Tileset
        }
        public static TileSet Load(XmlReader reader, string filename)
        {
            TileSet tileset = new TileSet();
            tileset.Columns = Int32.Parse(reader.GetAttribute("Columns"));
            tileset.Rows = Int32.Parse(reader.GetAttribute("Rows"));
            tileset.Hieghttile = Int32.Parse(reader.GetAttribute("Heighttile"));
            tileset.Widthtile = Int32.Parse(reader.GetAttribute("Widthtile"));

            // Xử lý để từ tên file ảnh trong giá trị element của node FileName
            // ta lấy thư mục bằng cách tách chuỗi trong path của file xml
            // ghép với tên file ảnh.
            string imagefilename = reader.GetAttribute("FileName");
            string relativepath = filename.Substring(0, filename.LastIndexOf('\\') + 1);  // +1 là dấu \ trong path
            tileset.FileName = relativepath + imagefilename;

            if (System.IO.File.Exists(tileset.FileName))
                tileset.Image = Image.FromFile(tileset.FileName);
            else
                throw new Exception("");

            reader.ReadStartElement("TileSet");
            while (reader.NodeType != XmlNodeType.EndElement || reader.Name != "TileSet")
            {
                reader.Read();
                if (reader.IsStartElement())
                {
                    if (reader.IsStartElement("FileName"))
                    {
                        //reader.Read();

                        //// Xử lý để từ tên file ảnh trong giá trị element của node FileName
                        //// ta lấy thư mục bằng cách tách chuỗi trong path của file xml
                        //// ghép với tên file ảnh.
                        //string imagefilename = reader.Value.Trim();
                        //string relativepath = filename.Substring(0, filename.LastIndexOf('\\') + 1);  // +1 là dấu \ trong path
                        //tileset.FileName = relativepath + imagefilename;

                        //if (System.IO.File.Exists(tileset.FileName))
                        //    tileset.Image = Image.FromFile(tileset.FileName);
                        //else
                        //    throw new Exception("");
                        continue;
                    }
                    if (reader.IsStartElement("Tiles"))
                    {
                        reader.ReadStartElement("Tiles");
                        while (reader.NodeType != XmlNodeType.EndElement || reader.Name != "Tiles")
                        {
                            reader.Read();
                            if (reader.IsStartElement("Tile"))
                            {
                                var tile = readtile(reader);
                                tile.Image = tileset.Image;
                                tileset.ListTiles.Add(tile);
                            }                            
                        }
                        continue;
                    }
                }
            }
            return tileset;
        }

        private static Tile readtile(XmlReader xmlreader)
        {
            int id = Int32.Parse(xmlreader.GetAttribute("Id"));
            string name = xmlreader.GetAttribute("Name");
            Rectangle rect = Rectangle.Empty;
            xmlreader.ReadStartElement("Tile");
            while (xmlreader.NodeType != XmlNodeType.EndElement || xmlreader.Name != "Tile")
            {
                xmlreader.Read();

                if (xmlreader.IsStartElement("Rect"))       // read <Rect> </Rect>
                {
                    rect.X = Int32.Parse(xmlreader.GetAttribute("X"));
                    rect.Y = Int32.Parse(xmlreader.GetAttribute("Y"));
                    rect.Width = Int32.Parse(xmlreader.GetAttribute("Width"));
                    rect.Height = Int32.Parse(xmlreader.GetAttribute("Height"));
                    continue;
                }
            }
            Tile tile = new Tile(null, rect, id);
            tile.Name = name;
            return tile;
        }

    }
}
