namespace MapEditor
{
    partial class CreateTilesForm
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
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.textBoxWidth = new System.Windows.Forms.TextBox();
            this.textBoxHeight = new System.Windows.Forms.TextBox();
            this.lable_width = new System.Windows.Forms.Label();
            this.label_height = new System.Windows.Forms.Label();
            this.buttonOK = new System.Windows.Forms.Button();
            this.panel_tile = new System.Windows.Forms.Panel();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.SuspendLayout();
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.Location = new System.Drawing.Point(642, 32);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(75, 23);
            this.buttonBrowse.TabIndex = 0;
            this.buttonBrowse.Text = "Browse";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // textBoxWidth
            // 
            this.textBoxWidth.Enabled = false;
            this.textBoxWidth.Location = new System.Drawing.Point(633, 95);
            this.textBoxWidth.Name = "textBoxWidth";
            this.textBoxWidth.Size = new System.Drawing.Size(100, 20);
            this.textBoxWidth.TabIndex = 1;
            this.textBoxWidth.TextChanged += new System.EventHandler(this.textBoxWidth_TextChanged);
            // 
            // textBoxHeight
            // 
            this.textBoxHeight.Enabled = false;
            this.textBoxHeight.Location = new System.Drawing.Point(633, 134);
            this.textBoxHeight.Name = "textBoxHeight";
            this.textBoxHeight.Size = new System.Drawing.Size(100, 20);
            this.textBoxHeight.TabIndex = 2;
            this.textBoxHeight.TextChanged += new System.EventHandler(this.textBoxHeight_TextChanged);
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
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(639, 180);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 5;
            this.buttonOK.Text = "OK";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
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
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog";
            this.openFileDialog.Title = "Chose Image TIles";
            // 
            // CreateTiles
            // 
            this.AcceptButton = this.buttonOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(775, 514);
            this.Controls.Add(this.panel_tile);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.label_height);
            this.Controls.Add(this.lable_width);
            this.Controls.Add(this.textBoxHeight);
            this.Controls.Add(this.textBoxWidth);
            this.Controls.Add(this.buttonBrowse);
            this.Name = "CreateTiles";
            this.Text = "Create Tiles";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonBrowse;
        private System.Windows.Forms.TextBox textBoxWidth;
        private System.Windows.Forms.TextBox textBoxHeight;
        private System.Windows.Forms.Label lable_width;
        private System.Windows.Forms.Label label_height;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Panel panel_tile;
        private System.Windows.Forms.OpenFileDialog openFileDialog;


    }
}

