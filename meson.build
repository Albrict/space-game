project('shspace', 'cpp',
        default_options: ['default_library=static', 'cpp_std=c++20'])
sdl2_dep = [dependency('sdl2')]

sdl_image = dependency('SDL2_image')

sdl_ttf = dependency('SDL2_ttf')

sanitazers = ['-fsanitize=address', '-fsanitize=leak', '-fsanitize=undefined', '-fsanitize=null']
src = ['src/main.cpp', 'src/game.cpp', 'src/esh_error.cpp', 'src/button.cpp', 'src/label.cpp', 
'src/sprite_sheet.cpp', 'src/play_scene.cpp', 'src/menu_scene.cpp', 'src/player.cpp', 'src/asteroid.cpp', 'src/timer.cpp', 'src/projectile.cpp', 
'src/player.cpp', 'src/texture.cpp', 'src/game_over_scene.cpp']

if get_option('buildtype') == 'debug'
    add_project_arguments(sanitazers, language : 'cpp')
    add_project_link_arguments(sanitazers, language : 'cpp')
endif

executable('shspace', src, dependencies: [sdl2_dep, sdl_image, sdl_ttf])
