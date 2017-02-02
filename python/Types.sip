/*
 * This file is part of libSavitar.
 *
 * Parts of this code have been copied from libArcus
 * Copyright (C) 2016 Ultimaker b.v. <a.hiemstra@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// Convert a python str object to a std::string.
%MappedType std::string
{
%TypeHeaderCode
#include <string>
%End

%ConvertFromTypeCode
    // convert an std::string to a Python (unicode) string
    PyObject* newstring;
    newstring = PyUnicode_DecodeUTF8(sipCpp->c_str(), sipCpp->length(), NULL);
    if(newstring == NULL) {
        PyErr_Clear();
        newstring = PyBytes_FromString(sipCpp->c_str());
    }
    return newstring;
%End

%ConvertToTypeCode
    // Allow a Python string (or a unicode string) whenever a string is
    // expected.
    // If argument is a Unicode string, just decode it to UTF-8
    // If argument is a Python string, assume it's UTF-8
    if (sipIsErr == NULL)
        return (PyBytes_Check(sipPy) || PyUnicode_Check(sipPy));

    if (sipPy == Py_None)
    {
        *sipCppPtr = new std::string;
        return 1;
    }

    if (PyUnicode_Check(sipPy))
    {
        PyObject* s = PyUnicode_AsEncodedString(sipPy, "UTF-8", "");
        *sipCppPtr = new std::string(PyBytes_AS_STRING(s));
        Py_DECREF(s);
        return 1;
    }

    if (PyBytes_Check(sipPy))
    {
        *sipCppPtr = new std::string(PyBytes_AS_STRING(sipPy));
        return 1;
    }
    return 0;
%End
};

// vector<SceneNode> is implemented as a Python list.
%MappedType std::vector<SceneNode>
{
%TypeHeaderCode
#include <vector>
using namespace std;
%End

%ConvertFromTypeCode
    PyObject* list;
    if ((list = PyList_New(sipCpp->size())) == NULL)
    {
        return NULL;
    }
    // Set the vector elements.
    vector<SceneNode>::iterator iter;
    int i(0);
    for (int i = 0; i < sipCpp->size(); ++i)
    {
        SceneNode *w = &sipCpp->at(i);
        PyObject *wobj;

        // Get the Python wrapper for the QWidget instance, creating a new
        // one if necessary, and handle any ownership transfer.
        if ((wobj = sipConvertFromType(w, sipType_SceneNode, sipTransferObj)) == NULL)
        {
            // There was an error so garbage collect the Python list.
            Py_DECREF(list);
            return NULL;
        }

        PyList_SET_ITEM(list, i++, wobj );
    }
    return list;
%End

%ConvertToTypeCode
  if (sipIsErr == NULL)
  {
    return PyList_Check(sipPy);
  }
  vector<SceneNode> *scene_node_vector = new vector<SceneNode>;
  for (int i = 0; i < PyList_GET_SIZE(sipPy); ++i)
  {
    //scene_node_vector->push_back(PyList_GET_ITEM(sipPy, i));
  }
  *sipCppPtr = scene_node_vector;
  return sipGetState(sipTransferObj);
%End
};