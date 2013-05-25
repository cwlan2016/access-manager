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
            "other-data/*.h"
        ]

        Group {
            name : "Forms"
            files: [
                "gui/*.ui"
            ]
        }

        Group {
            name : "Resources"
            files: [
                "images/*.png",
                "data.ico",
                "access-manager.rc",
                "schemadevicelist.xsd",
                "resource.qrc"
            ]
        }

        Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "xmlpatterns"] }
        Depends { name: "cpp" }

        cpp.includePaths: ["."]

//	Bug in this parameter or in my head.        
//        cpp.precompiledHeader: "stdafx.h"

        cpp.cxxFlags: {
            if ((qbs.toolchain == "gcc") || (qbs.toolchain == "mingw") || (qbs.toolchain == "clang"))
                return "-std=c++11"
        }

        cpp.dynamicLibraries: {
            if ((qbs.targetOS == "windows") && (qbs.buildVariant == "debug"))
                return ["netsnmpd","wsock32","advapi32"]
            else if ((qbs.targetOS == "windows") && (qbs.buildVariant == "release"))
                return ["netsnmp","wsock32","advapi32"]
            else
                return ["netsnmp"]
        }
    }
}  
