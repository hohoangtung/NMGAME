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
    public partial class NewMaps : Form
    {
        private int _columns;
        private int _rows;

        public int Columns
        {
            get { return _columns; }
            set { _columns = value; }
        }

        public int Rows
        {
            get { return _rows; }
            set { _rows = value; }
        }

        public NewMaps()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (Int32.TryParse(this.textBox1.Text, out _columns) == false)
                return;
            if (Int32.TryParse(this.textBox2.Text, out _rows) == false)
                return;
            this.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.Close();
        }
    }
}
