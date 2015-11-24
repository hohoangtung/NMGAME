using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Documents;
using System.Windows.Forms;

namespace MapEditor.Tiles
{
    class MapController
    {
        public MapController()
        {
        }

        // Hardcode: Kích thước tile dùng để vẽ trên graphics.
        public static Size TilesSize = new Size(50, 50);

        // Graphics dùng để vẽ hình.
        public Graphics Graphics{get; set;}

        public TilesMap TilesMap { get; set; }

        // Duyệt matrixindex và vẽ map.
        public void Draw()
        {
            if (this.Graphics == null)
            {
                throw new Exception("Khong tim thay graphics");
            }
            if (this.TilesMap == null)
                return;
            if (this.TilesMap.TileSet == null)
                return;
            for (int i = 0; i < TilesMap.Columns; i++)
            {
                for (int j = 0; j < TilesMap.Rows; j++)
                {
                    Tile tile = TilesMap.TileSet.ListTiles.ToList().Find(t => t.Id == TilesMap[i, j]);
                    if (tile == null)
                        continue;
                    tile.draw(
                        Graphics,
                        new Point(MapController.TilesSize.Width * i, MapController.TilesSize.Height * j),
                        MapController.TilesSize); // notice here
                }
            }
        }

        // Vẽ một tile lên toạ độ trên 
        public void DrawTile(Point map_coordinate, Tile tile)
        {
            tile.draw(this.Graphics, map_coordinate, MapController.TilesSize);         //hardcode
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
    }
}
