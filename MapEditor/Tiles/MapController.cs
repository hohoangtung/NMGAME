using MapEditor.QuadTree;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Documents;
using System.Windows.Forms;

namespace MapEditor.Tiles
{
    class MapController
    {
        // Tham chiếu đến buffer của tablelayout.creategrahics
        private Graphics _graphics;

        // hình chữ nhật gần nhất dùng làm khung để vẽ map.
        private Rectangle _lastVisibleRect;

        public MapController()
        {

        }

        private void ListItem_ListChanged(object sender, ListChangedEventArgs e)
        {
            // Nếu thêm item vào thì add gameObject_PropertyChanged cho item đó.
            if (e.ListChangedType == ListChangedType.ItemAdded)
            {
                (sender as BindingList<GameObject>).ElementAt(e.NewIndex).PropertyChanged += gameObject_PropertyChanged;
            }

            this.Draw(_lastVisibleRect);
        }

        private void gameObject_PropertyChanged(object sender, PropertyChangedEventArgs e)
        {
            if (this.Graphics != null)
            {
                this.Draw(_lastVisibleRect);
            }
        }

        // Graphics dùng để vẽ hình.
        public Graphics Graphics
        {
            get
            {
                 return _graphics;
            }
            set
            {
                _graphics = value; 
            }
        }

        public ObjectEditor ObjectEditor { get; set; }

        private TilesMap _tilesMap;
        public TilesMap TilesMap
        {
            get
            {
                return _tilesMap;
            }
            set
            {
                if (_tilesMap != value)
                {
                    _tilesMap = value;
                    if (value !=null)
                    {
                        value.PropertyChanged += (object sender, PropertyChangedEventArgs e) =>
                            {
                                if (e.PropertyName == "Columns" || e.PropertyName == "Rows")
                                {
                                    MapController.MapSize = new Size(
                                        _tilesMap.Columns * MainForm.Settings.TileSize.Width,
                                        _tilesMap.Rows * MainForm.Settings.TileSize.Height);
                                }
                            };
                    }
                }
            }
        }

        public static Size MapSize { get; set; }

        // Sự kiện được kích hoạt mỗi khi vẽ lại map, hoặc vẽ thêm một tile lên map
        public event EventHandler Drawn;

        protected void OnDraw(EventArgs e)
        {
            if (Drawn != null)
            {
                Drawn(this, e);
            }
        }

        // Duyệt matrixindex và vẽ map.
        // Chỉ vẽ các tile nằm trong màn hình.
        // Vẽ tất cả object.
        public void Draw(Rectangle visilbleRect)
        {
            if (this.Graphics == null)
            {
                return;
            }
            if (this.TilesMap == null)
                return;
            if (this.TilesMap.TileSet == null)
                return;
            var tilesize = MainForm.Settings.TileSize;
            int iBegin = Math.Max(visilbleRect.X / tilesize.Width - 1, 0);
            int iEnd = Math.Min(iBegin + visilbleRect.Width / tilesize.Width + 2, TilesMap.Columns);
            int jBegin = Math.Max(visilbleRect.Y / tilesize.Height - 1, 0);
            int jEnd = Math.Min(jBegin + visilbleRect.Height / tilesize.Height + 2, TilesMap.Rows);

            for (int i = iBegin; i < iEnd; i++)
            {
                for (int j = jBegin; j < jEnd; j++)
                {
                    Tile tile = TilesMap.TileSet.ListTiles.ToList().Find(t => t.Id == TilesMap[i, j]);
                    if (tile == null)
                        continue;
                    tile.draw(
                        Graphics,
                        new Point(tilesize.Width * i, tilesize.Height * j),
                        tilesize); // notice here -> 
                }
            }
            if (MainForm.Settings.UseTransform == true)
            {
                int worldheight = this.TilesMap.GetMapHeight();
                if (this.ObjectEditor.QuadTree == null)
                    this.ObjectEditor.draw(Graphics, worldheight);
                else
                    this.ObjectEditor.draw(Graphics, visilbleRect, worldheight);
            }
            else
            {
                if (this.ObjectEditor.QuadTree == null)
                    this.ObjectEditor.draw(Graphics);
                else
                    this.ObjectEditor.draw(Graphics, visilbleRect);
            }
            
            _lastVisibleRect = visilbleRect;
            OnDraw(null);
        }

        // Vẽ một tile lên toạ độ trên 
        public void DrawTile(Point map_coordinate, Tile tile)
        {
            tile.draw(this.Graphics, map_coordinate, MainForm.Settings.TileSize);
            OnDraw(null);
        }

        public void InitObjectEditor()
        {
            ObjectEditor = new ObjectEditor(TilesMap.ListObject);
            ObjectEditor.ListItem.ListChanged += ListItem_ListChanged;
            //done
        }

        // mở form để chọn ảnh và kích thước tile
        public void CreateTilesSet()
        {
            var _createTilesForm = new CreateTilesForm(); 
            DialogResult rs = _createTilesForm.ShowDialog();

            // lấy dữ liệu từ form
            if (rs == System.Windows.Forms.DialogResult.OK)
            {
                if (this.TilesMap == null)
                    this.TilesMap = new TilesMap(10, 10);
                this.TilesMap.TileSet = _createTilesForm.Tileset;
            }
        }

        // get lisviewItem để gán cho listview
        public IList<ListViewItem> getListViewItems()
        {
            if (this.TilesMap == null)
                return null;
            if (this.TilesMap.TileSet == null)
                return null;   

            List<ListViewItem> result= new List<ListViewItem>();
            foreach (Tile tile in this.TilesMap.TileSet.ListTiles)
            {
                result.Add(new TileItem(tile));
            }
            return result;
        }

        // get imagelist để gán cho listview
        public ImageList getImageList()
        {
            if (this.TilesMap == null)
                return null;
            if (this.TilesMap.TileSet == null)
                return null;
            ImageList imagelist = new ImageList();
            foreach (Tile tile in this.TilesMap.TileSet.ListTiles)
            {
                imagelist.Images.Add(tile.getbitmap());
            }
            imagelist.ImageSize = new Size(40, 40);     //hard code
            return imagelist;
        }

        public void RenderQuadTree()
        {
            if (this.ObjectEditor != null && this.Graphics != null)
            {
                this.ObjectEditor.RenderQuadTree(this.Graphics);
                OnDraw(null);       
            }
        }
    }
}
