/*
 * Copyright (c) 2010, 2013, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 *
 */

#ifndef SHARE_VM_MEMORY_METADATAFACTORY_HPP
#define SHARE_VM_MEMORY_METADATAFACTORY_HPP

#include "classfile/classLoaderData.hpp"
#include "utilities/array.hpp"
#include "utilities/exceptions.hpp"
#include "utilities/globalDefinitions.hpp"

class MetadataFactory : AllStatic {
 public:
  template <typename T>
  static Array<T>* new_array(ClassLoaderData* loader_data, int length, TRAPS) {
    // The "true" argument is because all metadata arrays are read only when
    // dumped to the shared archive
    return new (loader_data, length, /*read_only*/true, THREAD) Array<T>(length);
  }

  template <typename T>
  static Array<T>* new_array(ClassLoaderData* loader_data, int length, T value, TRAPS) {
    Array<T>* array = new_array<T>(loader_data, length, CHECK_NULL);
    for (int i = 0; i < length; i++) {
      array->at_put(i, value);
    }
    return array;
  }

  template <typename T>
  static Array<T>* new_writeable_array(ClassLoaderData* loader_data, int length, TRAPS) {
    return new (loader_data, length, /*read_only*/false, THREAD) Array<T>(length);
  }

  template <typename T>
  static Array<T>* new_writeable_array(ClassLoaderData* loader_data, int length, T value, TRAPS) {
    Array<T>* array = new_writeable_array<T>(loader_data, length, CHECK_NULL);
    for (int i = 0; i < length; i++) {
      array->at_put(i, value);
    }
    return array;
  }

  template <typename T>
  static void free_array(ClassLoaderData* loader_data, Array<T>* data) {
      delete[] data;
  }

  // Deallocation method for metadata
  template <class T>
  static void free_metadata(ClassLoaderData* loader_data, T md) {
      delete md;
  }
};

#endif // SHARE_VM_MEMORY_METADATAFACTORY_HPP
