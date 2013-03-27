package com.jayway.opengl.tutorial;

public class Data {
	int w;
	int h;

	byte[] yuv;
	byte[] y;
	byte[] u;
	byte[] v;
	byte[] uv;
	int[] rgb;

	public Data(int w, int h, byte[] yuv) {
		this.w = w;
		this.h = h;
		this.yuv = yuv;

		rgb = new int[w * h];
		y = new byte[w * h];
		u = new byte[w * h / 4];
		v = new byte[w * h / 4];
		uv = new byte[w * h / 2];
		for (int i = 0; i < y.length; i++) {
			y[i] = yuv[i];
		}
		for (int i = 0; i < uv.length; i++) {
			uv[i] = yuv[w * h + i];
		}
		for (int i = 0, index = w * h; i < u.length; i++) {
			v[i] = yuv[index++];
			u[i] = yuv[index++];
		}

		int yy = 0, uu = 0, vv = 0;
		int r, g, b;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				yy = (0xFF & (int) y[i * w + j]) - 16;
				yy = yy < 0 ? 0 : yy;
				if (j % 2 == 0) {
					// vv = (0xFF & uv[i / 2 * w + j]) - 128;
					// uu = (0xFF & uv[i / 2 * w + j + 1]) - 128;

					vv = (0xFF & v[i / 2 * w / 2 + j / 2]) - 128;
					uu = (0xFF & u[i / 2 * w / 2 + j / 2]) - 128;
				}

				int y1192 = 1192 * yy;
				r = (y1192 + 1634 * vv);
				g = (y1192 - 833 * vv - 400 * uu);
				b = (y1192 + 2066 * uu);

				if (r < 0)
					r = 0;
				else if (r > 262143)
					r = 262143;
				if (g < 0)
					g = 0;
				else if (g > 262143)
					g = 262143;
				if (b < 0)
					b = 0;
				else if (b > 262143)
					b = 262143;

				rgb[i * w + j] = (0xff000000 | ((r << 6) & 0xff0000)
						| ((g >> 2) & 0xff00) | ((b >> 10) & 0xff));
			}
		}
	}

}
