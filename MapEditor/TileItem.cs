using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor.Tiles
{
    public class TileItem : ListViewItem
    {
        private Tile _tile;
        public Tile Tile
        {
            get
            {
                return _tile;
            }
        }
        public TileItem(Tile tile)
            :base (tile.Name, tile.Id - 1)
        {
            _tile = tile;
            
        }

    }
}
