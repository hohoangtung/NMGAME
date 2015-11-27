using System;
using System.Collections.Generic;
using System.Configuration;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
    public class ApplicationSettings : ApplicationSettingsBase
    {

        [UserScopedSetting()]
        [DefaultSettingValue("50, 50")]
        public Size TileSize
        {
            get
            {
                return ((Size)this["TileSize"]);
            }
            set
            {
                this["TileSize"] = (Size)value;
            }
        }

        [UserScopedSetting()]
        [DefaultSettingValue("false")]
        public bool UseTransform
        {
            get
            {
                return ((bool)this["UseTransform"]);
            }
            set
            {
                this["UseTransform"] = (bool)value;
            }
        }

        [UserScopedSetting()]
        [DefaultSettingValue("3")]
        public int MaxObjectQuadTree
        {
            get
            {
                return ((int)this["MaxObjectQuadTree"]);
            }
            set
            {
                this["MaxObjectQuadTree"] = (int)value;
            }
        }

        [UserScopedSetting()]
        [DefaultSettingValue("32")]
        public int MaxLevelQuadTree
        {
            get
            {
                return ((int)this["MaxLevelQuadTree"]);
            }
            set
            {
                this["MaxLevelQuadTree"] = (int)value;
            }
        }
    }
}
