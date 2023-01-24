# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "B:/Programas/Espressif/frameworks/esp-idf-v5.0/components/bootloader/subproject"
  "B:/Proyectos/ModernWood/build/bootloader"
  "B:/Proyectos/ModernWood/build/bootloader-prefix"
  "B:/Proyectos/ModernWood/build/bootloader-prefix/tmp"
  "B:/Proyectos/ModernWood/build/bootloader-prefix/src/bootloader-stamp"
  "B:/Proyectos/ModernWood/build/bootloader-prefix/src"
  "B:/Proyectos/ModernWood/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "B:/Proyectos/ModernWood/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "B:/Proyectos/ModernWood/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
