add_rules("mode.debug", "mode.release")

local isMingw = false

option("unicode")
    set_default(false)
    set_showmenu(true)
    add_defines("UNICODE", "_UNICODE")
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

target("message")
    set_kind("binary")
    add_options("unicode", "utf8", "application")
    add_defines("WIN32")
    if (not has_config("application")) then
        add_defines("_CONSOLE")
    end
    add_links("user32")
    add_files("./demo/message/main.cpp")
