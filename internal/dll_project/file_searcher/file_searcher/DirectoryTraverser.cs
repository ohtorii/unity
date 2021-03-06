﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace file_searcher
{
	public class DirectoryTraverser
	{
		static bool _show_hidden_file = false;


		public static void ApplyArguments(Options args)
		{
			_show_hidden_file = args._show_hidden_files;
		}

		public static bool IgnoreFolder(string folder)
		{
			foreach (var pattern in Ignore._directorys)
			{
				if (folder.ToLower().EndsWith(pattern, StringComparison.InvariantCultureIgnoreCase))
				{
					return true;
				}
			}
			return false;
		}

		public static bool UseThisFolder(string folder)
		{
			if (IgnoreFolder(folder))
			{
				return false;
			}
			if (_show_hidden_file)
			{
				return true;
			}

			var file_attributes = new System.IO.FileInfo(folder).Attributes;
			if ((file_attributes & System.IO.FileAttributes.Hidden) == System.IO.FileAttributes.Hidden)
			{
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
			string[] sub_dirs = null;
			try
			{
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
					if (!UseThisFolder(dir))
					{
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

}
