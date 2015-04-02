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
using TgpLib;

namespace tgp_config {
	public partial class DialogForm : Form {
		class NamedValue<T> {
			public T Value { get; set; }
			public string DisplayString { get; set; }

			public NamedValue(T val, string disp) {
				Value = val;
				DisplayString = disp;
			}

			public NamedValue(T val)
				: this(val, val.ToString()) {
			}

			public override string ToString() {
				return DisplayString;
			}

		}
		static readonly string ConfigFileName="tgp.cfg";
		Config config = new Config();

		public DialogForm() {
			InitializeComponent();
		}
		
		private void DialogForm_Load(object sender, EventArgs e) {
			try {
				config.Load(ConfigFileName);
			}
			catch (FileNotFoundException) {
				// 何もしない
			}
			catch (IOException ex) {
				MessageBox.Show(string.Format(Messages.CfgFileLoadFailed, ConfigFileName, ex.GetType().FullName));
			}
			catch (ApplicationException ex) {
				MessageBox.Show(string.Format(Messages.CfgFileLoadFailed, ConfigFileName, ex.GetType().FullName));
			}
		}

	}
}
