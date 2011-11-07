#include <string.h>
#include "Python.h"
#include "../minrep.h"

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  _canonic (canonic)
 *  Description:  Takes a bytesarray object, brings it to canonical representation in
 *                place and returns a string (immutable) object representing the
 *                canonic form of the input sequence.
 * =====================================================================================
 */
static PyObject * _canonic(PyObject *self, PyObject *args)
{
        Py_buffer input;
	if (!PyArg_ParseTuple(args, "y*", &input)) {
		return NULL;
        }
	minrep_inplace(input.buf, input.len);
	return Py_BuildValue("s#", input.buf, input.len);
}

static PyObject * _minrep(PyObject *self, PyObject *args)
{
	// compares two cyclic strings and returns alphabetically smallest
	// cyclic shift of the string which contains the smallest such shift
	// from the pair
        Py_buffer inputA, inputB, *tmp;

	if (!PyArg_ParseTuple(args, "y*y*",
                              &inputA, &inputB))
        {
		return NULL;
        }
        unsigned int inputA_msp, inputB_msp, 
                     inputA_period, inputB_period;
	inputA_msp = MSP(inputA.len, inputA.buf, &inputA_period);
	inputB_msp = MSP(inputB.len, inputB.buf, &inputB_period);

	// positive result: A < B, negative result: A > B, zero: A == B
	int result = COMPARE2(inputA.len, inputB.len,
                              inputA.buf, inputB.buf,
                              inputA_msp, inputB_msp);

        // decide between A and B
        unsigned int tmp_msp;
	if (result > 0) {
                // A < B
                tmp_msp = inputA_msp;
		tmp = &inputA;
	} else if (result < 0) {
                // B < A
                tmp_msp = inputB_msp;
		tmp = &inputB;
	} else if (inputB.len < inputA.len) {
                // A == B, len(B) < len(A)
                tmp_msp = inputB_msp;
		tmp = &inputB;
	} else {
                // A == B, len(A) <= len(B)
                tmp_msp = inputA_msp;
		tmp = &inputA;
	}
        minrep_inplace_msp(tmp->buf, tmp->len, tmp_msp);

	return Py_BuildValue("s#", tmp->buf, tmp->len);
}

// TODO: create a function that simply returns minimal starting point
// TODO: create a function that compares two buffers using starting points
//       and COMPARE() function

static PyMethodDef cyclic_string_methods[] = {
        {"canonic", _canonic, METH_VARARGS, "Convert a cyclic string to its canonic form (minimal representation)"},
        {"minrep", _minrep, METH_VARARGS, "Run canonization on two strings and report a minimal representatin of the cyclically smallest string"},
        {NULL, NULL}
};

static struct PyModuleDef cyclic_string_module = {
        PyModuleDef_HEAD_INIT,
        "cyclic_string",
        "Module for working with cyclic strings",
        -1,
        cyclic_string_methods,
        NULL,
        NULL,
        NULL,
        NULL
};

PyMODINIT_FUNC PyInit_cyclic_string(void)
{
        return PyModule_Create(&cyclic_string_module);
}
