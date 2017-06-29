from distutils.core import setup, Extension


_helloworld = Extension("_helloworld",
		sources = ["helloworld.c",
			  ],
		include_dirs = ["./",
			       ],
	       )

setup(name = '_helloworld',
       version = '1.0',
       description = 'Hello world sample module',
       author = 'Digi International',
       url = 'http://www.digi.com',
       ext_modules = [_helloworld],
       py_modules = ['helloworld'],
     )
