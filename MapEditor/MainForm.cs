using MapEditor.Tiles;
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
        // PRIVATE ATTRIBUTE
        // -------------------------------------------------------------

        // Tile đang được chọn trong list view.
        private Tile _selectedTile;  

        // Lưới kẻ ô.
        private TableLayoutPanel _tablelayout;

        // Class chính dùng để điều khiển hành vi và chứa đối tượng TileMap.
        private MapController _mapController;

        // Để lưu đường dẫn tạm cho file xml.
        // khi save nếu path này rỗng thì mở dialog yêu cầu đừng dẫn, ngược lại ta dùng path này để lưu
        // khi load nếu path này rỗng thì load bình thường, ngược lại, ta lưu lại trước rồi mới load.
        private string _tilesetPath;

        // CONTRUCTOR
        // -------------------------------------------------------------

        public MainForm()
        {
            InitializeComponent();
            _mapController = new MapController();
        }


        // PRIVATE METHOD
        // -------------------------------------------------------------

        // Clone map từ file có sẵn
        private void cloneMap(Image img, int widthpixel, int heightpixel)
        {
            Point postion = Point.Empty;
            //MatrixSize = new Point(img.Width / widthpixel, img.Height / heightpixel);

            _mapController.TilesMap.Resize(img.Width / widthpixel, img.Height / heightpixel);

            initTableLayout(_mapController.TilesMap.Columns, _mapController.TilesMap.Rows);
            Bitmap bm = new Bitmap(widthpixel, heightpixel, img.PixelFormat);           //
            Rectangle srcRect = new Rectangle(0, 0, widthpixel - 1, heightpixel - 1);
            var imglist = this.listView1.LargeImageList.Images;
            for (int i = 0; i < _mapController.TilesMap.Columns; i++)
            {
                for (int j = 0; j < _mapController.TilesMap.Rows; j++)
                {
                    srcRect.Location = new Point(i * widthpixel, j * heightpixel);
                    bm = (img as Bitmap).Clone(srcRect, img.PixelFormat);
                    foreach (var tile in _mapController.TilesMap.TileSet.ListTiles)
                    {
                        if (this.compareBitmap(tile.getbitmap(), bm as Bitmap) == true)
                        {
                            _mapController.TilesMap[i, j] = tile.Id;
                        }
                    }
                }
            }
        }

        // So sánh 2 ảnh (Helper cho hàm clonemap).
        private bool compareBitmap(Bitmap bm1, Bitmap bm2, float accuracy = 1.0f)
        {
            if (accuracy > 1 || accuracy < 0)   // độ chính xác từ 0 đến 1
                throw new Exception("accuracy between 0 and 1");
            int bytes = bm1.Width * bm1.Height * (Image.GetPixelFormatSize(bm1.PixelFormat) / 8);
            byte[] bytearray1 = new byte[bytes];
            byte[] bytearray2 = new byte[bytes];
            System.Drawing.Imaging.BitmapData bitmapdata1 = bm1.LockBits(
                new Rectangle(0, 0, bm1.Width - 1, bm1.Height - 1),
                System.Drawing.Imaging.ImageLockMode.ReadOnly,
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

            _tablelayout.AutoSize = true;
            _tablelayout.Paint += _tablelayout_Paint;
            _tablelayout.MouseClick += tablelayout_MouseClick;
            this.splitContainer1.Panel2.Controls.Add(_tablelayout);

            this._mapController.Graphics = _tablelayout.CreateGraphics();
        }

        // event
        private void tablelayout_MouseClick(object sender, MouseEventArgs e)
        {
            if (_selectedTile == null)
                return;

            // selected point là chỉ số của matrix 
            Point selectedPoint = new Point(e.X / 50, e.Y / 50);
            _mapController.TilesMap[selectedPoint.X, selectedPoint.Y] = _selectedTile.Id;

            // location là vị trí vẽ trên tablelayout
            Point location = new Point(50 * selectedPoint.X, 50 * selectedPoint.Y);
            _mapController.DrawTile(location, _selectedTile);
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

        private void _tablelayout_Paint(object sender, PaintEventArgs e)
        {
            _mapController.Draw();
        }

        // menu toolstrip event
        private void creatTilesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this._mapController.CreateTilesSet();
            if (this._mapController.TilesMap == null)
                return;   
            if (this._mapController.TilesMap.TileSet == null)
                return;
            this.listView1.LargeImageList = this._mapController.getImageList();
            
            if (listView1.Items.Count > 0)
                listView1.Items.Clear();
            this.listView1.Items.AddRange(_mapController.getListViewItems().ToArray());
        }

        private void menuStrip1_ItemClicked(object sender, EventArgs e)
        {
            var newmapform = new NewMaps();
            var rs = newmapform.ShowDialog();
            if (rs == System.Windows.Forms.DialogResult.OK)
            {
                //MatrixSize = new Point(newmap.Columns, newmap.Rows);
                _mapController.TilesMap = new TilesMap(newmapform.Columns, newmapform.Rows);
                this.initTableLayout(newmapform.Columns, newmapform.Rows);
            }
        }

        private void cloneToolStripMenuItem_Click(object sender, EventArgs e)
        {
            var filedialog = new OpenFileDialog();
            filedialog.Filter = "Image Files (*.png)|*.png";
            DialogResult rs = filedialog.ShowDialog();
            if (rs == System.Windows.Forms.DialogResult.OK)
            {
                System.IO.Stream fstream = filedialog.OpenFile();
                Bitmap sourcebitmap = Bitmap.FromStream(fstream) as Bitmap;
                cloneMap(sourcebitmap, 16, 16);
            }
        }

        private void listView1_AfterLabelEdit(object sender, LabelEditEventArgs e)
        {
            ((sender as ListView).Items[e.Item] as TileItem).Tile.Name = e.Label;
        }

        private void saveMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (String.IsNullOrEmpty(this._tilesetPath) == true)
            {
                var filedialog = new SaveFileDialog();
                filedialog.Filter = "XML Files (*.xml)|*.xml";
                var result = filedialog.ShowDialog();
                if (result != System.Windows.Forms.DialogResult.OK)
                    return;
                this._tilesetPath = filedialog.FileName;
            }
            TilesMap.Save(_mapController.TilesMap, this._tilesetPath);
        }

        private void loadMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (String.IsNullOrEmpty(this._tilesetPath) == false)
            {
                string name = _tilesetPath.Substring(_tilesetPath.LastIndexOf('\\'));
                var result = MessageBox.Show(
                    "Do you want save " + name + "?",
                    "Save TileSet", MessageBoxButtons.OKCancel,
                    MessageBoxIcon.Question,
                    MessageBoxDefaultButton.Button2,
                    MessageBoxOptions.ServiceNotification);
                if (result == System.Windows.Forms.DialogResult.OK)
                {
                    TilesMap.Save(_mapController.TilesMap, _tilesetPath);
                }
                _tilesetPath = String.Empty;
            }
            var openfiledialog = new OpenFileDialog();
            openfiledialog.Filter = "XML Files (*.xml)|*.xml";
            var rs = openfiledialog.ShowDialog();
            if (rs != System.Windows.Forms.DialogResult.OK)
                return;
            _mapController.TilesMap = TilesMap.Load(openfiledialog.FileName);

            listView1.LargeImageList = _mapController.getImageList();
            listView1.Items.AddRange(_mapController.getListViewItems().ToArray());

            initTableLayout(_mapController.TilesMap.Columns, _mapController.TilesMap.Rows);
            _mapController.Draw();
            
        }


    } // END CLASS mainform
} // END NAMESPACE mapeditor
