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

        public void Init()
        {
            var resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));

            this.Name = "toolbar";
            this.ImageList = new ImageList();
            this.ImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("defaultImage.ImageStream")));

            this.Buttons.Add(initEditStateButton("editstate"));
            this.Buttons.Add(initAppSettingsButton("appsettings"));

            this.ButtonClick += (object sender, ToolBarButtonClickEventArgs e) =>
            {
                if (e.Button.Name == "editstate")
                {
                    buttonEditStateHandleClick(e);
                }
                else if (e.Button.Name == "appsettings")
                {
                    if (buttonAppSettingsHandleClick(e))
                    {
                        var mainform = (sender as ToolBar).FindForm() as MainForm;
                        mainform.InitTableLayout();
                        mainform.ReDrawMap();
                    }
                }

            };
        }


        // Hàm xử lý sự kiện nút appsetting click
        // @return: true nếu form bấm nút ok
        //          false nếu form bấm nút cancel.
        private bool buttonAppSettingsHandleClick(ToolBarButtonClickEventArgs e)
        {
            AppSettingsForm form = new AppSettingsForm(MainForm.Settings);
            DialogResult rs = form.ShowDialog();
            if (rs == DialogResult.Cancel)
            {
                MainForm.Settings.Reload();
                return false;
            }
            else if (rs == DialogResult.OK)
            {
                MainForm.Settings.Save();
                return true;
            }
            else
            {
                MainForm.Settings.Reload();
                return false;
            }
        }

        private ToolBarButton initAppSettingsButton(string name)
        {
            ToolBarButton appsettingsbtn = new ToolBarButton();
            appsettingsbtn.Name = name;
            appsettingsbtn.ImageIndex = 2;

            return appsettingsbtn;
        }

        private void buttonEditStateHandleClick(ToolBarButtonClickEventArgs e)
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

        private ToolBarButton initEditStateButton(string name )
        {
            ToolBarButton editstatebtn = new ToolBarButton();
            editstatebtn.Name = name;
            editstatebtn.Style = ToolBarButtonStyle.ToggleButton;
            editstatebtn.Pushed = false;
            editstatebtn.ImageIndex = 1;

            return editstatebtn;
        }

    }
}
