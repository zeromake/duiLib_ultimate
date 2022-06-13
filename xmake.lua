add_rules("mode.debug", "mode.release")

local isMingw = false

option("unicode")
    set_default(false)
    set_showmenu(true)
    add_defines("UNICODE", "_UNICODE", "PUGIXML_WCHAR_MODE")
    if (isMingw) then
        add_cxxflags("-municode")
        add_ldflags("-municode", {force = true})
    else
        add_cxflags("/D UNICODE")
    end

option("application")
    set_default(false)
    set_showmenu(true)
    if (isMingw) then
        add_cxflags("-Wl,--subsystem,windows", {force = true})
        add_ldflags("-Wl,--subsystem,windows", {force = true})
    else
        add_ldflags("/SUBSYSTEM:WINDOWS")
    end

option("utf8")
    set_default(true)
    set_showmenu(true)
    if (isMingw) then
    else
        add_cxflags("/utf-8", {force = true})
    end

target("cximage")
    set_kind("static")
    add_defines("WIN32")
    add_options("unicode", "utf8")
    add_links("gdi32")
    add_files(
        "./3rd/cximage/ximage.cpp",
        "./3rd/cximage/ximaenc.cpp",
        "./3rd/cximage/ximagif.cpp",
        "./3rd/cximage/ximainfo.cpp",
        "./3rd/cximage/ximalpha.cpp",
        "./3rd/cximage/ximapal.cpp",
        "./3rd/cximage/ximatran.cpp",
        "./3rd/cximage/ximawnd.cpp",
        "./3rd/cximage/xmemfile.cpp"
    )

target("controlax")
    set_kind("static")
    add_options("unicode", "utf8")
    add_defines("WIN32", "UILIB_EXPORTS")
    add_includedirs("./src/controlax")
    add_includedirs("./src/duilib")
    add_files("./src/controlax/*.cpp")
    add_links(
        "shell32",
        "oleaut32"
    )

target("duilib")
    set_kind("static")
    add_options("unicode", "utf8")
    add_defines("WIN32", "UILIB_EXPORTS")
    add_defines("USE_XIMAGE_EFFECT")
    set_pcxxheader("./src/duilib/stdafx.h")
    add_includedirs("./src/duilib")
    add_includedirs("./3rd/cximage")
    add_links(
        "gdiplus", -- render core
        "comctl32", -- UIManager
        "gdi32", -- dpi
        "imm32", -- Control/UIRichEdit
        "ws2_32", -- Control/UIIPAddress
        -- mingw support
        "ole32", -- render ole support
        "uuid" -- drag UIActiveX uuid
        -- "shell32",
        -- "oleaut32"
        -- "shlwapi",
        -- "winmm",
        -- "user32"
    )
    add_files("./src/duilib/**.cpp")
    add_deps("cximage")

target("duilib_ex")
    set_languages("c++20")
    set_kind("static")
    add_options("unicode", "utf8")
    add_defines("WIN32", "UILIB_EXPORTS", "PUGIXML_HEADER_ONLY")
    add_defines("USE_XIMAGE_EFFECT", "ui_pugi=pugi")
    set_pcxxheader("./src/duilib_ex/stdafx.h")
    add_includedirs("./src/duilib_ex")
    add_includedirs("./3rd/cximage")
    add_includedirs("./3rd/pugixml")
    add_includedirs("./3rd")
    add_links(
        "gdiplus", -- render core
        "comctl32", -- UIManager
        "gdi32", -- dpi
        "imm32", -- Control/UIRichEdit
        "ws2_32", -- Control/UIIPAddress
        -- mingw support
        "ole32", -- render ole support
        "uuid", -- drag UIActiveX uuid
        "oleaut32", -- UIDateTime
        "advapi32", -- UIAppRegistry reg support
        "shell32", -- PaintManagerUI OnDrop support
        "winmm",
        "msimg32",
        "shlwapi"
        -- "winmm",
        -- "user32"
    )
    add_files("./src/duilib_ex/**.cpp")
    add_deps("cximage")

target("hello")
    set_kind("binary")
    add_options("unicode", "utf8", "application")
    add_defines("WIN32", "UILIB_EXPORTS")
    if (not has_config("application")) then
        add_defines("_CONSOLE")
    end
    add_includedirs("./src/duilib", "./src/controlax")
    add_files("./demo/hello/main.cpp", "./demo/hello/hello.rc")
    add_deps("duilib")
    add_deps("controlax")


target("hello_ex")
    set_kind("binary")
    add_options("unicode", "utf8", "application")
    add_defines("WIN32", "UILIB_EXPORTS")
    if (not has_config("application")) then
        add_defines("_CONSOLE")
    end
    add_includedirs("./src/duilib_ex")
    add_includedirs("./3rd/pugixml")
    add_includedirs("./3rd")
    add_files("./demo/hello_ex/main.cpp", "./demo/hello_ex/hello.rc") --"./manifest/dpi.manifest")
    add_deps("duilib_ex")
    if (isMingw) then
        add_ldflags("-static", {force = true})
    end

target("image_box")
    set_kind("binary")
    add_options("unicode", "utf8", "application")
    add_defines("WIN32", "UILIB_EXPORTS")
    if (not has_config("application")) then
        add_defines("_CONSOLE")
    end
    add_includedirs("./src/duilib_ex")
    add_includedirs("./3rd")
    add_includedirs("./3rd/pugixml")
    add_files("./demo/image_box/main.cpp", "./manifest/dpi.rc")
    add_deps("duilib_ex")

target("message")
    set_kind("binary")
    add_options("unicode", "utf8", "application")
    add_defines("WIN32")
    if (not has_config("application")) then
        add_defines("_CONSOLE")
    end
    add_links("user32")
    add_files("./demo/message/main.cpp")



target("animation")
    set_kind("binary")
    add_options("unicode", "utf8", "application")
    add_defines("WIN32")
    if (not has_config("application")) then
        add_defines("_CONSOLE")
    end
    add_links("user32", "gdi32")
    add_files("./demo/animation/main.cpp", "./manifest/dpi.rc")
