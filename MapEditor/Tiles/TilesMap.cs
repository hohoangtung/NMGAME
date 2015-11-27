using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace MapEditor.Tiles
{
    public class TilesMap : INotifyPropertyChanged
    {
        #region Private Attribute
        private Point _mapSize;
        private int[,] _matrixIndex;
        private TileSet _tileset;
        #endregion  // private attribute

        public TilesMap(int columns, int rows)
        {
            _mapSize = new Point(columns, rows);
            _matrixIndex = new int[columns, rows];

            ListObject = new BindingList<GameObject>();
            TileSet = null;
            MapController.MapSize = new Size(
                _mapSize.X * MainForm.Settings.TileSize.Width,
                _mapSize.Y * MainForm.Settings.TileSize.Height);
        }

        public int Columns
        {
            get { return _mapSize.X; }
            private set
            {
                if (_mapSize.X != value)
                {
                    _mapSize.X = value;
                    OnProPertyChanged(new PropertyChangedEventArgs("Columns"));

                }
            }
        }

        public TileSet TileSet
        {
            get { return _tileset; }
            set {
                if (_tileset != value)
                {
                    _tileset = value;
          
                }
            }
        }
        public int Rows
        {
            get { return _mapSize.Y; }
            private set
            {
                if (_mapSize.Y != value)
                {
                    _mapSize.Y = value;
                    OnProPertyChanged(new PropertyChangedEventArgs("Rows"));
                }
            }
        }
        public BindingList<GameObject> ListObject { get; set; }
        public int this[int i, int j]
        {
            get
            {
                if (i >= Columns || j >= Rows)
                    throw new Exception();
                return _matrixIndex[i, j];
            }
            set
            {
                if (i >= Columns || j >= Rows)
                    throw new Exception();
                _matrixIndex[i, j] = value;
            }
        }


        public static TilesMap Load(string path)
        {
            TilesMap tilesmap = null;
            int columns = 0;
            int rows = 0;
            using (XmlTextReader reader = new XmlTextReader(path))
            {
                while (reader.Read())
                {
                    if (reader.IsStartElement())
                    {
                        if (reader.Name == "Tilesmap")
                        {
                            columns = Int32.Parse(reader.GetAttribute("columns"));
                            rows = Int32.Parse(reader.GetAttribute("rows"));
                            tilesmap = new TilesMap(columns, rows);
                        }
                        if (reader.Name == "Row")
                        {
                            int rownumber = Int32.Parse(reader.GetAttribute("id"));
                            string indexs = reader.ReadString();
                            var row = indexs.Split('\t');
                            for (int i = 0; i < columns; i++)
                            {
                                tilesmap[i, rownumber] = Int32.Parse(row[i]);
                            }
                        }
                        if (reader.Name == "TileSet")
                        {
                            tilesmap.TileSet = TileSet.Load(reader, path);
                        }
                        if (reader.Name == "Objects")
                        {
                            var listobject = ObjectEditor.Load(reader, path);
                            tilesmap.ListObject = new BindingList<GameObject>(listobject);
                        }
                    }
                    if (reader.NodeType == XmlNodeType.EndElement && reader.Name == "Tilesmap")
                        break;
                }
            }
            return tilesmap;
        }

        public static void Save(TilesMap tilesmap, string path)
        {
            if (tilesmap == null)
                throw new ArgumentException("Argument does not accept null", "tilesmap");
            if (path == null)
                throw new ArgumentException("Argument does not accept null", "path");
            using (XmlTextWriter wr = new XmlTextWriter(path, Encoding.UTF8))
            {
                wr.Formatting = Formatting.Indented;
                wr.WriteStartDocument();
                wr.WriteStartElement("Tilesmap");
                {
                    wr.WriteAttributeString("columns", tilesmap.Columns.ToString());
                    wr.WriteAttributeString("rows", tilesmap.Rows.ToString());

                    wr.WriteStartElement("MatrixIndex");
                    {
                        for (int i = 0; i < tilesmap.Rows; i++)
                        {
                            wr.WriteStartElement("Row");
                            {
                                wr.WriteAttributeString("id", i.ToString());
                                for (int j = 0; j < tilesmap.Columns; j++)
                                {
                                    wr.WriteString(tilesmap[j, i].ToString());
                                    if (j != tilesmap.Columns - 1)
                                        wr.WriteString("\t");
                                }
                            }
                            wr.WriteEndElement(); // rows
                        }
                    }
                    wr.WriteEndElement();   // Matrixindex
                    if (tilesmap.TileSet != null)
                    {
                        TileSet.Save(wr, tilesmap.TileSet, path);
                    }
                    if (tilesmap.ListObject.Any())
                    {
                        ObjectEditor.Save(wr, tilesmap.ListObject, path);
                    }

                }

                wr.WriteEndElement();       // Tiles map
                wr.WriteEndDocument();
                wr.Close();
                wr.Dispose();
            }
        }

        public void Resize(int columns, int rows)
        {
            if (columns == this.Columns && rows == this.Rows)
            {
                return;
            }
            _mapSize = new Point(columns, rows);
            _matrixIndex = new int[columns, rows];
        }

        public int GetMapHeight()
        {
            return this.Rows * MainForm.Settings.TileSize.Height;
        }
        public int GetMapWidth()
        {
            return this.Columns * MainForm.Settings.TileSize.Width;
        }
        public Size GetMapSize()
        {
            return new Size(
                this.GetMapWidth(),
                this.GetMapHeight());
        }

        public event PropertyChangedEventHandler PropertyChanged;

        protected void OnProPertyChanged(PropertyChangedEventArgs args)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, args);
            }
        }
    }
}