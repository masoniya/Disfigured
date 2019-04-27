#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	//Save iamge pixel data to file
	void save_image(char const *filename, int x, int y, int comp, const void *data, int stride_bytes);
#ifdef __cplusplus
}
#endif
