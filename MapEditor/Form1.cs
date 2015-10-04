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
    public partial class Form1 : Form
    {
        private Image _img;
        private int _widthCount;
        private int _heightCount;
        private Pen _pen;
        private Graphics _graphics;
        public int WidthCount
        {
            get { return _widthCount; }
            set { _widthCount = value; }
        }

        public int HeightCount
        {
            get { return _heightCount; }
            set { _heightCount = value; }
        }
        public int WidthTile { get; set; }
        public int HeightTile { get; set; }
        public Image Image { get { return _img; } }
       
        private BufferedGraphicsContext _context;
        private BufferedGraphics _buffergraphics;
        private System.Drawing.Drawing2D.GraphicsState _state;
        public Form1()
        {
            InitializeComponent();
            _pen = new Pen(new SolidBrush(Color.Blue), 0.5f);
            _graphics = this.panel_tile.CreateGraphics();
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer, true);
            _context = BufferedGraphicsManager.Current;
            
            _context.MaximumBuffer = this.panel_tile.Size + new Size(1,1);
            _buffergraphics = _context.Allocate(this.panel_tile.CreateGraphics(), new Rectangle(Point.Empty, panel_tile.Size));
        }

        private void button_browse_Click(object sender, EventArgs e)
        {
            var openfileDialog = new OpenFileDialog();
            var rs = openfileDialog.ShowDialog();
            if (rs == System.Windows.Forms.DialogResult.OK)
            {
                Stream fileStream = openfileDialog.OpenFile(); 
                _img = Image.FromStream(fileStream);
                _buffergraphics.Graphics.DrawImage(_img,new Rectangle(Point.Empty, panel_tile.Size));
                _buffergraphics.Render(_graphics);
                _state = _buffergraphics.Graphics.Save();
            }
        }


        private void button_OK_Click(object sender, EventArgs e)
        {
            int width = 0;
            int height = 0;
            if (Int32.TryParse(this.textBox_height.Text, out height) == false)
                return;
            if (Int32.TryParse(this.textBox_height.Text, out height) == false)
                return;
            this.WidthTile = width;
            this.HeightTile = height;
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }

        private void drawgridview(int wcount, int hcount)
        {
            _buffergraphics.Graphics.Clear(Color.Black);
            _buffergraphics.Graphics.DrawImage(_img, new Rectangle(Point.Empty, panel_tile.Size));
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
        private void textBox_width_TextChanged(object sender, EventArgs e)
        {
            int w;
            if (Int32.TryParse((sender as TextBox).Text, out w) == true)
            {
                this.WidthTile = w;
                this.WidthCount = _img.Width / w;
            }
            else
            {
                this.WidthTile = 0;
                this.WidthCount = 1;
            }
            drawgridview(WidthCount, HeightCount);
        }

        private void textBox_height_TextChanged(object sender, EventArgs e)
        {
            int h;
            if (Int32.TryParse((sender as TextBox).Text, out h) == true)
            {
                this.HeightTile = h;
                this.HeightCount = _img.Height / h;
            }
            else
            {
                this.HeightTile = 0;
                this.HeightCount = 1;
            }
            drawgridview(WidthCount, HeightCount);
        }

        private void panel_tile_Paint(object sender, PaintEventArgs e)
        {
            if (_img == null)
                return;
            _buffergraphics.Dispose();
            _buffergraphics = _context.Allocate(_graphics, new Rectangle(Point.Empty, panel_tile.Size));
            drawgridview(WidthCount, HeightCount);
        }


    }
}
