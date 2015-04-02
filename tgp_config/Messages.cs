using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace tgp_config {
	public static class Messages {

		public static readonly string CfgFileLoadFailed
			= "設定ファイル {0} の読み込み中に、例外 {1} が発生しました。"
			+ "デフォルトの設定が作成されたので、「OK」を押して保存すれば設定ファイルを作成し直します。"
			+ "";

	}
}
