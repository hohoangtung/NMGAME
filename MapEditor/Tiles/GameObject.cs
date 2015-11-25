using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Design;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Wexman.Design;
/*
 *  Property grid không edit được trên Dictionary nên cần dùng package của Wexman để edit được dictionary.
 *  Để download package vào nuget manager tìm kiếm với từ khoá GenericDictionaryEditor
 *  Cần add reference System.Design
 *  Tham khảo:
 *  http://gendictedit.codeplex.com/SourceControl/latest#trunk/Example/FourtyTwo.cs
 */
namespace MapEditor.Tiles
{
    public class GameObject : INotifyPropertyChanged
    {
        private Rectangle _activebound;
        private Rectangle _initBound;

        private Image _image;
        private int id;
        private string name;
        private Dictionary<string, string> _parameters;
        public GameObject(int x, int y, int width, int height)
        {
            Parameters = new Dictionary<string, string>();
            this.InitBound = new Rectangle(x, y, width, height);
            this.ActiveBound = new Rectangle(x, y, width, height);
        }

        protected void OnPropertyChanged(PropertyChangedEventArgs e)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged(this, e);
            }
        }
        public event PropertyChangedEventHandler PropertyChanged;

        // Danh sách các đối số dùng để khởi tạo đối tượng, bao gồm cả Position.
        [Editor(typeof(GenericDictionaryEditor<string, string>), typeof(UITypeEditor))]
        [GenericDictionaryEditor(Title = "Parameters", KeyDisplayName = "Name", KeyDefaultProviderType = typeof(KeyDefault), ValueDefaultProviderType = typeof(ValueDefault), ValueDisplayName = "Value")]
        public Dictionary<string, string> Parameters
        {
            get { return _parameters; }
            set { _parameters = value; }
        }

        // Bound dùng để tạo quad tree. Không dùng Bound.Location làm position để khởi tạo đối tượng trong game.
        public Rectangle ActiveBound
        {
            get
            {
                return this._activebound;
            }
            set
            {
                //if (value.Contains(InitBound) == false && value != Rectangle.Empty)
                //    return;
                _activebound = value;
                _initBound = value;
                OnPropertyChanged(new PropertyChangedEventArgs("ActiveBound"));
            }
        }

        public Rectangle InitBound
        {
            get { return _initBound; }
            set
            {
                if (_initBound == value)
                    return;
                _initBound = value;
                _activebound = value;
                //if (ActiveBound != Rectangle.Empty)
                //{
                //    if (ActiveBound.Contains(_initBound) == false)
                //    {
                //        if (ActiveBound.X > _initBound.X)
                //            _initBound.X = ActiveBound.X;
                //        if (ActiveBound.Y > _initBound.Y)
                //            _initBound.Y = ActiveBound.Y;
                //        if (ActiveBound.Right < _initBound.Right)
                //            _initBound.Width = ActiveBound.Right - _initBound.X;
                //        if (ActiveBound.Bottom < _initBound.Bottom)
                //            _initBound.Height = ActiveBound.Bottom - _initBound.Y;
                //    }
                //}
                OnPropertyChanged(new PropertyChangedEventArgs("InitBound"));
            }
        }

        // set Activebound bỏ qua các điều kiện. Unsafe
        public void setActiveBound(Rectangle rect)
        {
            _activebound = rect;
        }
        
        // set InitBound bỏ qua các điều kiện. Unsafe
        public void setInitBound(Rectangle rect)
        {
            _initBound = rect;
        }

        public Image Image
        {
            get { return _image; }
            set
            {
                _image = value;
                OnPropertyChanged(new PropertyChangedEventArgs("Image"));
            }
        }

        // Id ứng với Id trong game.
        public int Id
        {
            get { return id; }
            set { id = value; }
        }

        // Dùng để hiển thị tên trên listbox, dont care trong game.
        public string Name
        {
            get { return name; }
            set { name = value;
            OnPropertyChanged(new PropertyChangedEventArgs("Name"));
            }
        }

        public Rectangle GetInitBoundTransform(int worldheight)
        {
            return new Rectangle(
                this.InitBound.X,
                worldheight - this.InitBound.Y,
                this.InitBound.Width,
                this.InitBound.Height);
        }

        internal Rectangle GetActiveBoundTransform(int worldheight)
        {
            return new Rectangle(
                this.ActiveBound.X,
                worldheight - this.ActiveBound.Y,
                this.ActiveBound.Width,
                this.ActiveBound.Height);
        }
    }

    public class KeyDefault : DefaultProvider<string>
    {
        public override string GetDefault(DefaultUsage usage)
        {
            return "params";
        }
    }
    public class ValueDefault : DefaultProvider<string>
    {
        public override string GetDefault(DefaultUsage usage)
        {
            return "0";
        }
    }
}
