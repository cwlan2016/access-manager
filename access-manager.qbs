import qbs 1.0

Project {
    Product {
        type: "application"
        name: "access-manager"

        files: [
            "*.cpp",
            "configs/*.cpp",
            "devices/*.cpp",
            "gui/*.cpp",
            "ports/*.cpp",
            "models/*.cpp",
            "other-data/*.cpp",
            "*.h",
            "configs/*.h",
            "devices/*.h",
            "ports/*.h",
            "models/*.h",
            "gui/*.h",
            "other-data/*.h",
            "gui/*.ui"
        ]

        Group {         
            name : "Other Resources"
            files: [
                "images/*.png",
                "gui/icons/*.png",
                "data.ico",
                "schemadevicelist.xsd"
            ]
        }

        Group {
            fileTags: ["qrc"]
            name: "Qt Resource"
            files: [
                "resource.qrc"
            ]
        }

        Group {
            fileTags: ["rc"]
            name: "RC Resource"
            files: [
                "access-manager.rc"
            ]
        }

        Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "xmlpatterns", "concurrent"] }
        Depends { name: "cpp" }

        cpp.includePaths: ["."]

//	Bug in this parameter or in my head.        
//        cpp.precompiledHeader: "stdafx.h"
//        cpp.precompiledHeaderDir : "."
        cpp.cxxFlags: {
        if (qbs.toolchain.contains("gcc")
                || qbs.toolchain.contains("mingw")
                || qbs.toolchain.contains("clang"))
            return "-std=c++11"
        }

        cpp.dynamicLibraries: {
            if (qbs.targetOS.contains("windows"))
                return ["netsnmp","wsock32","advapi32"]
            else
                return ["netsnmp"]
        }
    }
}  
