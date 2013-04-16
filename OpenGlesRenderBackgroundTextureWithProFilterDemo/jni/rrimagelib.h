/**
 * 封装libjpeg和libpng的调用
 *
 * @Author Tristan Sun
 * @Data 2012-11-09
 */

#ifndef __RRIMAGELIB_H
#define __RRIMAGELIB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <jpeglib.h>
#include <setjmp.h>
#include <android/log.h>
#include <math.h>
#include <string.h>

#define RR_DEBUG 1
#define RR_SIG -907575282
#define RR_DEBUG_SIG -1760852138

#define LOG_TAG "rrimagelib"

#define PNG_MAGIC_SIZE 4

// 图片类型常量
#define TYPE_RRIMAGE_UNSPECIFIED 0
#define TYPE_RRIMAGE_JPEG 1
#define TYPE_RRIMAGE_PNG 2
#define TYPE_RRIMAGE_BMP 3


// 压缩常量
#define COMPRESS_MAX_WIDTH 1600
#define COMPRESS_MIN_WIDTH 960

// 定义rotate的旋转常量（顺时针）
#define ROTATE_0   0x001
#define ROTATE_90  0x002
#define ROTATE_180 0x004
#define ROTATE_270 0x008
// 兼容以前版本保留的变量
#define ISO_0	0x010
#define ISO_180 0x020
#define ISO_360 0x040
// 镜像
#define FLIP_LEFT_RIGHT	0x101
#define FLIP_TOP_BOTTOM	0x102

#if RR_DEBUG
#define LOGD(...) (__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))
#else
#define LOGD(...)
#endif

#undef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#undef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define CLAMP(a)  (MIN(255, MAX(0, (a))))

typedef void (*COMPRESS_METHOD)(int, int, int*, int*);

typedef struct {
	unsigned char r;  // [0, 255]
	unsigned char g;  // [0, 255]
	unsigned char b;  // [0, 255]
} RGB;

typedef struct {
	double h;   // [0, 360]
	double s;   // [0, 1]
	double l;   // [0, 1]
} HSL;

typedef struct {
	double h;   // [0, 360]
	double s;   // [0, 1]
	double v;   // [0, 1]
} HSV;

typedef struct {
	double h;
	double s;
	double i;
} HSI;

typedef struct {
	unsigned int width;	//宽
	unsigned int height; //高
	unsigned int channels;	//通道数
	unsigned int stride;	//一行字节数，一般为宽＊通道
	unsigned char *pixels;
	unsigned char type;	// 图片源为jpeg格式或png格式，TYPE_RRIMAGE_JPEG表示jpeg，TYPE_RRIMAGE_PNG表示png，TYPE_RRIMAGE_UNSPECIFIED表示未知
	unsigned char quality; // 图片质量
} rrimage;

typedef struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
}* my_error_ptr;

rrimage *init_rrimage();

void free_rrimage(rrimage *);

rrimage * clone_rrimage(rrimage *);

/**
 * RGBA数据转换为RGB数据，忽略A通道
 */
void strip_alpha(rrimage *);

void my_error_exit(j_common_ptr cinfo);

void my_output_message(j_common_ptr cinfo);

rrimage* read_jpeg(char *);

int write_jpeg(char *, rrimage *);

#ifdef __cplusplus
}
#endif

#endif
