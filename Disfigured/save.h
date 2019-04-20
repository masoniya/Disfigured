#pragma once

#ifdef __cplusplus
extern "C" {
#endif
	void save_image(char const *filename, int x, int y, int comp, const void *data, int stride_bytes);
#ifdef __cplusplus
}
#endif
