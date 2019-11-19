using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace file_searcher
{
	class Ignore
	{
		/// <summary>
		/// 除外ディレクトリ、すべて小文字で指定すること。
		/// </summary>
		public readonly static string[] _directorys = {
			"\\.git",
			"\\.svn",
			"\\.vs",
			"\\.vscode",
			"\\__pycache__",
			"\\.hg",
			"\\.bzr", };

		/// <summary>
		/// 除外ファイル、すべて小文字で指定すること。
		/// </summary>
		public readonly static string[] _files = {
			"DS_Store",
			".pyc",
			".ipch",
			".db",
			".opendb",
			".obj",
			".tlog",
			".pch",
			".pdb",
			".exp",
			".iobj",
			".ipdb",
			".lib",
			".log",
			".db-shm",
			".db-wal",
			".lastcodeanalysissucceeded",
			".ilk",
			".codeanalysis",
			".codeanalysisast",
			"tags","ctags","GPATH","GRTAGS","GTAGS",
		};
	}
}
