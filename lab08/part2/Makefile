# This is a template makefile for Vulkan.
# You will need to customize it for your installation.

# Set the value of SDK_LOC to the location of your Vulkan Samples

SDK_LOC = /home/sbonada/Documents/repo/VulkanSamples

# This is where you specify the name of your program.
# It is assumed that there is a $(TARGET).cpp file to start with

TARGET = draw_cube

CXX_FLAGS =  -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-strict-aliasing -fno-builtin-memcmp -std=c++11 -fno-rtti -fvisibility=hidden -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-strict-aliasing -fno-builtin-memcmp -std=c++11 -fvisibility=hidden -Wno-sign-compare -g

CXX_DEFINES = -DDATADIR=\"share\" -DSYSCONFDIR=\"etc\" -DVK_USE_PLATFORM_XCB_KHR -DVK_USE_PLATFORM_XLIB_KHR -DVULKAN_SAMPLES_BASE_DIR=\"/home/mark/gitco/VulkanSamples\"

CXX_INCLUDES = -I$(SDK_LOC)/include -I$(SDK_LOC)/API-Samples/utils -I/usr/include/vulkan -I$(SDK_LOC)/API-Samples/data -I$(SDK_LOC)/external/glslang -I$(SDK_LOC)/external/spirv-tools/include

$(TARGET): $(TARGET).o
	/usr/bin/c++ -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-strict-aliasing -fno-builtin-memcmp -std=c++11 -fno-rtti -fvisibility=hidden -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers -fno-strict-aliasing -fno-builtin-memcmp -std=c++11 -fvisibility=hidden -Wno-sign-compare -g   draw_cube.o  -o draw_cube -rdynamic -lxcb -lm $(SDK_LOC)/dbuild/API-Samples/utils/libvsamputils.a $(SDK_LOC)/external/glslang/build/install/lib/libglslang.a $(SDK_LOC)/external/glslang/build/install/lib/libOGLCompiler.a $(SDK_LOC)/external/glslang/build/install/lib/libOSDependent.a $(SDK_LOC)/external/glslang/build/install/lib/libHLSL.a $(SDK_LOC)/external/glslang/build/install/lib/libSPIRV.a $(SDK_LOC)/external/glslang/build/install/lib/libSPVRemapper.a -lxcb $(SDK_LOC)/dbuild/loader/libvulkan.so.1.0.32 -lpthread $(SDK_LOC)/external/spirv-tools/build/source/libSPIRV-Tools.a -ldl -lpthread -lm -lm -Wl,-rpath,$(SDK_LOC)/dbuild/loader

$(TARGET).o: $(TARGET).cpp cube_data.h
	/usr/bin/c++ $(CXX_FLAGS) $(CXX_DEFINES) $(CXX_INCLUDES) -c draw_cube.cpp -o draw_cube.o

clean:
	rm $(TARGET) $(TARGET).o
