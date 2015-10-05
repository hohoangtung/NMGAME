using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
{
    class TileItem : ListViewItem
    {
        private Tile _tile;
        public Tile Tile
        {
            get
            {
                return _tile;
            }
        }
        public TileItem(Tile tile, string tilename , int imageIndex)
            :base (tilename, imageIndex)
        {
            _tile = tile;
        }

    }
}
