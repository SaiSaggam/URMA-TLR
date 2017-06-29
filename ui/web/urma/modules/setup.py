from distutils.core import setup, Extension


ddp_if = Extension("ddp_if",
		sources = ["ddp_python.c",
				"ddp_if.c",
			   "ddp_tlr.c",
			   "ddp_firmware.c",
			  ],
		include_dirs = ["./",
			       ],
	       )

setup(name = 'ddp_if',
       version = '1.0',
       description = 'Digi DDP module',
       author = 'Digi International',
       url = 'http://www.digi.com',
       ext_modules = [ddp_if],
       py_modules = [],
     )
