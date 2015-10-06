namespace MapEditor
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.button_browse = new System.Windows.Forms.Button();
            this.textBox_width = new System.Windows.Forms.TextBox();
            this.textBox_height = new System.Windows.Forms.TextBox();
            this.lable_width = new System.Windows.Forms.Label();
            this.label_height = new System.Windows.Forms.Label();
            this.button_OK = new System.Windows.Forms.Button();
            this.panel_tile = new System.Windows.Forms.Panel();
            this.SuspendLayout();
            // 
            // button_browse
            // 
            this.button_browse.Location = new System.Drawing.Point(639, 27);
            this.button_browse.Name = "button_browse";
            this.button_browse.Size = new System.Drawing.Size(75, 23);
            this.button_browse.TabIndex = 0;
            this.button_browse.Text = "Browse";
            this.button_browse.UseVisualStyleBackColor = true;
            this.button_browse.Click += new System.EventHandler(this.button_browse_Click);
            // 
            // textBox_width
            // 
            this.textBox_width.Location = new System.Drawing.Point(633, 95);
            this.textBox_width.Name = "textBox_width";
            this.textBox_width.Size = new System.Drawing.Size(100, 20);
            this.textBox_width.TabIndex = 1;
            this.textBox_width.TextChanged += new System.EventHandler(this.textBox_width_TextChanged);
            // 
            // textBox_height
            // 
            this.textBox_height.Location = new System.Drawing.Point(633, 134);
            this.textBox_height.Name = "textBox_height";
            this.textBox_height.Size = new System.Drawing.Size(100, 20);
            this.textBox_height.TabIndex = 2;
            this.textBox_height.TextChanged += new System.EventHandler(this.textBox_height_TextChanged);
            // 
            // lable_width
            // 
            this.lable_width.AutoSize = true;
            this.lable_width.Location = new System.Drawing.Point(655, 79);
            this.lable_width.Name = "lable_width";
            this.lable_width.Size = new System.Drawing.Size(62, 13);
            this.lable_width.TabIndex = 3;
            this.lable_width.Text = "width (pixel)";
            // 
            // label_height
            // 
            this.label_height.AutoSize = true;
            this.label_height.Location = new System.Drawing.Point(651, 118);
            this.label_height.Name = "label_height";
            this.label_height.Size = new System.Drawing.Size(63, 13);
            this.label_height.TabIndex = 4;
            this.label_height.Text = "height(pixel)";
            // 
            // button_OK
            // 
            this.button_OK.Location = new System.Drawing.Point(639, 180);
            this.button_OK.Name = "button_OK";
            this.button_OK.Size = new System.Drawing.Size(75, 23);
            this.button_OK.TabIndex = 5;
            this.button_OK.Text = "OK";
            this.button_OK.UseVisualStyleBackColor = true;
            this.button_OK.Click += new System.EventHandler(this.button_OK_Click);
            // 
            // panel_tile
            // 
            this.panel_tile.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.panel_tile.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel_tile.Dock = System.Windows.Forms.DockStyle.Left;
            this.panel_tile.Location = new System.Drawing.Point(0, 0);
            this.panel_tile.Name = "panel_tile";
            this.panel_tile.Size = new System.Drawing.Size(490, 514);
            this.panel_tile.TabIndex = 6;
            this.panel_tile.Paint += new System.Windows.Forms.PaintEventHandler(this.panel_tile_Paint);
            // 
            // Form1
            // 
            this.AcceptButton = this.button_OK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(775, 514);
            this.Controls.Add(this.panel_tile);
            this.Controls.Add(this.button_OK);
            this.Controls.Add(this.label_height);
            this.Controls.Add(this.lable_width);
            this.Controls.Add(this.textBox_height);
            this.Controls.Add(this.textBox_width);
            this.Controls.Add(this.button_browse);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button_browse;
        private System.Windows.Forms.TextBox textBox_width;
        private System.Windows.Forms.TextBox textBox_height;
        private System.Windows.Forms.Label lable_width;
        private System.Windows.Forms.Label label_height;
        private System.Windows.Forms.Button button_OK;
        private System.Windows.Forms.Panel panel_tile;


    }
}

