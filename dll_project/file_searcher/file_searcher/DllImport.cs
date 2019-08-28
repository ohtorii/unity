using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;


namespace file_searcher
{
	class DllImport
	{
		[DllImport("shlwapi.dll", CharSet = CharSet.Auto)]
		public static extern bool PathRelativePathTo(
			 [Out] StringBuilder pszPath,
			 [In] string pszFrom,
			 [In] System.IO.FileAttributes dwAttrFrom,
			 [In] string pszTo,
			 [In] System.IO.FileAttributes dwAttrTo
		);
	}
}
