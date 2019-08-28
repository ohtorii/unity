using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace file_searcher
{
	class Program
	{
		static int Main(string[] args)
		{
			var app = new Application();
			return (int)app.Exec(args);
		}
	}
}
