
add_requires("tinyxml2")

target("xml_benchmark")
    set_kind("binary")
    add_options("unicode", "utf8")
    add_includedirs("../../3rd/pugixml")
    add_defines("WIN32", "PUGIXML_HEADER_ONLY")--, "PUGIXML_COMPACT")
    add_links("user32")
    add_files("./main.cpp")
    add_packages("tinyxml2")
