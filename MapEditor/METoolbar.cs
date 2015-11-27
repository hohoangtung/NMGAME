using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace MapEditor
{
    class METoolbar : ToolBar
    {
        public METoolbar()
        {

        }
        public ToolBarButton EditState { get; set; }
        public ToolBarButton AppSettings { get; set; }
        public ToolBarButton Save { get; set; }
        public ToolBarButton SaveAs { get; set; }
        public ToolBarButton Open { get; set; }
        public ToolBarButton QuadTree { get; set; }
        public void Init()
        {
            var resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));

            this.Name = "toolbar";
            this.ImageList = new ImageList();
            this.ImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("defaultImage.ImageStream")));
            this.Appearance = ToolBarAppearance.Flat;

            this.EditState = initEditStateButton("editstate");
            this.AppSettings = initAppSettingsButton("appsettings");
            this.Save = initSaveButton("save");
            this.SaveAs = initSaveAsButton("saveAs");
            this.Open = initOpenButton("open");
            this.QuadTree = initQuadTreeButton("quadtree");

            this.Buttons.Add(EditState);
            this.Buttons.Add(AppSettings);
            this.Buttons.Add(initSeperator("seperator1"));
            this.Buttons.Add(Save);
            this.Buttons.Add(SaveAs);
            this.Buttons.Add(Open);
            this.Buttons.Add(initSeperator("seperator2"));
            this.Buttons.Add(QuadTree);

            this.ButtonClick += (object sender, ToolBarButtonClickEventArgs e) =>
            {
                if (e.Button.Name == "editstate")
                {
                    this.buttonEditStateHandleClick(sender, e);
                }
                else if (e.Button.Name == "appsettings")
                {
                    this.buttonAppSettingsHandleClick(sender, e);
                }
                else if (e.Button.Name == "save")
                {
                    this.buttonSaveHandleClick(sender, e);
                }
                else if (e.Button.Name == "saveAs")
                {
                    this.buttonSaveAsHandleClick(sender, e);
                }
                else if (e.Button.Name == "open")
                {
                    this.buttonOpenHandleClick(sender, e);   
                }
                else if (e.Button.Name == "quadtree")
                {
                    this.buttonQuadtreeHandleClick(sender, e);
                }
            };
        }

        private ToolBarButton initQuadTreeButton(string name)
        {
            ToolBarButton quadtreebtn = new ToolBarButton();
            quadtreebtn.Name = name;
            quadtreebtn.Style = ToolBarButtonStyle.ToggleButton;
            quadtreebtn.Pushed = false;
            quadtreebtn.ImageIndex = 6;

            return quadtreebtn;
        }

        private ToolBarButton initSeperator(string name)
        {
            ToolBarButton seperator = new ToolBarButton();
            seperator.Name = name;
            seperator.Style = ToolBarButtonStyle.Separator;
            return seperator;
        }

        private ToolBarButton initOpenButton(string name)
        {
            ToolBarButton openbtn = new ToolBarButton();
            openbtn.Name = name;
            openbtn.ImageIndex = 5;
            return openbtn;
        }


        // Khởi tạo nút SaveAs
        private ToolBarButton initSaveAsButton(string name)
        {
            ToolBarButton saveasbtn = new ToolBarButton();
            saveasbtn.Name = name;
            saveasbtn.ImageIndex = 4;
            return saveasbtn;
        }

        // Khởi tạo nút Save.
        private ToolBarButton initSaveButton(string name)
        {
            ToolBarButton savebtn = new ToolBarButton();
            savebtn.Name = name;
            savebtn.ImageIndex = 3;
            return savebtn;
        }

        // Khởi tạo nút AppSetting
        // @return ToolBarButton.
        private ToolBarButton initAppSettingsButton(string name)
        {
            ToolBarButton appsettingsbtn = new ToolBarButton();
            appsettingsbtn.Name = name;
            appsettingsbtn.ImageIndex = 2;

            return appsettingsbtn;
        }

        // Khởi tạo nút EditState
        private ToolBarButton initEditStateButton(string name)
        {
            ToolBarButton editstatebtn = new ToolBarButton();
            editstatebtn.Name = name;
            editstatebtn.Style = ToolBarButtonStyle.ToggleButton;
            editstatebtn.Pushed = false;
            editstatebtn.ImageIndex = 1;

            return editstatebtn;
        }


        // Hàm xử lý sự kiện nút appsetting click
        // @return: true nếu form bấm nút ok
        //          false nếu form bấm nút cancel.
        private void buttonAppSettingsHandleClick(object sender, ToolBarButtonClickEventArgs e)
        {
            AppSettingsForm form = new AppSettingsForm(MainForm.Settings);
            DialogResult rs = form.ShowDialog();
            if (rs == DialogResult.Cancel)
            {
                MainForm.Settings.Reload();
            }
            else if (rs == DialogResult.OK)
            {
                MainForm.Settings.Save();
                var mainform = (sender as ToolBar).FindForm() as MainForm;
                mainform.InitTableLayout();
                mainform.ReDrawMap();
            }
            else
            {
                MainForm.Settings.Reload();
            }
        }

        // Hàm xử lý sự kiện nhấn nút EditState
        private void buttonEditStateHandleClick(object sender, ToolBarButtonClickEventArgs e)
        {
            if (e.Button.Pushed)
            {
                e.Button.ToolTipText = "Object";
                e.Button.ImageIndex = 0;
            }
            else
            {
                e.Button.ToolTipText = "Map";
                e.Button.ImageIndex = 1;
            }
        }

        // Hàm xử lý sự kiện nhấn nút Open
        private void buttonOpenHandleClick(object sender, ToolBarButtonClickEventArgs e)
        {
            var mainform = (sender as ToolBar).FindForm() as MainForm;
            mainform.LoadMap();
        }

        // Hàm xử lý sự kiện nhấn nút Save As
        private void buttonSaveAsHandleClick(object sender, ToolBarButtonClickEventArgs e)
        {
            var mainform = (sender as ToolBar).FindForm() as MainForm;
            mainform.SaveAs();
        }

        // Hàm xử lý sự kiện nhấn nút Save As
        private void buttonSaveHandleClick(object sender, ToolBarButtonClickEventArgs e)
        {
            var mainform = (sender as ToolBar).FindForm() as MainForm;
            mainform.Save();
        }

        private void buttonQuadtreeHandleClick(object sender, ToolBarButtonClickEventArgs e)
        {
            var mainform = (sender as ToolBar).FindForm() as MainForm;
            if (e.Button.Pushed)
            {
                mainform.DrawQuadTree();
            }
            else
            {
                mainform.ReDrawMap();
            }
        }

    }
}
