/*
 * This file is part of libSavitar.
 *
 * Parts of this code have been copied from libArcus
 * Copyright (C) 2016 Ultimaker b.v. <a.hiemstra@ultimaker.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */


/**
 * Convert a python str object to a std::string.
 */
%MappedType std::string
{
    %TypeHeaderCode
    #include <string>
    %End

    %ConvertFromTypeCode // From C++ to python
        PyObject* newstring;
        newstring = PyUnicode_DecodeUTF8(sipCpp->c_str(), sipCpp->length(), NULL);
        if(newstring == NULL)
        {
            PyErr_Clear();
            newstring = PyBytes_FromString(sipCpp->c_str());
        }
        return newstring;
    %End

    %ConvertToTypeCode // From python to C++
        // Allow a Python string (or a unicode string) whenever a string is
        // expected.
        // If argument is a Unicode string, just decode it to UTF-8
        // If argument is a Python string, assume it's UTF-8
        if (sipIsErr == NULL)
        {
            return (PyBytes_Check(sipPy) || PyUnicode_Check(sipPy));
        }

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

/**
 * Convert a list filled with SceneNodes to a vector with SceneNodes.
 */
%MappedType std::vector<SceneNode*>
{
    %TypeHeaderCode
    #include <vector>
    %End

    %ConvertFromTypeCode // From C++ to python
        PyObject *result_list = PyList_New(sipCpp -> size());

        // Create the Python list of the correct length.
        if (!result_list)
        {
            return NULL;
        }

        // Go through each element in the C++ instance and convert it to a
        // wrapped P2d.
        for (int i = 0; i < (int)sipCpp->size(); ++i)
        {
            SceneNode *cpp = new SceneNode(*sipCpp->at(i));
            PyObject *pobj = sipConvertFromInstance(cpp, sipClass_SceneNode, sipTransferObj);

            // Get the Python wrapper for the Type instance, creating a new
            // one if necessary, and handle any ownership transfer.
            if (!pobj)
            {
                // There was an error so garbage collect the Python list.
                Py_DECREF(result_list);
                return NULL;
            }

            // Add the wrapper to the list.
            PyList_SET_ITEM(result_list, i, pobj);
        }

        // Return the Python list.
        return result_list;
    %End

    %ConvertToTypeCode // From python to C++
        // Check if type is compatible
        if (!sipIsErr)
        {
            // Must be any iterable
            PyObject *i = PyObject_GetIter(sipPy);
            bool iterable = (i != NULL);
            Py_XDECREF(i);
            return iterable;
        }

        // Iterate over the object
        PyObject *iterator = PyObject_GetIter(sipPy);
        PyObject *item;

        std::vector<SceneNode*> *result_vector = new std::vector<SceneNode*>();

        while ((item = PyIter_Next(iterator)))
        {
            if (!sipCanConvertToInstance(item, sipClass_SceneNode, SIP_NOT_NONE))
            {
                PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to SceneNode");
                *sipIsErr = 1;
                break;
            }

            int state;
            SceneNode* p = reinterpret_cast<SceneNode*>(sipConvertToInstance(item, sipClass_SceneNode, 0, SIP_NOT_NONE, &state, sipIsErr));

            if (!*sipIsErr)
            {
                result_vector->push_back(p);
            }

            sipReleaseInstance(p, sipClass_SceneNode, state);
            Py_DECREF(item);
        }

        Py_DECREF(iterator);

        if (*sipIsErr)
        {
            delete result_vector;
            return 0;
        }

        *sipCppPtr = result_vector;
        return sipGetState(sipTransferObj);
    %End
};

/**
 * Convert to and from byte arrays.
 * Uses the internal data vector directly to create/extract a PyBytes* instance.
 */
%MappedType bytearray
{
    %TypeHeaderCode
    #include <vector>
    #include <cstdint>
    #include "Types.h"
    %End

    %ConvertFromTypeCode // From C++ to python
        return PyBytes_FromStringAndSize(reinterpret_cast<const char *>(sipCpp->data()), sipCpp->size());
    %End

    %ConvertToTypeCode // From python to C++
        if (sipIsErr == NULL)
        {
            return (PyBytes_Check(sipPy));
        }

        if (sipPy == Py_None)
        {
            *sipCppPtr = new bytearray;
            return 1;
        }

        if (PyBytes_Check(sipPy))
        {
            uint8_t *buffer = reinterpret_cast<uint8_t *>(PyBytes_AS_STRING(sipPy));
            *sipCppPtr = new bytearray(buffer, buffer + PyBytes_GET_SIZE(sipPy));
            return 1;
        }
        return 0;
    %End
};

/**
 * SIP generic implementation for std::vector<TYPE>
 */
template<TYPE>
%MappedType std::vector<TYPE>
{
    %TypeHeaderCode
    #include <vector>
    %End

    %ConvertFromTypeCode // From C++ to python
        PyObject *result_list = PyList_New(sipCpp -> size());

        // Create the Python list of the correct length.
        if (!result_list)
        {
            return NULL;
        }

        // Go through each element in the C++ instance and convert it to a
        // wrapped P2d.
        for (int i = 0; i < (int)sipCpp->size(); ++i)
        {
            TYPE *cpp = new TYPE(sipCpp->at(i));
            PyObject *pobj = sipConvertFromInstance(cpp, sipClass_TYPE, sipTransferObj);

            // Get the Python wrapper for the Type instance, creating a new
            // one if necessary, and handle any ownership transfer.
            if (!pobj)
            {
                // There was an error so garbage collect the Python list.
                Py_DECREF(result_list);
                return NULL;
            }

            // Add the wrapper to the list.
            PyList_SET_ITEM(result_list, i, pobj);
        }

        // Return the Python list.
        return result_list;
    %End

    %ConvertToTypeCode // From python to C++
        // Check if type is compatible
        if (!sipIsErr)
        {
            // Must be any iterable
            PyObject *i = PyObject_GetIter(sipPy);
            bool iterable = (i != NULL);
            Py_XDECREF(i);
            return iterable;
        }

        // Iterate over the object
        PyObject *iterator = PyObject_GetIter(sipPy);
        PyObject *item;

        std::vector<TYPE> *result_vector = new std::vector<TYPE>();

        while ((item = PyIter_Next(iterator)))
        {
            if (!sipCanConvertToInstance(item, sipClass_TYPE, SIP_NOT_NONE))
            {
                PyErr_Format(PyExc_TypeError, "object in iterable cannot be converted to TYPE");
                *sipIsErr = 1;
                break;
            }

            int state;
            TYPE* p = reinterpret_cast<TYPE*>(sipConvertToInstance(item, sipClass_TYPE, 0, SIP_NOT_NONE, &state, sipIsErr));

            if (!*sipIsErr)
            {
                result_vector->push_back(*p);
            }

            sipReleaseInstance(p, sipClass_TYPE, state);
            Py_DECREF(item);
        }

        Py_DECREF(iterator);

        if (*sipIsErr)
        {
            delete result_vector;
            return 0;
        }

        *sipCppPtr = result_vector;
        return sipGetState(sipTransferObj);
    %End
};


/**
 * Convert a (python) dict with strings as keys and values to a map<string, string>.
 */
%MappedType std::map<std::string, std::string>
{
    %TypeHeaderCode
    #include <map>
    %End

    %ConvertFromTypeCode // From C++ to python
        // Create the dictionary.
        PyObject *result_dict = PyDict_New();

        if (!result_dict)
        {
            return NULL;
        }

        // Set the dictionary elements.
        std::map<std::string, std::string>::const_iterator i = sipCpp->begin();

        while (i != sipCpp->end())
        {
            std::string *key = new std::string((*i).first);
            std::string *value = new std::string((*i).second);

            PyObject *key_object = sipConvertFromNewType(key, sipType_std_string, sipTransferObj);
            PyObject *value_object = sipConvertFromNewType(value, sipType_std_string, sipTransferObj);

            if (key_object == NULL || value_object == NULL || PyDict_SetItem(result_dict, key_object, value_object) < 0)
            {
                Py_DECREF(result_dict);

                if (key_object)
                {
                    Py_DECREF(key_object);
                }
                else
                {
                    delete key;
                }

                if (value_object)
                {
                    Py_DECREF(value_object);
                }
                else
                {
                    delete value;
                }

                return nullptr;
            }

            Py_DECREF(key_object);
            Py_DECREF(value_object);

            ++i;
        }

        return result_dict;
    %End

    %ConvertToTypeCode // From python to C++
        PyObject *key_object, *value_object;
        SIP_SSIZE_T i = 0;

        // Check the type if that is all that is required.
        if (sipIsErr == nullptr)
        {
            if (!PyDict_Check(sipPy))
            {
                return 0;
            }

            while (PyDict_Next(sipPy, &i, &key_object, &value_object))
            {
                if (!sipCanConvertToType(key_object, sipType_std_string, SIP_NOT_NONE))
                {
                    return 0;
                }

                if (!sipCanConvertToType(value_object, sipType_std_string, SIP_NOT_NONE))
                {
                    return 0;
                }
            }

            return 1;
        }

        std::map<std::string, std::string> *std_map = new std::map<std::string, std::string>;

        while (PyDict_Next(sipPy, &i, &key_object, &value_object))
        {
            int key_state, value_state;

            std::string *key = reinterpret_cast<std::string *>(sipConvertToType(key_object, sipType_std_string, sipTransferObj, SIP_NOT_NONE, &key_state, sipIsErr));
            std::string *value = reinterpret_cast<std::string *>(sipConvertToType(value_object, sipType_std_string, sipTransferObj, SIP_NOT_NONE, &value_state, sipIsErr));

            if (*sipIsErr)
            {
                sipReleaseType(key, sipType_std_string, key_state);
                sipReleaseType(value, sipType_std_string, value_state);

                delete std_map;
                return 0;
            }

            (*std_map)[*key] = *value;

            sipReleaseType(key, sipType_std_string, key_state);
            sipReleaseType(value, sipType_std_string, value_state);
        }

        *sipCppPtr = std_map;

        return sipGetState(sipTransferObj);
    %End
};
