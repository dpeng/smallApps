#ifndef  _CONVERT_H
#define  _CONVERT_H

#define uint8_t		unsigned char
#define int8_t		signed char
#define uint16_t	unsigned short
#define int16_t		signed short
#define uint32_t	unsigned int
#define int32_t		signed int

static int params_bic[32][6] = {{0, -3, 256, 4,   -1,  0},  {1, -5,  255, 6,   -1,  0},
{1,  -9, 254, 12,  -2,  0},  {2, -13, 251, 19,  -3,  0},
{2, -17, 247, 27,  -4,  1},  {2, -19, 243, 36,  -6,  0},
{3, -22, 237, 45,  -8,  1},  {3, -24, 231, 54,  -9,  1},
{3, -25, 224, 64,  -11, 1},  {3, -25, 216, 74,  -13, 1},
{3, -27, 208, 86,  -15, 1},  {3, -27, 199, 95,  -16, 2},
{3, -27, 190, 106, -18, 2},  {3, -27, 181, 117, -20, 2},
{3, -26, 170, 128, -21, 2},  {3, -25, 160, 139, -23, 2},
{3, -24, 149, 149, -24, 3},  {2, -23, 139, 160, -25, 3},
{2, -21, 128, 170, -26, 3},  {2, -20, 117, 180, -26, 3},
{2, -18, 106, 190, -27, 3},  {2, -16, 95,  199, -27, 3},
{1, -15, 85,  208, -26, 3},  {1, -13, 75,  216, -26, 3},
{1, -11, 64,  224, -25, 3},  {1, -9,  54,  231, -24, 3},
{1, -8,  45,  237, -22, 3},  {0, -6,  36,  243, -19, 2},
{1, -4,  27,  247, -17, 2},  {0, -3,  19,  251, -13, 2},
{0, -2,  12,  254, -9,  1},  {0, -1,  6,   255, -5,  1}
};


static int params_uv[32][2] = {{0,   256}, {8,   248}, {16,  240}, {24,  232}, 
{32,  224},	{40,  216}, {48,  208}, {56,  200}, 
{64,  192}, {72,  184}, {80,  176}, {88,  168}, 
{96,  160}, {104, 152}, {112, 144},	{120, 136}, 
{128, 128}, {136, 120}, {144, 112}, {152, 104}, 
{160,  96}, {168,  88}, {176,  80}, {184,  72}, 
{192,  64}, {200,  56}, {208,  48}, {216,  40},
{224,  32}, {232,  24}, {240,  16}, {248,   8}
};

static int params_bil[32][4] ={{40, 176, 40, 0}, {36, 176, 43, 1},
{32, 175, 48, 1}, {30, 174, 51, 1},
{27, 172, 56, 1}, {24, 170, 61, 1},
{22, 167, 66, 1}, {19, 164, 71, 2},
{17, 161, 76, 2}, {15, 157, 82, 2},
{14, 152, 87, 3}, {12, 148, 93, 3},
{11, 143, 99, 3}, {9, 138, 105, 4},
{8, 133, 110, 5}, {7, 128, 116, 5},
{6, 122, 122, 6}, {5, 117, 127, 7},
{5, 110, 133, 8}, {4, 105, 138, 9},
{3, 99, 143, 11}, {3, 93, 148, 12},
{3, 87, 152, 14}, {2, 82, 157, 15},
{2, 76, 161, 17}, {2, 71, 164, 19},
{1, 66, 167, 22}, {1, 61, 170, 24},
{1, 56, 172, 27}, {1, 52, 173, 30},
{1, 47, 175, 33}, {0, 44, 176, 36}
};


static void YResizeCubic(unsigned char* ptr_in, unsigned char* ptr_rz, 
				  int old_rows, int old_cols, int rsz_rows, int rsz_cols)
{
	unsigned char* ptr_temp;
	unsigned char* ptr_line;
	ptr_temp = (unsigned char*)malloc((old_rows + 6) * rsz_cols);
	ptr_line = (unsigned char*)malloc(old_cols + 6);

	int i, j, m, idx, tmp_data;
	int* ptr_flt;
	unsigned long ratio;

	ratio = old_cols * 1024 / rsz_cols;

	for(i = 0; i < old_rows; i++)
	{
		memcpy(ptr_line + 3, ptr_in + i * old_cols, old_cols);
		memset(ptr_line, ptr_in[i * old_cols], 3);
		memset(ptr_line + old_cols + 3, ptr_in[(i + 1) * old_cols - 1], 3);

		for(j = 0; j < rsz_cols; j++)
		{
			idx     = ((j * ratio) % 1024 * 32) / 1024;
			ptr_flt = params_bic[idx];

			idx      = j * ratio / 1024 + 3;
			tmp_data = 0;
			for(m = 0; m < 6; m++)
				tmp_data += ptr_line[idx + m - 2] * ptr_flt[m];

			tmp_data /= 256;
			if(tmp_data < 0) tmp_data = 0;
			if(tmp_data > 255) tmp_data = 255;
			ptr_temp[(i + 3) * rsz_cols + j] = (unsigned char)tmp_data;
		}
	}

	memcpy(ptr_temp, ptr_temp + 3 * rsz_cols, rsz_cols);
	memcpy(ptr_temp + rsz_cols, ptr_temp + 3 * rsz_cols, rsz_cols);
	memcpy(ptr_temp + rsz_cols * 2, ptr_temp + 3 * rsz_cols, rsz_cols);
	memcpy(ptr_temp + rsz_cols * (old_rows + 3), ptr_temp + (old_rows + 2) * rsz_cols, rsz_cols);
	memcpy(ptr_temp + rsz_cols * (old_rows + 4), ptr_temp + (old_rows + 2) * rsz_cols, rsz_cols);
	memcpy(ptr_temp + rsz_cols * (old_rows + 5), ptr_temp + (old_rows + 2) * rsz_cols, rsz_cols);

	ratio = old_rows * 1024 / rsz_rows;
	for(j = 0; j < rsz_cols; j++)
		for(i = 0; i < rsz_rows; i++)
		{
			idx     = ((i * ratio) % 1024 * 32) / 1024;
			ptr_flt = params_bic[idx];

			idx      = i * ratio / 1024 + 3;
			tmp_data = 0;
			for(m = 0; m < 6; m++)
				tmp_data += ptr_temp[(idx + m - 2) * rsz_cols + j] * ptr_flt[m];
			tmp_data /= 256;
			if(tmp_data < 0) tmp_data = 0;
			if(tmp_data > 255) tmp_data = 255;
			ptr_rz[i * rsz_cols + j] = (unsigned char)tmp_data;
		}

		free(ptr_temp);
		free(ptr_line);
}

////////////////////////////////////////////////////////////////////////////
// U and V component resize using bilinear interpolation
static void UVResize(unsigned char* ptr_in, unsigned char* ptr_rz, 
			  int old_rows, int old_cols, int rsz_rows, int rsz_cols)
{
	unsigned char* ptr_temp;
	unsigned char* ptr_line;
	ptr_temp = (unsigned char*)malloc((old_rows + 2) * rsz_cols);
	ptr_line = (unsigned char*)malloc(old_cols + 2);

	int i, j, idx, tmp_data;
	int* ptr_flt;
	unsigned long ratio;

	ratio = 1024 * old_cols / rsz_cols;
	for(i = 0; i < old_rows; i++)
	{
		memcpy(ptr_line + 1, ptr_in + i * old_cols, old_cols);
		ptr_line[0] = ptr_line[1];
		ptr_line[old_cols + 1] = ptr_line[old_cols];

		for(j = 0; j < rsz_cols; j++)
		{
			idx     = ((j * ratio) % 1024 * 32) / 1024;
			ptr_flt = params_uv[idx];

			idx      = j * ratio / 1024 + 1;
			tmp_data = (ptr_line[idx] * ptr_flt[0] + ptr_line[idx + 1] * ptr_flt[1]) / 256;
			if(tmp_data > 255) tmp_data = 255;
			ptr_temp[(i + 1) * rsz_cols + j] = (unsigned char)tmp_data;
		}
	}
	memcpy(ptr_temp, ptr_temp + rsz_cols, rsz_cols);
	memcpy(ptr_temp + rsz_cols * (old_rows + 1), ptr_temp + old_rows * rsz_cols, rsz_cols);	

	ratio = 1024 * old_rows / rsz_rows;
	for(j = 0; j < rsz_cols; j++)
		for(i = 0; i < rsz_rows; i++)
		{
			idx     = ((i * ratio) % 1024 * 32) / 1024;	
			ptr_flt = params_uv[idx];

			idx      = i * ratio / 1024 + 1;
			tmp_data = (ptr_temp[idx * rsz_cols + j] * ptr_flt[0] + 
				ptr_temp[(idx + 1) * rsz_cols + j] * ptr_flt[1]) / 256;
			if(tmp_data > 255) tmp_data = 255;
			ptr_rz[i * rsz_cols + j] = (unsigned char)tmp_data;
		}

		free(ptr_temp);
		free(ptr_line);
}


#define RGB_Y_OUT		1.164
#define B_U_OUT			2.018
#define Y_ADD_OUT		16

#define G_U_OUT			0.391
#define G_V_OUT			0.813
#define U_ADD_OUT		128

#define R_V_OUT			1.596
#define V_ADD_OUT		128


#define SCALEBITS_OUT	13
#define FIX_OUT(x)		((uint16_t) ((x) * (1L<<SCALEBITS_OUT) + 0.5))

static int32_t RGB_Y_tab[256];
static int32_t B_U_tab[256];
static int32_t G_U_tab[256];
static int32_t G_V_tab[256];
static int32_t R_V_tab[256];

static BYTE str_zgf[800*3];
static BYTE g_rgb24[720*576*3];

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) > (b) ? (b) : (a))
/* yuv 4:2:0 planar -> rgb24 */


static void colorspace_init(void) {
	int32_t i;

	for(i = 0; i < 256; i++) {
		RGB_Y_tab[i] = FIX_OUT(RGB_Y_OUT) * (i - Y_ADD_OUT);
		B_U_tab[i] = FIX_OUT(B_U_OUT) * (i - U_ADD_OUT);
		G_U_tab[i] = FIX_OUT(G_U_OUT) * (i - U_ADD_OUT);
		G_V_tab[i] = FIX_OUT(G_V_OUT) * (i - V_ADD_OUT);
		R_V_tab[i] = FIX_OUT(R_V_OUT) * (i - V_ADD_OUT);
	}
}

static void yv12_to_rgb24_c(uint8_t *dst, int dst_stride,
					 uint8_t *y_src, uint8_t *u_src, uint8_t * v_src, 
					 int y_stride, int uv_stride,
					 int width, int height)
{
	static int zgf_i = 0;
	if (zgf_i == 0)
	{
		colorspace_init();
		zgf_i = 1;
	}

	unsigned char* zgf_dst = dst;

	const uint32_t dst_dif = 6 * dst_stride - 3 * width;
	int32_t y_dif = 2 * y_stride - width;

	uint8_t *dst2 = dst + 3 * dst_stride;
	const uint8_t *y_src2 = y_src + y_stride;
	uint32_t x, y;

	if (height < 0) { // flip image?
		height = -height;
		y_src += (height - 1) * y_stride;
		y_src2 = y_src - y_stride;
		u_src += (height / 2 - 1) * uv_stride;
		v_src += (height / 2 - 1) * uv_stride;
		y_dif = -width - 2 * y_stride;
		uv_stride = -uv_stride;
	}

	for (y = height / 2; y; y--) 
	{
		// process one 2x2 block per iteration
		for (x = 0; x < (uint32_t)width / 2; x++)
		{
			int u, v;
			int b_u, g_uv, r_v, rgb_y;
			int r, g, b;

			u = u_src[x];
			v = v_src[x];

			b_u = B_U_tab[u];
			g_uv = G_U_tab[u] + G_V_tab[v];
			r_v = R_V_tab[v];

			rgb_y = RGB_Y_tab[*y_src];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst[0] = MAX(0,MIN(255, b));
			dst[1] = MAX(0,MIN(255, g));
			dst[2] = MAX(0,MIN(255, r));

			y_src++;
			rgb_y = RGB_Y_tab[*y_src];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst[3] = MAX(0,MIN(255, b));
			dst[4] = MAX(0,MIN(255, g));
			dst[5] = MAX(0,MIN(255, r));
			y_src++;

			rgb_y = RGB_Y_tab[*y_src2];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst2[0] = MAX(0,MIN(255, b));
			dst2[1] = MAX(0,MIN(255, g));
			dst2[2] = MAX(0,MIN(255, r));
			y_src2++;

			rgb_y = RGB_Y_tab[*y_src2];
			b = (rgb_y + b_u) >> SCALEBITS_OUT;
			g = (rgb_y - g_uv) >> SCALEBITS_OUT;
			r = (rgb_y + r_v) >> SCALEBITS_OUT;
			dst2[3] = MAX(0,MIN(255, b));
			dst2[4] = MAX(0,MIN(255, g));
			dst2[5] = MAX(0,MIN(255, r));
			y_src2++;

			dst += 6;
			dst2 += 6;
		}

		dst += dst_dif;
		dst2 += dst_dif;

		y_src += y_dif;
		y_src2 += y_dif;

		u_src += uv_stride;
		v_src += uv_stride;
	}

	for (int i = 0; i < height/2; i++)
	{
		memcpy(str_zgf, zgf_dst + i*width*3, width*3);
		memcpy(zgf_dst+i*width*3, zgf_dst+(height-i-1)*width*3, width*3);
		memcpy(zgf_dst+(height-1-i)*width*3, str_zgf, width*3);
	}

}
#endif