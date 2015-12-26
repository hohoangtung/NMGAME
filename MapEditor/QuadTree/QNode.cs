using MapEditor.Tiles;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Xml;

namespace MapEditor.QuadTree
{

    public class QNode
    {
        #region Private Element
        private int _level;
        private Int64 _id;
        private List<GameObject> _listObject;
        private Rectangle _bound;
        private QNode[] _childs;
        private QNode _parent;
        #endregion

        // 4 node con của một node, nếu là node lá, 4 node con sẽ có giá trị là null.
        public QNode[] Childs
        {
            get { return _childs; }
            set { _childs = value; }
        }

        // Danh sách các đối tượng mà node này quản lý.
        public List<GameObject> ListObject
        {
            get { return _listObject; }
            set { _listObject = value; }
        }

        /// <summary>
        ///  South West (10 giờ rưỡi)
        /// </summary>
        public QNode SW
        {
            get
            {
                return (Childs == null) ? null : Childs[0];
            }
            set
            {
                Childs[0] = value;
            }
        }

        /// <summary>
        /// South East (1 giờ rưỡi)
        /// </summary>
        public QNode SE
        {
            get
            {
                return (Childs == null) ? null : Childs[1];
            }
            set
            {
                Childs[1] = value;
            }
        }

        /// <summary>
        /// North East (4 giờ rưỡi)
        /// </summary>
        public QNode NE
        {
            get
            {
                return (Childs == null) ? null : Childs[2];
            }
            set
            {
                Childs[2] = value;
            }
        }

        /// <summary>
        /// North West (7 giờ rưỡi)
        /// </summary>
        public QNode NW
        {
            get
            {
                return (Childs == null) ? null : Childs[3];
            }
            set
            {
                Childs[3] = value;
            }
        }

        // Bậc của node, nếu là node gốc sẽ có level là 0
        public int Level
        {
            get { return _level; }
            set { _level = value; }
        }

        // Định danh của node. sử dụng quy tắc dịch bit để tại id.
        public Int64 Id
        {
            get { return _id; }
            set { _id = value; }
        }

        // Khung hình chữ nhật của node.
        public Rectangle Bound
        {
            get { return _bound; }
            set { _bound = value; }
        }

        public QNode(int level, Rectangle bound, QNode parent)
        {
            this._level = level;
            this._bound = bound;
            this._parent = parent;
            this.Id = (parent == null) ? 0: parent.Id;
            _listObject = new List<GameObject>();
            _childs = new QNode[4];
        }

        // IMPORTANT 
        // Tạo node con từ các object vào bound. 
        public void initChild()
        {
            // Chỉ tạo node con khi thoả hai điều kiện là số bậc không quá cao, và sô object không quá ít.
            if (this.Level >= MainForm.Settings.MaxLevelQuadTree || this.ListObject.Count <= MainForm.Settings.MaxObjectQuadTree)
                return;

            // Tính 4 hình chữ nhật con của node này
            Rectangle[] rects = this.devideBound();

            // Khởi tạo đối tương childs
            for (int i = 0; i < 4; i++)
            {
                this.Childs[i] = new QNode(this.Level + 1, rects[i], this);
            }

            // Tính id cho node. nhưng gần như tôi không dùng đến id. Có thể dont care.
            QNode.GenerateIds(this);

            for (int i = 0; i < 4; i++)
            {
                foreach (GameObject gameobj in this.ListObject)
                {
                    // done = true nếu gameobject nằm trong child[i]
                    bool done = this.Childs[i].insertObject(gameobj);
                }
            }

            // Node không phải lá không được giữ lại object.
            this.removeall();

            // Lặp lại quá trình trên cho tát cả childs của nó
            for (int i = 0; i < 4; i++)
            {
                Childs[i].initChild();
            }
        }

        // Helper -> dont care
        public static void GenerateIds(QNode parent)
        {
            parent.SW.Id = parent.Id;
            int offset = 2 << parent.Level;
            parent.SE.Id = Convert.ToInt64(1 << offset)| parent.Id;
            parent.NE.Id = Convert.ToInt64(1 << (offset + 1)) | parent.Id;
            parent.NW.Id = parent.SE.Id | parent.NE.Id;
        }

        // Loại bỏ object khởi node này, nếu nó không phải là node lá (không có object nào cả) thì tìm trong tất cả con nó.
        public void removeObject(GameObject obj)
        {
            if (this.isLeaf() == true)
            {
                this.ListObject.Remove(obj);
            }
            else
            {
                for (int i = 0; i < 4; i++)
                {
                    Childs[i].removeObject(obj);
                }
            }
        }

        // Chèn một object vào node này.
        public bool insertObject(GameObject obj)
        {
            // Một số xử lý liên quan đến UI.
            //-------------------------------------------------------
            Rectangle activebound;
            if (MainForm.Settings.UseTransform == true)
            {
                activebound = obj.GetActiveBoundTransform(MapController.MapSize.Height);
            }
            else
            {
                activebound = obj.ActiveBound;
            }
            //-------------------------------------------------------
            // Return true đến đối tượng nằm gọn trong node, bên ngoài nếu thấy true sẽ không insert vào các node khác nữa. Để tăng hiệu suất.
            if (this.Bound.Contains(activebound))
            {
                this.ListObject.Add(obj);
                return true;
            }
            else if (this.Bound.IntersectsWith(activebound))
            {
                this.ListObject.Add(obj);
                return false;
            }
            return false;
        }

        public void removeall()
        {
            this.ListObject.Clear();
        }

        // Chia bound của node hiện tại thành 4 node con, bằng cách chia đôi 2 cạnh của hcn.
        public Rectangle[] devideBound()
        {
            Rectangle[] result = new Rectangle[4];

            // Đây là lí do tại sao chúng ta có cây tứ phân mà không phải cây lục phân hay bát phân.
            // ĐỐi với cây tứ phân, để tính được giá trị bound mới, ta cần chia đôi cạnh của hcn. Phép chia 2 thể hiện bằng phép dịch bit sẽ tăng performance
            // Giả sử nếu dùng cây sáu nhánh, sẽ có một cạnh ta phải chia 3, phép chia cho 3 không thể biểu diễn bằng phép dịch bit

            // X >> 1 tức là X chia 2 (hai bằng hai mũ một).
            int haftwidth = this.Bound.Width >> 1;
            int haftheight = this.Bound.Height >> 1;

            // SW
            result[0] = new Rectangle()
            {
                X = this.Bound.X,
                Y = this.Bound.Y,
                Width = haftwidth,
                Height = haftheight,
            };

            // SE
            result[1] = new Rectangle()
            {
                X = this.Bound.X + haftwidth,
                Y = this.Bound.Y,
                Width = haftwidth,
                Height = haftheight,
            };

            // NE
            result[2] = new Rectangle()
            {
                X = this.Bound.X + haftwidth,
                Y = this.Bound.Y + haftheight,
                Width = haftwidth,
                Height = haftheight,
            };

            // WN
            result[3] = new Rectangle()
            {
                X = this.Bound.X,
                Y = this.Bound.Y + haftheight,
                Width = haftwidth,
                Height = haftheight,
            };
            return result;
        }
        
        // Kiểm tra bound của nođe này có giao nhau không.
        public bool isIntersect(Rectangle rect)
        {
            return this.Bound.IntersectsWith(rect);
        }

        // Kiểm tra bound của node này có chứa hình chữ nhật rect không.
        public bool isContains(Rectangle rect)
        {
            return this.Bound.Contains(rect);
        }

        // Return true nếu là node lá, return fale nếu ngược lại.
        public bool isLeaf()
        {
            if (this.Childs[0] == null)
                return true;
            return false;
        }

        // Lấy ra danh sách các đối tượng mà node của nó có giao với hình chữ nhật rect, thông thường rect là khung màn hình.
        // PHIÊN BẢN LỖI. (lỗi performance)
        public IList<GameObject> getListObject(Rectangle rect)
        {
            List<GameObject> rs = new List<GameObject>(); // =>  anti-performance => WHY?
            if (this.isContains(rect) || this.isIntersect(rect) || rect.IntersectsWith(this.Bound))
            {
                if (this.isLeaf() == true)
                {
                    rs.AddRange(this.ListObject);
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        rs.AddRange(_childs[i].getListObject(rect));
                    }
                }
            }
            return rs;
        }

        /*
         *  Phiên bản thứ hai này có một sự khác biệt lớn.
         *  thay vì, ta khai báo một đối tượng list cục bộ như trên, ở đây ta cho một tham số return_listobject để thêm đối tượng vào đó.
         *  Sở dĩ phiên bản ở trên performance thấp hơn vì mỗi lần gọi đệ quy, đối tượng rs được tạo mới một lần,
         *  và khi return đối tương đó bị huỷ đi, và copy lại vào giá trị nhận return
         *  quá trình tạo, huỷ copy liên tục trong vòng đệ quy gây giảm hiệu suất.
         */

        public void getListObject(ref List<GameObject> return_listobject, Rectangle rect)
        {
            //List<GameObject> rs = new List<GameObject>(); 
            // Thật ra đoạn này tôi làm thừa, vì chỉ cần kiểm tra intersect thôi, contain là một trường hợp của intersect
            if (this.isContains(rect) || this.isIntersect(rect) || rect.IntersectsWith(this.Bound))
            {
                // Nếu là node lá thì tiến hành kiểm tra chèn object, nếu không phải thì gọi con nó.
                if (this.isLeaf() == true)
                {
                    // Nếu return_listobject có đối tượng thì tiến hành kiểm tra từng phần tử xem có trùng không, không trùng thi chèn vào.
                    // Nếu return_listobject không có đối tượng nào thi chèn cả listobject vào.
                    if (return_listobject.Any() == true)
                    {
                        foreach (var obj in this.ListObject)
                        {
                            if (return_listobject.Contains(obj) == false)
                            {
                                return_listobject.Add(obj);
                            }
                        }                       
                    }
                    else
                    {
                        return_listobject.AddRange(this.ListObject);
                    }
                }
                else
                {
                    for (int i = 0; i < 4; i++)
                    {
                        _childs[i].getListObject(ref return_listobject, rect);
                    }
                }
            }
        }
    }

}