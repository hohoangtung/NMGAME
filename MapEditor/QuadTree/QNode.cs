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

        public QNode[] Childs
        {
            get { return _childs; }
            set { _childs = value; }
        }

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
        /// North West (7 giờ rưỡ)
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

        public int Level
        {
            get { return _level; }
            set { _level = value; }
        }

        public Int64 Id
        {
            get { return _id; }
            set { _id = value; }
        }

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

        public void initChild()
        {
            if (this.Level >= MainForm.Settings.MaxLevelQuadTree || this.ListObject.Count <= MainForm.Settings.MaxObjectQuadTree)
                return;
            var rects = this.devideBound();
            for (int i = 0; i < 4; i++)
            {
                this.Childs[i] = new QNode(this.Level + 1, rects[i], this);
            }
            QNode.GenerateIds(this);

            for (int i = 0; i < 4; i++)
            {
                foreach (GameObject gameobj in this.ListObject)
                {
                    // done = true nếu gameobject nằm trong child[i]
                    bool done = this.Childs[i].insertObject(gameobj);
                }
            }
            // remove những đối tượng nằm ở cả 4 node.
            this.removeall();
            for (int i = 0; i < 4; i++)
            {
                Childs[i].initChild();
            }
        }

        public static void GenerateIds(QNode parent)
        {
            parent.SW.Id = parent.Id;
            int offset = 2 << parent.Level;
            parent.SE.Id = Convert.ToInt64(1 << offset)| parent.Id;
            parent.NE.Id = Convert.ToInt64(1 << (offset + 1)) | parent.Id;
            parent.NW.Id = parent.SE.Id | parent.NE.Id;
        }

        public void removeObject(GameObject obj)
        {
            this.ListObject.Remove(obj);
        }

        public bool insertObject(GameObject obj)
        {
            Rectangle activebound;
            if (MainForm.Settings.UseTransform == true)
            {
                activebound = obj.GetActiveBoundTransform(MapController.MapSize.Height);
            }
            else
            {
                activebound = obj.ActiveBound;
            }
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

        public Rectangle[] devideBound()
        {
            Rectangle[] result = new Rectangle[4];
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
        
        public bool isIntersect(Rectangle rect)
        {
            return this.Bound.IntersectsWith(rect);
        }

        public bool isContains(Rectangle rect)
        {
            return this.Bound.Contains(rect);
        }

        //public static QNode Load(XmlTextReader reader, string path)
        //{
        //    return null;
        //}
        //public static bool Save(XmlTextWriter writter, string path)
        //{
        //    return false;
        //}

        public bool isLeaf()
        {
            if (this.Childs[0] == null)
                return true;
            return false;
        }

        public IList<GameObject> getListObject(Rectangle rect)
        {
            List<GameObject> rs = new List<GameObject>();
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
    }

}