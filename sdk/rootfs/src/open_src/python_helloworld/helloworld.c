// ext1.c: A sample C extension: one simple function

#include "Python.h"

// hello_world function.

static PyObject *
helloworld(PyObject *self, PyObject *args)
{
	(void)self;
	(void)args;

	return Py_BuildValue("s", "hello, world!");
}

// Module functions table.

static PyMethodDef module_functions[] = {
	{ "helloworld", helloworld, METH_VARARGS, "Say hello." },
	{ NULL, 0, 0, NULL }      /* sentinel */
};

// This function is called to initialize the module.

void
init_helloworld(void)
{
    Py_InitModule3("_helloworld", module_functions, "A minimal hello world module.");
}