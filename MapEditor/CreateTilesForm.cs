using MapEditor.Tiles;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
{
    public partial class CreateTilesForm : Form
    {
        // CONTRUCTOR
        // -------------------------------------------------------------

        public CreateTilesForm()
        {
            InitializeComponent();
            _pen = new Pen(new SolidBrush(Color.Blue), 0.5f);
            _graphics = this.panel_tile.CreateGraphics();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true); // dont know
            ListViewItems = new List<ListViewItem>();
            // khởi tạo backbuffer với độ rộng lớn hơn 1 (recommend)
            _context = BufferedGraphicsManager.Current;
            _context.MaximumBuffer = this.panel_tile.Size + new Size(1, 1);
            _buffergraphics = _context.Allocate(_graphics, new Rectangle(Point.Empty, panel_tile.Size));
            // bind tileset
            _tileset = new Tiles.TileSet();
            this.textBoxWidth.DataBindings.Add("Text", _tileset, "Widthtile");
            this.textBoxHeight.DataBindings.Add("Text", _tileset, "Hieghttile");
        }

        // PRIVATE ATTRIBUTE
        // ---------------------------------------------------------
        private Pen _pen;
        private Graphics _graphics;
        private TileSet _tileset;
        private BufferedGraphicsContext _context;
        private BufferedGraphics _buffergraphics;
        
        // PRIVATE METHOD
        // --------------------------------------------------------

        // vẽ lưới kẻ 
        private void drawgridview(int wcount, int hcount)
        {
            _buffergraphics.Graphics.Clear(Color.Black);
            _buffergraphics.Graphics.DrawImage(Tileset.Image, new Rectangle(Point.Empty, panel_tile.Size));
            // vẽ đường kẻ dọc
            float tileWidth = ((float)this.panel_tile.Width) / wcount;
            for (int i = 0; i < wcount; i++)
            {
                int drawpoint = (int)(tileWidth * i) - 1;
                _buffergraphics.Graphics.DrawLine(
                    _pen,
                    new Point(drawpoint, 0),
                    new Point(drawpoint, panel_tile.Height)
                    );
            }
            // vẽ đường kẻ ngang
            float tileheight = ((float)this.panel_tile.Height) / hcount;
            for (int i = 0; i < hcount; i++)
            {
                int drawpoint = (int)(tileheight * i) - 1;
                _buffergraphics.Graphics.DrawLine(
                    _pen,
                    new Point(0, drawpoint),
                    new Point(panel_tile.Width, drawpoint)
                    );
            }
            _buffergraphics.Render(_graphics);
        }
        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            var openfileDialog = new OpenFileDialog();
            openfileDialog.Filter = "Image files (*.jpg, *.jpeg, *.jpe, *.jfif, *.png) | *.jpg; *.jpeg; *.jpe; *.jfif; *.png";
            var result = openfileDialog.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                Stream fileStream = openfileDialog.OpenFile();
                Tileset.FileName = openfileDialog.FileName;
                Tileset.Image = Image.FromStream(fileStream);
                _buffergraphics.Graphics.DrawImage(Tileset.Image, new Rectangle(Point.Empty, panel_tile.Size));
                _buffergraphics.Render(_graphics);
                this.textBoxHeight.Enabled = true;
                this.textBoxWidth.Enabled = true;
            }
        }
        private void buttonOK_Click(object sender, EventArgs e)
        {
            Rectangle rect = new Rectangle(0, 0, this.Tileset.Widthtile, this.Tileset.Hieghttile);

            for (int j = 0; j < this.Tileset.Rows; j++)
            {
                for (int i = 0; i < this.Tileset.Columns; i++)
                {

                    rect.Location = new Point(i * rect.Width, j * rect.Height);
                   // var bm = (Tileset.Image as Bitmap).Clone(rect, Tileset.Image.PixelFormat);
                    this.Tileset.ListTiles.Add(new Tile(
                        Tileset.Image,
                        new Rectangle(rect.Location, new Size(rect.Size.Width - 1, rect.Size.Height - 1)),
                        j * Tileset.Columns + i + 1)
                        );
                    this.ListViewItems.Add(new TileItem(
                        Tileset.ListTiles.Last()
                        ));
                }
            }
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
        private void textBoxWidth_TextChanged(object sender, EventArgs e)
        {
            if (Tileset.Image == null)
                return;
            int w;
            if (Int32.TryParse((sender as TextBox).Text, out w) == true)
            {
                if (w == 0)
                    return;
                w = Tileset.Image.Width / w;
            }
            else
            {
                w = 1;
            }
            drawgridview(w, Tileset.Rows);
        }
        private void textBoxHeight_TextChanged(object sender, EventArgs e)
        {
            if (Tileset.Image == null)
                return;
            int h;
            if (Int32.TryParse((sender as TextBox).Text, out h) == true)
            {
                if (h == 0)
                    return;
                h = Tileset.Image.Height / h;
            }
            else
            {
                h = 1;
            }
            drawgridview(Tileset.Columns, h);
        }
        private void panel_tile_Paint(object sender, PaintEventArgs e)
        {
            if (Tileset.Image == null)
                return;
            _buffergraphics.Dispose();
            _buffergraphics = _context.Allocate(_graphics, new Rectangle(Point.Empty, panel_tile.Size));
            drawgridview(Tileset.Columns, Tileset.Rows);
        }

        // PUBLIC PROPERTY
        // --------------------------------------------------------

        public TileSet Tileset
        {
            get { return _tileset; }
            set { _tileset = value; }
        }
        public List<ListViewItem> ListViewItems { get; set; }

        // PUBLIC METHOD
        // --------------------------------------------------------

        

    }
}
