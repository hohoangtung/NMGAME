using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
{
    public partial class MainForm : Form
    {
        private Point _matrixSize;          // kích thước ma trận index

        private int[,] _matrixIndex;        // ma trận index lưu index của tile
        private List<Tile> _listTile;       // danh sách các tile
        private Tile _selectedTile;         // current tile
        private TableLayoutPanel _tablelayout;  // lưới kẻ ô ở background
        private Bitmap _bitmap;

        // kích thước ma trận index
        public Point MatrixSize
        {
            get { return _matrixSize; }
            set 
            {
                _matrixSize = value;
                _matrixIndex = new int[value.X, value.Y];
            }
        }
        public MainForm()
        {
            InitializeComponent();
            _listTile = new List<Tile>();
            _matrixSize = Point.Empty;
            _matrixIndex = null;
            _selectedTile = null;
            _bitmap = null;
        }

        private void creatTilesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form1 form = new Form1();
            var rs = form.ShowDialog();
            if (rs == System.Windows.Forms.DialogResult.OK)
            {
                // ảnh được load từ file
                _bitmap = (Bitmap)form.Image;
                // số tile theo chiều width (số cột)
                int widthcount = form.WidthCount;

                // số tile theo chiều height (số dòng)
                int heightcount = form.HeightCount;

                //hcn xác định ô tile
                Rectangle rct = new Rectangle(0, 0, _bitmap.Width / widthcount, _bitmap.Height / heightcount);
                
                // image list để gán vào listItem
                var imglist = new ImageList();

                int temp = heightcount * widthcount;
                // cắt từ hình được load. tạo imagelist --> listitem
                for (int i = 0; i < temp; i++)
                {
                    rct.Location = new Point(
                        (i % widthcount) * rct.Width,
                        (i / widthcount) * rct.Height
                        );
                    var bm = _bitmap.Clone(rct, _bitmap.PixelFormat);
                    var bm2 = _bitmap.Clone(rct, _bitmap.PixelFormat);
                    imglist.Images.Add(bm);
                    imglist.ImageSize = new Size(40, 40);
                    _listTile.Add(new Tile(_bitmap, new Rectangle(rct.Location, new Size(rct.Size.Width - 1, rct.Size.Height - 1)), i + 1));
                    listView1.Items.Add(new TileItem(_listTile[i], "tile_" + i.ToString(), i));
                    
                }

                listView1.LargeImageList = imglist;


            }

        }

        /// <summary>
        /// dùng để tạo ma trận index tự động (clone map)
        /// </summary>
        /// <param name="img"> ảnh gốc từ file</param>
        /// <param name="widthpixel"> chiều rộng tile</param>
        /// <param name="heightpixel">chiểu cao tile</param>
        private void cloneMap(Image img, int widthpixel, int heightpixel)
        {
            Point postion = Point.Empty;            
            MatrixSize = new Point( img.Width / widthpixel, img.Height / heightpixel);
            initTableLayout(MatrixSize.X, MatrixSize.Y);
            Bitmap bm = new Bitmap(widthpixel, heightpixel, img.PixelFormat);           //
            Rectangle srcRect = new Rectangle(0,0,widthpixel - 1, heightpixel - 1);
            var imglist = this.listView1.LargeImageList.Images;
            for (int i = 0; i < MatrixSize.X; i++)
            {
                for (int j = 0; j < MatrixSize.Y; j++)
                {
                    srcRect.Location = new Point(i * widthpixel, j * heightpixel);
                    bm = (img as Bitmap).Clone(srcRect, img.PixelFormat);
                    foreach (var tile in this._listTile)
                    {
                        if (this.compareBitmap(tile.getbitmap(), bm as Bitmap) == true)
                        {
                            this._matrixIndex[i, j] = tile.Id;
                        }
                    }
                }
            }
        }
        // so sánh 2 ảnh
        private bool compareBitmap(Bitmap bm1, Bitmap bm2, float accuracy = 1.0f)
        {
            if (accuracy > 1 || accuracy < 0)   // độ chính xác từ 0 đến 1
                throw new Exception("accuracy between 0 and 1");
            int bytes = bm1.Width * bm1.Height * (Image.GetPixelFormatSize(bm1.PixelFormat) / 8);
            byte[] bytearray1 = new byte[bytes];
            byte[] bytearray2 = new byte[bytes];
            System.Drawing.Imaging.BitmapData bitmapdata1 = bm1.LockBits(
                new Rectangle(0,0, bm1.Width - 1, bm1.Height - 1),
                System.Drawing.Imaging.ImageLockMode.ReadOnly ,
                bm1.PixelFormat);
            System.Drawing.Imaging.BitmapData bitmapdata2 = bm2.LockBits(
                new Rectangle(0, 0, bm2.Width - 1, bm2.Height - 1),
                System.Drawing.Imaging.ImageLockMode.ReadOnly,
                bm1.PixelFormat);

            System.Runtime.InteropServices.Marshal.Copy(bitmapdata1.Scan0, bytearray1, 0, bytes);
            System.Runtime.InteropServices.Marshal.Copy(bitmapdata2.Scan0, bytearray2, 0, bytes);
            int dif = 0;    // độ lệch
            for (int i = 0; i < bytes - 1; i++)
            {
                if (bytearray1[i] != bytearray2[i])
                    dif++;
            }
            bm1.UnlockBits(bitmapdata1);
            bm2.UnlockBits(bitmapdata2);
            // nếu độ chính xác cộng độ lệch nhỏ hơn một thì đúng (accuracy <= 1 - dif)
            if (accuracy + ((float)dif) / bytes <= 1.0f)
                return true;
            return false;
        }
        private void splitContainer1_Panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void menuStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
             
        }

        private void menuStrip1_ItemClicked(object sender, EventArgs e)
        {
            var newmap = new NewMaps();
            var rs = newmap.ShowDialog();
            if (rs == System.Windows.Forms.DialogResult.OK)
            {
                MatrixSize = new Point(newmap.Columns, newmap.Rows);
                this.initTableLayout(newmap.Columns, newmap.Rows);

            }
        }
        // khởi tạo lưới khung
        private void initTableLayout(int columns, int rows)
        {
            _tablelayout = new TableLayoutPanel();

            _tablelayout.ColumnCount = columns;
            // khởi tạo ma trận index
            // khởi tạo cột
            float columnsize = 100.0f / columns;
            for (int i = 0; i < columns; i++)
            {
                var columnstyle = new ColumnStyle(SizeType.Absolute, 49);
                _tablelayout.ColumnStyles.Add(columnstyle);
            }
            // khởi tạo dòng
            _tablelayout.RowCount = rows;
            float rowsize = 100.0f / rows;
            for (int i = 0; i < rows; i++)
            {
                var rowstyle = new RowStyle(SizeType.Absolute, 49);
                _tablelayout.RowStyles.Add(rowstyle);
            }

            _tablelayout.BackColor = Color.FromArgb(205, 205, 205);
            _tablelayout.CellBorderStyle = TableLayoutPanelCellBorderStyle.Single;
            _tablelayout.Margin = new System.Windows.Forms.Padding(0, 0, 0, 0);
            _tablelayout.Name = "map";
            var s = 50 * columns* rows;
            _tablelayout.AutoSize = true;
            //_tablelayout.Size = new Size(50 * newmap.Columns, 50 * newmap.Rows);
            _tablelayout.Paint += _tablelayout_Paint;
            _tablelayout.MouseClick += tablelayout_MouseClick;
            this.splitContainer1.Panel2.Controls.Add(_tablelayout);
        }
        private void _tablelayout_Paint(object sender, PaintEventArgs e)
        {
            //var graphics = (sender as TableLayoutPanel).CreateGraphics();
            //if (graphics.IsClipEmpty == false)
            //    return;
            drawmapfromMatrixIndex();
        }

        private void drawmapfromMatrixIndex()
        {
            for (int i = 0; i < _matrixSize.X; i++)
            {
                for (int j = 0; j < _matrixSize.Y; j++)
                {
                    Tile tile = _listTile.Find(t => t.Id == _matrixIndex[i,j]);
                    if (tile == null)
                        continue;
                    Graphics graphics = this._tablelayout.CreateGraphics();
                    tile.draw(graphics, new Point(50 * i, 50 * j), new Size(50, 50));
                }
            }
        }

        private void tablelayout_MouseClick(object sender, MouseEventArgs e)
        {
            var grp = _tablelayout.CreateGraphics();
            Point selectedPoint = new Point(e.X / 50, e.Y / 50);
            _matrixIndex[selectedPoint.X, selectedPoint.Y] = _selectedTile.Id;
            Point location = new Point(50 * selectedPoint.X, 50 * selectedPoint.Y);
            
            _selectedTile.draw(grp, location, new Size(50, 50));
        }


        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if ((sender as ListView).SelectedItems.Count == 0)
            {
                _selectedTile = null;
                return;
            }
            _selectedTile = ((sender as ListView).SelectedItems[0] as TileItem).Tile;
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (_matrixIndex == null)
                return;
            var filedialog = new SaveFileDialog();
            filedialog.Filter = "Text Files (*.txt)|*.txt";
            DialogResult rs = filedialog.ShowDialog();
            if(rs == System.Windows.Forms.DialogResult.OK)
            {
                try
                {
                    System.IO.Stream fstream = filedialog.OpenFile();
                    System.IO.StreamWriter file = new System.IO.StreamWriter(fstream);
                    file.Write(this._matrixSize.X + "\t");
                    file.Write(this._matrixSize.Y);
                    file.WriteLine();
                    for (int i = 0; i < _matrixSize.Y; i++)
                    {
                        for (int j = 0; j < _matrixSize.X; j++)
                        {
                            file.Write(_matrixIndex[j, i] + "\t");
                        }
                        file.WriteLine();
                    }
                    file.Flush();
                    file.Close();
                }
                catch(Exception exception)
                {
#if DEBUG
                    System.Diagnostics.Debug.WriteLine(exception.Message);
#endif
                }
            }
        }

        private void cloneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var filedialog = new OpenFileDialog();
            //filedialog.Filter = "Image Files (*.png)|*.png";
            DialogResult rs = filedialog.ShowDialog();
            if(rs == System.Windows.Forms.DialogResult.OK)
            {
                System.IO.Stream fstream = filedialog.OpenFile();
                Bitmap sourcebitmap = Bitmap.FromStream(fstream) as Bitmap;
                cloneMap(sourcebitmap, 16, 16);
            }
        }
    }
}
