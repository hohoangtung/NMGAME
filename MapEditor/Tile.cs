using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{

    class Tile
    {
        private int _id;
        
        private Rectangle _srcRect;

        Image _image;               // ảnh gồm nhiều tile. khi vẽ cắt bằng srcRect như Sprite

        public int Id
        {
            get { return _id; }
            set { _id = value; }
        }
        // trả về ảnh đã cắt bằng srcRect
        public Bitmap getbitmap()
        {
            return ((_image as Bitmap).Clone(_srcRect, _image.PixelFormat));
        }
        public Tile(Image image, Rectangle srcRect, int id)
        {
            _id = id;
            _srcRect = srcRect;
            _image = image;
        }
        public void draw(Graphics graphics, Point position, Size size)
        {
            graphics.DrawImage(_image, new Rectangle(position, size), _srcRect, GraphicsUnit.Pixel);
        }
    }
}
