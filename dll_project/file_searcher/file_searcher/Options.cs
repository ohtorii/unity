using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommandLine;



namespace file_searcher
{
	public class Options
	{
		// 基本的な形式
		[Option('o', "output", Required = true, HelpText = "Output filename")]
		public string _output_filename { get; set; }

		// enumを受け取ることも可能(指定にはenumの名前を指定する)
		[Option('m', "mode", Required = true, HelpText = "Path mode")]
		public PathMode _path_mode { get; set; }

		[Option('w', "workind_directory", Required = false, HelpText = "Working directory", Default = ".")]
		public string _working_directory { get; set; }

		[Option('p', "path", Required = false, HelpText = "Search path", Default = ".")]
		public string _search_path { get; set; }

		[Option('n', "pattern", Required = false, HelpText = "Search pattern", Default = "*")]
		public string _search_pattern { get; set; }

		[Option('h', "show_hidden_files", Required = false, HelpText = "Show hidden files", Default = false)]
		public bool _show_hidden_files { get; set; }

	}

}
