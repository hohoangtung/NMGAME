using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor.Tiles
{

    public class  Tile : INotifyPropertyChanged
    {
        private int _id;
        private string _name;
        private Rectangle _srcRect;

        public Rectangle SrcRect
        {
            get { return _srcRect; }
            set { setProperty(ref _srcRect, value, "SrcRect"); }
        }

        Image _image;               // ảnh gồm nhiều tile. khi vẽ cắt bằng srcRect như Sprite

        public int Id
        {
            get { return _id; }
            set { setProperty(ref _id, value, "Id"); }
        }
        public string Name
        {
            get { return _name;}
            set { setProperty(ref _name, value, "Name"); }
        }
        public Image Image
        {
            get { return _image; }
            set { setProperty(ref _image, value, "Image"); }
        }
        // trả về ảnh đã cắt bằng srcRect
        public Bitmap getbitmap()
        {
            return ((_image as Bitmap).Clone(SrcRect, _image.PixelFormat));
        }
        public Tile(Image image, Rectangle srcRect, int id)
        {
            Id = id;
            SrcRect = srcRect;
            _image = image;
            Name = "tile_" + id.ToString();
            
        }
        public void draw(Graphics graphics, Point position, Size size)
        {
            graphics.DrawImage(_image, new Rectangle(position, size), SrcRect, GraphicsUnit.Pixel);
        }

        public event PropertyChangedEventHandler PropertyChanged;
        private void setProperty<T>(ref T obj, T value, string propertyName)
        {
            if (object.Equals(obj, value))
                return;
            obj = value;
            if (PropertyChanged != null)
                PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
