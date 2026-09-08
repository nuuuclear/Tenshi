![logo](./Angel.png)
<br>
Angel is a 2D game engine written in c++ using SDL3 and miniaudio.

## To use it in you project:

```shell
# add it as a submodule
cd your_project
git submodule add https://github.com/nuuuclear/Angel Angel

# or clone it directly
cd your_project
git clone https://github.com/nuuuclear/Angel
```

Download and include SDL3, SDL3_ttf, SDL3_image, and miniaudio.

Using CMake:
```cmake
# inlcude it:
add_subdirectory(Angel) # or the directory you put it in.

# link it to your project:
target_link_libraries(YourGame PRIVATE angel::angel)
```

In your main.cpp:
```cpp
#include <Angel/Angel.h>

static int windowWidth = 100;
static int windowHeight = 100;

static const char* windowName = "Hello, world!";

int main(int argc, char *argv[]) {
  if (!Angel_Init()) // initializes Angel (returns true on success)
    return 1;
  
  // Create a window
  // NULL the is window creation flags, it uses the standard SDL window flags.
	if (!Angel_StartRenderer(windowName, windowWidth, windowHeight, NULL)) {
		Angel_Finish();
		return 1;
	}

  while (1) { // game loop
    input.beginFrame(); // clear input buffer
    input.poll(); // fill input buffer with input data.
  
    if (input.quitRequested()) // a more robust feature for this will come!
      running = false;
  
    // do stuff
  }
  
  Angel_Finish(); // finalize

  return 0;
}
```

To actually use the game engine, for now just look around in the source code. this is a work in progress! 
I will be making a wiki/documentation on how to use it once it is a little further into development.
<hr>

As of now this only works on Windows, although it wouldn't be too hard to get working elsewhere.
