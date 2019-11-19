using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace file_searcher
{
	class FileTraverser
	{
		static bool _show_hidden_file = false;

		public class Result
		{
			public Result(string rel,string abs)
			{
				rel_filename_ = rel;
				abs_filename_ = abs;
			}
			public string rel_filename_;
			public string abs_filename_;
		}

		

		/// <summary>
		/// 絶対パスから相対パスを取得します。
		/// </summary>
		/// <param name="basePath">基準とするフォルダのパス。</param>
		/// <param name="absolutePath">相対パス。</param>
		/// <returns>絶対パス。</returns>
		public static string GetRelativePath(string basePath, string absolutePath)
		{
			var sb = new StringBuilder(260);
			bool res = DllImport.PathRelativePathTo(sb,
				basePath, System.IO.FileAttributes.Directory,
				absolutePath, System.IO.FileAttributes.Normal);
			if (!res)
			{
				throw new Exception("相対パスの取得に失敗しました。");
			}
			return sb.ToString();
		}


		public static void ApplyArguments(Options args)
		{
			_show_hidden_file = args._show_hidden_files;
		}

		public static bool IgnoreFile(string filename)
		{
			foreach (var pattern in Ignore._files)
			{
				if (filename.ToLower().EndsWith(pattern, StringComparison.InvariantCultureIgnoreCase))
				{
					return true;
				}
			}
			return false;
		}

		static bool UseThisFile(string file, System.IO.FileAttributes file_attributes)
		{
			if (IgnoreFile(file))
			{
				return false;
			}
			if (_show_hidden_file)
			{
				return true;
			}

			if ((file_attributes & System.IO.FileAttributes.Hidden) == System.IO.FileAttributes.Hidden)
			{
				return false;
			}
			return true;
		}


		public static IEnumerable<Result> SafeEnumerateFilesInAllFiles(string path)
		{
			return SafeEnumerateFilesInAllFiles(path, "*");
		}

		public static IEnumerable<Result> SafeEnumerateFilesInAllFiles(string path, string searchPattern)
		{
			var abs_root_path = System.IO.Path.GetFullPath(path);
			return SafeEnumerateFilesInAllFilesMain(path, abs_root_path, searchPattern);
		}

		static IEnumerable<Result> SafeEnumerateFilesInAllFilesMain(string path, string abs_root_path, string searchPattern)
		{
			{
				IEnumerable<FileInfo> files = null;				

				try
				{//ファイルを返す
					var di = new System.IO.DirectoryInfo(System.IO.Path.GetFullPath(path));
					files = di.EnumerateFiles(searchPattern, System.IO.SearchOption.TopDirectoryOnly);
				}
				catch (System.UnauthorizedAccessException)
				{
					//pass
				}

				if (files != null)
				{
					foreach (var f in files)
					{
						if (UseThisFile(f.FullName, f.Attributes))
						{
							yield return new Result(GetRelativePath(abs_root_path, f.FullName), f.FullName);
						}
					}
				}
			}

			string[] sub_dirs = null;
			try
			{
				sub_dirs = System.IO.Directory.GetDirectories(path, "*", System.IO.SearchOption.TopDirectoryOnly);
			}
			catch (System.UnauthorizedAccessException)
			{
				//pass
			}

			if (sub_dirs != null)
			{
				foreach (var dir in sub_dirs)
				{
					if (!DirectoryTraverser.UseThisFolder(dir))
					{
						continue;
					}
					foreach (var o in SafeEnumerateFilesInAllFilesMain(dir, abs_root_path, searchPattern))
					{
						yield return o;
					}
				}
			}
		}
	}
}
