-- set minimum xmake version
set_xmakever("2.8.2")

-- includes
includes("lib/commonlibf4")

-- set project
set_project("DisableLevelUpHealing")
set_version("1.0.2")
set_license("GPL-3.0")

-- set defaults
set_languages("c++23")
set_warnings("allextra")

-- set policies
set_policy("package.requires_lock", true)

-- add rules
add_rules("mode.debug", "mode.releasedbg")
add_rules("plugin.vsxmake.autoupdate")

-- add requires
add_requires("cmake")
add_requires("simpleini")
add_requires("rsm-mmio")
add_requires("spdlog")

-- targets
target("DisableLevelUpHealing")
    -- add dependencies to target
    add_deps("commonlibf4")

	-- add packages
	add_packages("simpleini")

    -- add commonlibf4 plugin
    add_rules("commonlibf4.plugin", {
        name = "DisableLevelUpHealing",
        author = "ForestJ316"
    })

    -- add src files
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_includedirs("src")
    set_pcxxheader("src/pch.h")

	-- add extra files
	add_extrafiles(".clang-format")
