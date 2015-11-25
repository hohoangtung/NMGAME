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
    public partial class AppSettingsForm : Form
    {
        public AppSettingsForm(ApplicationSettings appsetting)
        {
            InitializeComponent();
            this.AppSettingValue.SelectedObject = appsetting;

        }

        private void propertyGrid1_Click(object sender, EventArgs e)
        {
            
        }

        private void AppSettingsForm_Load(object sender, EventArgs e)
        {

        }

        private void cancelbtn_Click(object sender, EventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.Close();
        }

        private void okbtn_Click(object sender, EventArgs e)
        {
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}
