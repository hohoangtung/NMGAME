using MapEditor.Tiles;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml.Serialization;

namespace MapEditor
{

    public partial class TestForm : Form
    {
        public TestForm()
        {
            InitializeComponent();
            TilesMap tilemaps = new TilesMap(3, 3);
            tilemaps[1, 2] = 3;
            tilemaps [2, 0] = 5;
            TilesMap.Save(tilemaps, "tilemap.xml");
            tilemaps = TilesMap.Load("tilemap.xml");
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //var config = new Configuration();
            //config.intVal = 1;
            //config.floatVal = 2.3f;
            //config.stringVal = "4.5-6str";
            //this.Save(config, "test.xml");
            //var config = Load<Configuration>("test.xml");
            System.Xml.XmlTextWriter wr = new System.Xml.XmlTextWriter("abc.xml", Encoding.UTF8);
            wr.Formatting = System.Xml.Formatting.Indented; 
            wr.WriteElementString("abc", "xyz");
            
        }


        public T Load<T>(string fileName)
        {
            XmlSerializer xml = new XmlSerializer(typeof(T));
            using (StreamReader sr = new StreamReader(fileName))
            {
                return (T)xml.Deserialize(sr);
            }

        }

    }
    public class Configuration
    {
        public int intVal { get; set; }
        public float floatVal { get; set; }
        public string stringVal { get; set; }


    }

}
