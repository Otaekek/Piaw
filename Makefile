NAME = piaw
CFLAGS = -pthread -std=c++11 -o3
UNAME_S := $(shell uname -s)
#CFLAGS += -Wall -Werror -Wextra
#CFLAGS += -pedantic -pedantic-errors
#ifeq ($(shell basename $(CC)),clang) # Need autoconf
#CFLAGS += -Weverything
#endif

# Debug
ifeq ($(DEBUG),yes)
	CFLAGS += -g3 -O0
endif

# Profile
ifeq ($(PROF),yes)
	CFLAGS += -pg
endif

CFLAGS += -O3 -march=native -pipe -msse -msse2 -msse3 -mmmx -m3dnow 
LDFLAGS += -O3 -std=c++11

# Sanitize
ifeq ($(SAN),yes)
	LDFLAGS += -fsanitize=address
	CFLAGS += -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls
endif

CFLAGS += -fPIC
LDFLAGS += -pthread -fPIC
LDFLAGS +=  -rdynamic

# Input
#SRC_SUBDIR += input
#SOURCES += readline.c

# main
SOURCES += main.cpp

# piaw_music
SRC_SUBDIR += piaw_music
SOURCES += piawMusic.class.cpp
CFLAGS += -Isrc/piaw_music

#piawMap
SRC_SUBDIR += piaw_map_manager
SOURCES += piawMap.class.cpp
CFLAGS += -Isrc/piaw_map_manager

# cube map
 SRC_SUBDIR += cube_map
# SOURCES += mesher_emit_vertex.cpp
# SOURCES += mesher.cpp
# SOURCES += surfaceNetMesher.cpp
CFLAGS	+= -Isrc/cube_map
# SOURCES += cubeMap.class.cpp
SOURCES += perlinNoise.cpp
# SOURCES += terrainGeneration.cpp

# piaw missile
SRC_SUBDIR += piaw_missile
SOURCES += piawMissile.class.cpp
CFLAGS += -Isrc/piaw_missile

# Piaw line entity
SRC_SUBDIR += piaw_line_entity
SOURCES += piawLineEntity.class.cpp
CFLAGS += -Isrc/piaw_line_entity

# Piaw player spaceship
SRC_SUBDIR += piaw_player_spaceship
SOURCES += piawPlayerSpaceShip.class.cpp
CFLAGS += -Isrc/piaw_player_spaceship

#partcle system
SRC_SUBDIR += particle_system
CFLAGS += -Isrc/particle_system
SOURCES += particleSystem.class.cpp

# memory allocator
SRC_SUBDIR += memory_allocator
CFLAGS += -Isrc/memory_allocator/
SOURCES += stackAllocator.class.cpp
SOURCES += poolAllocator.class.cpp

# asset_db_system
SRC_SUBDIR += static_memory_manager
CFLAGS += -Isrc/static_memory_manager
SOURCES += staticMemoryManager.class.cpp

# dynamic memory manager
SRC_SUBDIR += dynamic_memory_manager
CFLAGS += -Isrc/dynamic_memory_manager
SOURCES += dynamicMemoryManager.class.cpp

# file loader
SRC_SUBDIR += file_loader
CFLAGS += -Isrc/file_loader
SOURCES += fileLoader.class.cpp

#	renderer
SRC_SUBDIR += renderer
CFLAGS += -Isrc/renderer
SOURCES += onScreenRenderPass.cpp
SOURCES += renderDataSys.class.cpp
SOURCES += complexObjectRenderingPipeline.cpp
SOURCES += renderBuiltIn.class.cpp
SOURCES	+= renderBuiltInInstancing.class.cpp
#SOURCES += voxRenderer.cpp

# jb system
SRC_SUBDIR += job_system
CFLAGS += -Isrc/job_system
SOURCES += jobHandler.class.cpp

# transform
SRC_SUBDIR += transform
CFLAGS += -Isrc/transform
SOURCES += transform.class.cpp

# fpsController
SRC_SUBDIR += basicFPSControlBuiltin
CFLAGS += -Isrc/basicFPSControlBuiltin
SOURCES += basicFPSControlBuiltin.class.cpp

# fpsController
SRC_SUBDIR += input_builtin
CFLAGS += -Isrc/input_builtin
SOURCES += inputBuiltin.class.cpp

# static mesh factory
SRC_SUBDIR += static_mesh_factory
CFLAGS += -Isrc/static_mesh_factory
SOURCES += basicMeshFactory.class.cpp

# basic light factory
SRC_SUBDIR += basic_light_factory
CFLAGS += -Isrc/basic_light_factory
SOURCES+= basicLightFactory.class.cpp

# Terrain generation
SRC_SUBDIR += terrain_generation_builtin
CFLAGS += -Isrc/terrain_generation_builtin
SOURCES += terrainGenerationBuiltin.class.cpp

# texture Built in
SRC_SUBDIR += texture_builtin
CFLAGS += -Isrc/texture_builtin
SOURCES += texture_builtin.class.cpp

#shutdown Builtin
SRC_SUBDIR += shutdown
CFLAGS += -Isrc/shutdown
SOURCES += shutdown.class.cpp

#map editor
SRC_SUBDIR += map_editor_builtin
CFLAGS += -Isrc/map_editor_builtin
SOURCES += mapEditorBuiltin.class.cpp

# directory smart loader
SRC_SUBDIR += directory_smart_loader
CFLAGS += -Isrc/directory_smart_loader
SOURCES += directorySmartLoader.class.cpp

# runTime library
SRC_SUBDIR += runtime_library
CFLAGS += -Isrc/runtime_library
SOURCES += runtimeLibrary.class.cpp

# #Voxels physic
# SRC_SUBDIR += voxPhysic
# SOURCES += voxPhysic.class.cpp
# CFLAGS += -Isrc/voxPhysic

# GUI Event System
SRC_SUBDIR += GUI
CFLAGS += -Isrc/GUI
SOURCES += GUIEventSystem.class.cpp

#Entity System
SRC_SUBDIR += entitySystem
SOURCES += entitySystem.class.cpp
SOURCES += entity.class.cpp

#time
SRC_SUBDIR += timeBuiltin
CFLAGS += -Isrc/timeBuiltin
SOURCES += time.class.cpp

# Generation
INC_PATH = inc
INC_PATH += assimp/include
SRC_PATH = src
CFLAGS += $(addprefix -I,$(INC_PATH))
vpath %.cpp $(SRC_PATH) $(addprefix $(SRC_PATH)/,$(SRC_SUBDIR))

# Lib tierces
#LIB42_PATH = lib42
#LIB42 = $(LIB42_PATH)/lib42.a
#CFLAGS += -I $(LIB42_PATH)/inc
#LDFLAGS += -L $(LIB42_PATH) -l42

# Lib tierces

#  assimp
LIB_ASSIMP_PATH = assimp

CFLAGS += -I$(LIB_ASSIMP_PATH)/include
LDFLAGS += -L./$(LIB_ASSIMP_PATH)/lib/ -lassimp

#sfml
LDFLAGS += -lsfml-audio

#  glm
LIB_GLM_PATH = glm/glm/
CFLAGS += -I$(LIB_GLM_PATH)

# glfw
CFLAGS += -I./glfw-3.2.1/include/GLFW/ -DGL_GLEXT_PROTOTYPES
ifeq ($(UNAME_S),Linux)
	LDFLAGS += -D_GLIBCXX_GTHREAD_USE_WEAK=0  -lglfw3 -lrt -lm -ldl -lXrandr \
	-lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes \
	-lX11 -lpthread -lxcb -lXau -lXdmcp -lGL  -DGL_GLEXT_PROTOTYPES
endif
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -L/usr/local/lib -L$(HOME)/.brew/lib -lglfw
endif

#  opengl
ifeq ($(UNAME_S),Linux)
	LDFLAGS += -lGL -lGLU
endif
ifeq ($(UNAME_S),Darwin)
	LDFLAGS += -framework opengl
endif

# FREEIMAGE
LDFLAGS += -lfreeimage

# Object files
OBJ_PATH = .obj
OBJECTS = $(SOURCES:%.cpp=$(OBJ_PATH)/%.o)

BUILD_DIR = $(OBJ_PATH) $(DEP_PATH)

# Rules
.PHONY: all

.SECONDARY: $(OBJECTS)

all: sound script $(LIB42) $(DEPS) $(NAME)

script:
	@export LD_LIBRARY_PATH=./assimp/lib
	@echo  $(CFLAGS) "\c" > .tmpheader
	@echo -n $(LDFLAGS) > .tmpobject
	@python3 genHeader.py > headerList.hpp

$(NAME): $(OBJECTS) | $(LIB42)
	g++ -o $@ $^ $(LDFLAGS)
	#aplay makefile_asset/oe.wav&

$(LIB42):
	$(MAKE) -C $(LIB42_PATH) all

$(OBJECTS): $(OBJ_PATH)/%.o: %.cpp | $(OBJ_PATH)
	g++ $(CFLAGS) -o $@ -c $<
	@echo -n " " $@ >> .tmpobject

$(BUILD_DIR):
	@-mkdir -p $@

.PHONY: clean fclean re sanitize unsanitize

clean:
	$(RM) -r $(OBJ_PATH)

fclean: clean
	$(RM) $(NAME)
	$(RM) -rf $(DEP_PATH)

sound:
	#aplay makefile_asset/roll.wav&

re: fclean all

sanitize:
	$(MAKE) -C ./ re SAN=yes DEBUG=yes

unsanitize:
	$(MAKE) -C ./ re DEBUG=yes

# Submodule
.PHONY: sub-update sub-init

sub-init:
	git submodule update --init --recursive

sub-update:
	git submodule update --remote --recursive

# Tests
.PHONY: check test-cleanup

check: all
	@$(MAKE) -C $(TEST_PATH) re
	@./$(TEST_EXEC)

# Tools
.PHONY: norme valgrind ctags clean-tools

norme:
	@ ! norminette -R CheckTopCommentHeader $(SRC_PATH) | grep -v -B 1 "^Norme"
	@ ! norminette -R CheckTopCommentHeader $(INC_PATH) | grep -v -B 1 "^Norme"

valgrind:
	valgrind --leak-check=full ./$(NAME)

callgrind:
	valgrind --tool=callgrind --callgrind-out-file=$(CG_OUTPUT_FILE) ./$(NAME)
	callgrind_annotate --auto=yes $(CG_OUTPUT_FILE)

ctags:
	ctags -R --tag-relative=yes --exclude='.git*' --exclude='test' --exclude='*.o' --exclude='*dSYM' --exclude='doc' --exclude='exercices'

clean-tools:
	$(RM) -r *.dSYM/
