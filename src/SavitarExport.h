/*
 * This file is part of libSavitar
 *
 * Copyright (C) 2017 Ultimaker b.v. <j.vankessel@ultimaker.com>
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

#ifndef SAVITAR_EXPORT_H
#define SAVITAR_EXPORT_H

#if _WIN32
    #ifdef MAKE_SAVITAR_LIB
        #define SAVITAR_EXPORT __declspec(dllexport)
    #else
        #define SAVITAR_EXPORT
    #endif
#else
    #define SAVITAR_EXPORT
#endif

#endif
