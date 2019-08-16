using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace file_searcher
{
	enum CommandLineCode
	{
		Error=0,
		Success=1,
	}


	public class Directory
	{
		static string[] _ignore = { "\\.git", "\\.svn", "\\.vs", "\\.vscode", "\\__pycache__", "\\.hg", "\\.bzr", };
		static bool _show_hidden_file = false;


		public static void ApplyArguments(Arguments args)
		{
			_show_hidden_file = args._show_hidden_file;
		}

		static bool Ignore(string folder)
		{
			foreach (var pattern in _ignore)
			{
				if (folder.EndsWith(pattern, StringComparison.InvariantCultureIgnoreCase))
				{
					return true;
				}
			}
			return false;
		}

		static bool UseThisFolder(string folder) {
			if (Ignore(folder))
			{
				return false;
			}
			var file_attributes = new System.IO.FileInfo(folder).Attributes;
			if ((file_attributes & System.IO.FileAttributes.Hidden) == System.IO.FileAttributes.Hidden) {
				return false;
			}
			return true;
		}

		public static IEnumerable<string> SafeEnumerateFilesInAllDirectories(string path)
		{
			return SafeEnumerateFilesInAllDirectories(path, "*");
		}

		public static IEnumerable<string> SafeEnumerateFilesInAllDirectories(string path, string searchPattern)
		{
			string[] sub_dirs=null;
			try
			{
				/*if(!UseThisFolder(path)) {
					yield break;
				}*/
				sub_dirs = System.IO.Directory.GetDirectories(path, searchPattern, System.IO.SearchOption.TopDirectoryOnly);
			}
			catch (System.UnauthorizedAccessException)
			{
				//pass
			}

			if (sub_dirs != null)
			{
				foreach (var dir in sub_dirs)
				{
					if(!UseThisFolder(dir)){
						continue;
					}
					yield return dir;
				}

				foreach (var dir in sub_dirs)
				{
					if (!UseThisFolder(dir))
					{
						continue;
					}
					foreach (var o in SafeEnumerateFilesInAllDirectories(dir, searchPattern))
					{
						yield return o;
					}
				}

			}
		}
	}


	public class Arguments
	{
		public bool Parse(string[] args)
		{
			/*
			 * Usage: file_sercher.exe output_filename type current_working_directory search_path search_pattern 
			 */
			_output_filename = args[0];
			switch (args[1])
			{
				case "directory":
					_type = TraverseType.Directory;
					break;
				case "file":
					_type = TraverseType.File;
					break;
				default:
					Console.WriteLine("Invalid argument.");
					return false;
			}
			_current_working_directory = args[2];
			_search_path = args[3];
			_search_pattern = args[4];
			return true;
		}
		public enum TraverseType{
			Directory,
			File,
		}

		public string _output_filename="";
		public TraverseType _type=TraverseType.Directory;
		public string _current_working_directory="";
		public string _search_path="";
		public string _search_pattern = "";
		public bool _show_hidden_file=false;
	}

	class Application
	{
		public CommandLineCode Exec(string[] args)
		{
			if(!_arguments.Parse(args))
			{
				return CommandLineCode.Error;
			}
			switch (_arguments._type)
			{
				case Arguments.TraverseType.Directory:
					return TraverseDirectory();
				case Arguments.TraverseType.File:
					return TraverseFile();
				default:
					//pass
					break;
			}
			return CommandLineCode.Error;
		}

		CommandLineCode TraverseDirectory()
		{
			System.IO.Directory.SetCurrentDirectory(_arguments._current_working_directory);

			var writer = new System.IO.StreamWriter(_arguments._output_filename, false, Encoding.UTF8);
			//IEnumerable<string> subFolders = System.IO.Directory.EnumerateDirectories(_arguments._search_path, _arguments._search_pattern, System.IO.SearchOption.AllDirectories);
			var subFolders = Directory.SafeEnumerateFilesInAllDirectories(_arguments._search_path, _arguments._search_pattern);
			//サブフォルダを列挙する
			foreach (string subFolder in subFolders)
			{
				writer.WriteLine(FormatString(subFolder));
			}

			writer.Close();
			return CommandLineCode.Success;
		}

		string FormatString(string folder)
		{
			if (folder.StartsWith(".\\"))
			{
				//ファイル先頭の.\を取り除く
				folder=folder.Substring(2);
			}
			if (!folder.EndsWith(System.IO.Path.DirectorySeparatorChar.ToString()))
			{
				//\で終端する
				folder = folder+ System.IO.Path.DirectorySeparatorChar;
			}
			return folder;
		}

		CommandLineCode TraverseFile()
		{
			return CommandLineCode.Success;
		}
		Arguments _arguments=new Arguments();
	}





	class Program
	{
		static int Main(string[] args)
		{
			var app = new Application();
			return (int)app.Exec(args);
		}
	}
}
