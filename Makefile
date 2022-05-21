#
# Cross Platform Makefile
# Compatible with MSYS2/MINGW, Ubuntu 14.04.1 and Mac OS X
#
# You will need SDL2 (http://www.libsdl.org):
# Linux:
#   apt-get install libsdl2-dev
# Mac OS X:
#   brew install sdl2
# MSYS2:
#   pacman -S mingw-w64-i686-SDL2
#

CXX = g++
# CXX = clang++

EXE = sarasa # Esta variable conviene hacerle un override cuando corremos mingw32-make , que es lo que yo hago para ponerle el nombre de la carpeta.
IMGUI_DIR = ../..
SOURCES = main.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_tables.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl3.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)
LINUX_GL_LIBS = -lGL

CXXFLAGS = -std=c++11 -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat
#-g ; Es para debug así que lo saco
LIBS =

##---------------------------------------------------------------------
## OPENGL ES
##---------------------------------------------------------------------

## This assumes a GL ES library available in the system, e.g. libGLESv2.so
# CXXFLAGS += -DIMGUI_IMPL_OPENGL_ES2
# LINUX_GL_LIBS = -lGLESv2
## If you're on a Raspberry Pi and want to use the legacy drivers,
## use the following instead:
# LINUX_GL_LIBS = -L/opt/vc/lib -lbrcmGLESv2

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += $(LINUX_GL_LIBS) -ldl `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(OS), Windows_NT)
    ECHO_MESSAGE = "MinGW"
    # VOY a hacer un STATIC link de todo, pero como dice el sdl2-config.cmake (ver ese archivo que tiene tipo toda la posta del universo dentro de la carpeta lib/cmake de la librería) TIENEN que estar las siguientes librerías EN ORDEN, y ANTES QUE TODO EL RESTO!! (o sea las de mingw32 y sdl)
	LIBS += -static # Forzamos que busque static de todo que es lo más práctico en vez de andar poniendo de a uno.
	LIBS += -lmingw32 -lSDL2main -lSDL2
	# Estos son supuestamente necesarios para sdl y el opengl obviamente, si hacemos dynamic o static como sea
    LIBS += -lgdi32 -lopengl32 -limm32 
    ###
    ## Acá agrego OTRAS librerías que son parte de sdl pero según el archivo de cmake mismo SON necesarias si quiero hacer STATIC link de SDL (y es verdad porque si no las pongo no compila). Lo que sí los dos primeros parámetros no parecen ser necesarios de hecho lleva otro más primero que no puse llamado "-Wl,--dynamicbase" que por decir dynamic no lo pongo nomás.
    
	LIBS += -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

    ### ### COLOCAR  directorios de sdl aca! las LIBS y los INCLUDE, en los flags, por qué no sé porque muchas veces falla si los ponemos al principio!
    ###### Por último los flags de C . Lo podemos poner todo el LIBS o FLAGS es lo mismo porque el compilador lee todo de corrido, pero así queda lindo. -mwindows necesitamos viejo y conocido para que no abra la pedorra consola o bien -WL,--subsystem-windows.
    # Clave el -s para que no pese infinito.
    CXXFLAGS += -I../../../SDL2_2.0.20/x86_64-w64-mingw32/include/SDL2/ -L../../../SDL2_2.0.20/x86_64-w64-mingw32/lib/ -s
    #-mwindows

     ### `pkg-config --static --libs sdl2` ;Esto viene por defecto para usar con "pkg-config" que es una pija que nunca anda y para orto compilado, se va 
     ####
    CFLAGS = $(CXXFLAGS)
	endif 

# 	-static-libgcc -static-libstdc++ -Wl,-Bstatic -lwinpthread
# --std=c++17 
# -lSDL2
#     -lmingw32
# -Wl,--subsystem,console
# -Wl,--entry,mainCRTStartup
# -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive
# -municode
# -lmingw32
# -lcomctl32 -D WIN_32_LEAN_AND_MEAN -D UNICODE -D _UNICODE
##`pkg-config --cflags sdl2`


##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
