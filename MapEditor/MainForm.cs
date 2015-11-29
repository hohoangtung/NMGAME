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
        public static ApplicationSettings Settings =  new ApplicationSettings();

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

        private BufferedGraphics _buffergraphics;
        private Graphics _graphics;
        private int _scrollIndex;
        private METoolbar toolbar;

        // CONTRUCTOR
        // -------------------------------------------------------------

        public MainForm()
        {
            InitializeComponent();
            _mapController = new MapController();
            _mapController.Drawn += (object sender, EventArgs e) =>
                {
                    this._buffergraphics.Render(this._graphics);
                };
            InitToolBar();
            this.disableSaveButton();
        }

        // PRIVATE METHOD
        // -------------------------------------------------------------

        // Khởi tạo toolbar của form
        private void InitToolBar()
        {
            this.toolbar = new METoolbar();
            this.toolbar.Init();

            this.Controls.Add(toolbar);
        }

        // Clone map từ file có sẵn
        private void cloneMap(Image img, int widthpixel, int heightpixel)
        {
            Point postion = Point.Empty;
            //MatrixSize = new Point(img.Width / widthpixel, img.Height / heightpixel);

            _mapController.TilesMap.Resize(img.Width / widthpixel, img.Height / heightpixel);

            this.InitTableLayout();

            // Khởi tạo ObjectEditor
            this._mapController.InitObjectEditor();
            this._mapController.ObjectEditor.Bind(this.listBoxObject);

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
        public void InitTableLayout()
        {
            if (_tablelayout != null)
            {
                this.splitContainer2.Panel1.Controls.Remove(_tablelayout);
            }
            int columns = this._mapController.TilesMap.Columns;
            int rows = this._mapController.TilesMap.Rows;
                _tablelayout = new TableLayoutPanel();

            _tablelayout.ColumnCount = columns;
            // khởi tạo ma trận index
            // khởi tạo cột
            Size tilesize = MainForm.Settings.TileSize;
            float columnsize = 100.0f / columns;
            for (int i = 0; i < columns; i++)
            {
                var columnstyle = new ColumnStyle(SizeType.Absolute, tilesize.Width - 1);
                _tablelayout.ColumnStyles.Add(columnstyle);
            }
            // khởi tạo dòng
            _tablelayout.RowCount = rows;
            float rowsize = 100.0f / rows;
            for (int i = 0; i < rows; i++)
            {
                var rowstyle = new RowStyle(SizeType.Absolute, tilesize.Height - 1);
                _tablelayout.RowStyles.Add(rowstyle);
            }

            _tablelayout.BackColor = Color.FromArgb(205, 205, 205);
            _tablelayout.CellBorderStyle = TableLayoutPanelCellBorderStyle.Single;
            _tablelayout.Margin = new System.Windows.Forms.Padding(5);
            _tablelayout.Name = "map";

            _tablelayout.AutoSize = true;
            _tablelayout.Paint += _tablelayout_Paint;
            _tablelayout.MouseClick += TablelayoutMouseClick;
            this.splitContainer2.Panel1.Controls.Add(_tablelayout);
            this.splitContainer2.SplitterDistance = (int)(this.splitContainer2.Size.Height * 0.75);

            _tablelayout.MouseDown += TablelayoutMouseDown;
            _tablelayout.MouseUp += TablelayoutMouseUp;

            // Khởi tạo graphics.
            _graphics = _tablelayout.CreateGraphics();
            var context = BufferedGraphicsManager.Current;
            context.MaximumBuffer = _tablelayout.Size + new Size(1, 1);
            _buffergraphics = context.Allocate(_tablelayout.CreateGraphics(), new Rectangle(Point.Empty, _tablelayout.Size));

            this._mapController.Graphics = _buffergraphics.Graphics;

        }

        private Rectangle getVisibleMap()
        {
            var size = this.splitContainer2.Panel1.ClientSize;
            Point location = new Point(
                this.splitContainer2.Panel1.HorizontalScroll.Value,
                this.splitContainer2.Panel1.VerticalScroll.Value);
            return new Rectangle(location, size);
        }

        // event

        private void TablelayoutMouseClick(object sender, MouseEventArgs e)
        {
            if (this.toolbar.Buttons["editstate"].Pushed == true)
                return;
            if (_selectedTile == null)
                return;
            Size tilesize = MainForm.Settings.TileSize;
            // selected point là chỉ số của matrix 
            Point selectedPoint = new Point(e.X / tilesize.Width, e.Y / tilesize.Height);
            if (selectedPoint.X >= _mapController.TilesMap.Columns)
                return;
            if (selectedPoint.Y >= _mapController.TilesMap.Rows)
                return;
            _mapController.TilesMap[selectedPoint.X, selectedPoint.Y] = _selectedTile.Id;

            // location là vị trí vẽ trên tablelayout
            Point location = new Point(tilesize.Width * selectedPoint.X, tilesize.Height * selectedPoint.Y);
            _mapController.DrawTile(location, _selectedTile);
            this.enableSaveButton();            
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
            _mapController.Draw(getVisibleMap());
            if (this.toolbar != null && this.toolbar.QuadTree.Pushed)
            {
                this.DrawQuadTree();
            }
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
                this.InitTableLayout();
                // Khởi tạo ObjectEditor
                this._mapController.InitObjectEditor();
                this._mapController.ObjectEditor.Bind(this.listBoxObject);

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
            this.Save();
        }
        private string saveasDialog()
        {
            var filedialog = new SaveFileDialog();
            filedialog.Filter = "XML Files (*.xml)|*.xml";
            var result = filedialog.ShowDialog();
            if (result != System.Windows.Forms.DialogResult.OK)
                return String.Empty;
            return filedialog.FileName;
        }
        public void SaveAs()
        {
            string newpath = saveasDialog();
            if (String.IsNullOrEmpty(newpath) == false)
            {
                this._tilesetPath = newpath;
            }
            else
                return;
            Cursor.Current = Cursors.WaitCursor;
            TilesMap.Save(_mapController.TilesMap, this._tilesetPath);
            Cursor.Current = Cursors.Default;
            this.disableSaveButton();
        }
        public void Save()
        {
            if (String.IsNullOrEmpty(this._tilesetPath) == true)
            {
                string newpath = saveasDialog();
                if (String.IsNullOrEmpty(newpath))
                    return;
                else
                    this._tilesetPath = newpath;
            }
            Cursor.Current = Cursors.WaitCursor;
            TilesMap.Save(_mapController.TilesMap, this._tilesetPath);
            Cursor.Current = Cursors.Default;
            this.disableSaveButton();
        }
        private void disableSaveButton()
        {
            this.saveMapToolStripMenuItem.Enabled = false;
            if (this.toolbar.Save != null)
            {
                toolbar.Save.Enabled = false;
            }
        }
        private void enableSaveButton()
        {
            this.saveMapToolStripMenuItem.Enabled = true;
            if (this.toolbar.Save != null)
            {
                toolbar.Save.Enabled = true;
            }
        }
        private void loadMapToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.LoadMap();
            if (_mapController.TilesMap != null)
            {
                 var mapbound = new Rectangle(0,0,
                    this._mapController.TilesMap.GetMapWidth(),
                    this._mapController.TilesMap.GetMapHeight());
                this._mapController.ObjectEditor.InitQuadTree(0, mapbound);           
            }
        }

        public void LoadMap()
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
            Cursor.Current = Cursors.WaitCursor;
            _mapController.TilesMap = TilesMap.Load(openfiledialog.FileName);
            this._tilesetPath = openfiledialog.FileName;

            listView1.LargeImageList = _mapController.getImageList();
            listView1.Items.AddRange(_mapController.getListViewItems().ToArray());

            this.InitTableLayout();

            // Khởi tạo ObjectEditor
            this._mapController.InitObjectEditor();
            this._mapController.ObjectEditor.Bind(this.listBoxObject);
            this._mapController.ObjectEditor.ListItem.ListChanged += (object s, ListChangedEventArgs arg) =>
            {
                this.enableSaveButton();
            };
            _mapController.Draw(getVisibleMap());
            Cursor.Current = Cursors.Default;
            this.disableSaveButton();
        }

        private void TablelayoutMouseDown(object sender, MouseEventArgs e)
        {
            if (this.toolbar.Buttons["editstate"].Pushed == false)
                return;
            System.Diagnostics.Debug.WriteLine("down");
            if (MainForm.Settings.UseTransform == true)
            {
                int height = this._mapController.TilesMap.GetMapHeight();
                this._mapController.ObjectEditor.MouseDown = new Point(e.Location.X, height - e.Location.Y);
            }
            else
            {
                this._mapController.ObjectEditor.MouseDown = e.Location;
            }
        }

        private void TablelayoutMouseUp(object sender, MouseEventArgs e)
        {
            if (this.toolbar.Buttons["editstate"].Pushed == false)
                return;
            System.Diagnostics.Debug.WriteLine("Up");
            if (MainForm.Settings.UseTransform == true)
            {
                int height = this._mapController.TilesMap.GetMapHeight();
                this._mapController.ObjectEditor.MouseUp = new Point(e.Location.X, height - e.Location.Y);
            }
            else
            {
                this._mapController.ObjectEditor.MouseUp = e.Location;
            }
            this._mapController.ObjectEditor.InitGameObject();
            var mapbound = new Rectangle(0, 0,
                this._mapController.TilesMap.GetMapWidth(),
                this._mapController.TilesMap.GetMapHeight());
            this._mapController.ObjectEditor.InitQuadTree(0, mapbound);
            this.ReDrawMap();
            this.enableSaveButton();
        }

        private void listBoxObject_SelectedIndexChanged(object sender, EventArgs e)
        {
            this.gameObjectproperty.SelectedObject = ((sender as ListBox).SelectedItem as GameObject);
        }

        private void Panel_Scroll(object sender, ScrollEventArgs e)
        {
            if (e.Type == ScrollEventType.EndScroll)
            {
                this._scrollIndex = e.NewValue;
            }
        }

        private void gameObjectproperty_PropertyValueChanged(object s, PropertyValueChangedEventArgs e)
        {
            (this.listBoxObject.DataSource as BindingSource).ResetBindings(false);
            //this.ReDrawMap();
            this.enableSaveButton();
        }

        private void listBoxObject_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == System.Windows.Forms.MouseButtons.Right)
            {
                listBoxObject.SelectedIndex = listBoxObject.IndexFromPoint(e.Location);
                if (listBoxObject.SelectedIndex != -1)
                {
                    ContextMenuListBox.Show(listBoxObject.PointToScreen(e.Location));
                }
            }
        }

        private void ContextMenuListBox_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
        {
            if (e.ClickedItem.Text == "Delete")
            {
                (listBoxObject.DataSource as BindingSource).RemoveCurrent();
            }
            else if (e.ClickedItem.Text == "Fit Tile")
            {
                var current = (listBoxObject.DataSource as BindingSource).Current as GameObject;
                ObjectEditor.FitTile(current);

                this._mapController.Draw(getVisibleMap());
            }
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            MainForm.Settings.PropertyChanged += (object s, PropertyChangedEventArgs property_event) =>
                {
                    if (this.Focused == true)
                        this._mapController.Draw(getVisibleMap());
                    if (_mapController.TilesMap != null)
                    {
                        MapController.MapSize = new Size(
                            _mapController.TilesMap.Columns * MainForm.Settings.TileSize.Width,
                            _mapController.TilesMap.Rows * MainForm.Settings.TileSize.Height);

                        var mapbound = new Rectangle(0, 0,
                                this._mapController.TilesMap.GetMapWidth(),
                                this._mapController.TilesMap.GetMapHeight());
                        this._mapController.ObjectEditor.InitQuadTree(0, mapbound);
                    }
                };
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            MainForm.Settings.Save();
        }

        public void ReDrawMap()
        {
            this._mapController.Draw(this.getVisibleMap());
        }
        public void DrawQuadTree()
        {
            this._mapController.RenderQuadTree();
        }

        private void exportQTreeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.ExportQuadTree();
        }

        private void ExportQuadTree()
        {
            var filedialog = new SaveFileDialog();
            filedialog.Filter = "XML Files (*.xml)|*.xml";
            var result = filedialog.ShowDialog();
            if (result != System.Windows.Forms.DialogResult.OK)
                return;
            string filename = filedialog.FileName;
            Cursor.Current = Cursors.WaitCursor;
            ObjectEditor.SaveQuadTree(this._mapController.ObjectEditor.QuadTree, filename);
            Cursor.Current = Cursors.Default;
        }
    } // END CLASS mainform
} // END NAMESPACE mapeditor
