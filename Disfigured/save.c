#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include "save.h"


void save_image(char const *filename, int x, int y, int comp, const void *data, int stride_bytes) {
	stbi_flip_vertically_on_write(1);
	stbi_write_png(filename, x, y, comp, data, stride_bytes);
}
