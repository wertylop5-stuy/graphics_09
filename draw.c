//for qsort
#define _GNU_SOURCE

#include"include/draw.h"

void pixel_color(struct Pixel *p, unsigned char r,
		unsigned char g, unsigned char b) {
	p->r = r;
	p->g = g;
	p->b = b;
}

void plot_point(Frame grid, int x, int y, struct Pixel *p) {
	//printf("Plotting %d, %d (%d, %d)\n", x, IMG_HEIGHT-1-y, x, y);
	if (x > -1 && y > -1) {
		grid[IMG_HEIGHT-1-y][x] = *p;
	}
	//grid[y][x] = *p;	//if top left is (0, 0)
}

void draw_lines(Frame grid, struct Matrix *m, struct Pixel *p) {
	int x;
	for (x = 1; x < m->back; x+=2) {
		draw_line(grid, p,
				m->m[0][x-1], m->m[1][x-1],
				m->m[0][x], m->m[1][x]);
	}
}

void draw_line(Frame grid, struct Pixel *p, int x1, int y1, int x2, int y2) {
	//make sure x1 y1 is the left point
	if (x1 > x2) {
		int temp = x1;
		x1 = x2;
		x2 = temp;
		
		temp = y1;
		y1 = y2;
		y2 = temp;
	}
	int a = y2-y1;
	int b = -(x2-x1);
	int d;
	
	float m = ((float)a)/(-b);
	//printf("%d, %d %f\n", a, -b, m);
	
	//octant 1
	if (m >= 0 && m <= 1) {
		if (x1 > x2) {
			int temp = x1;
			x1 = x2;
			x2 = temp;
			
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		a = y2-y1;
		b = -(x2-x1);
		
		d = 2*a + b;
		//printf("1\n");
		
		while (x1 <= x2 && x1 < IMG_WIDTH) {
			plot_point(grid, x1, y1, p);
			
			if (d > 0) {
				y1++;
				d += 2*b;
			}
			
			x1++;
			d += 2*a;
		}
	}
	//octant 2
	else if (m > 1) {
		if (y1 > y2) {
			int temp = x1;
			x1 = x2;
			x2 = temp;
			
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		a = y2-y1;
		b = -(x2-x1);
		
		d = a + 2*b;
		//printf("2\n");
		
		while (y1 <= y2 && y1 < IMG_HEIGHT) {
			plot_point(grid, x1, y1, p);
			
			if (d < 0) {
				x1++;
				d += 2*a;
			}
			
			y1++;
			d += 2*b;
		}
	}
	//octant 8
	else if (m < 0 && m >= -1) {
		if (x1 > x2) {
			int temp = x1;
			x1 = x2;
			x2 = temp;
			
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		a = y2-y1;
		b = -(x2-x1);
		
		d = 2*a - b;
		//printf("8\n");
		
		while (x1 <= x2 && x1 < IMG_WIDTH) {
			plot_point(grid, x1, y1, p);
			
			if (d < 0) {
				y1--;
				d -= 2*b;
			}
			
			x1++;
			d += 2*a;
		}
	}
	//octant 7
	else if (m < -1) {
		if (y1 < y2) {
			int temp = x1;
			x1 = x2;
			x2 = temp;
			
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		a = y2-y1;
		b = -(x2-x1);
		
		d = a - 2*b;
		//printf("7\n");
		
		while (y1 >= y2 && y1 < IMG_HEIGHT) {
			plot_point(grid, x1, y1, p);
			
			if (d > 0) {
				x1++;
				d += 2*a;
			}
			
			y1--;
			d -= 2*b;
		}
	}
}

void find_norm(struct Matrix *m, int p1, int p2, int p3,
		float *norm_out) {
	//create vectors
	float tempA[] = {
		m->m[0][p2] - m->m[0][p1],
		m->m[1][p2] - m->m[1][p1],
		m->m[2][p2] - m->m[2][p1],
	};
	
	float tempB[] = {
		m->m[0][p3] - m->m[0][p1],
		m->m[1][p3] - m->m[1][p1],
		m->m[2][p3] - m->m[2][p1]
	};
	
	//cross product
	norm_out[0] = tempA[1]*tempB[2] - tempA[2]*tempA[1];
	norm_out[1] = tempA[2]*tempB[0] - tempA[0]*tempB[2];
	norm_out[2] = tempA[0]*tempB[1] - tempA[1]*tempB[0];
}

void draw_polygons(Frame f, struct Matrix *m, struct Pixel *p) {
	int x;
	float norm[3];
	struct Pixel fill_color;
	int r = 10, g = 34, b = 123;
	for (x = 0; x < m->back; x+=3) {
		find_norm(m, x, x+1, x+2, norm);
		//backface culling: don't draw if polygon
		//not in sight
		if (norm[2] > 0) {
			/*
			draw_line(f, p,
				m->m[0][x],
				m->m[1][x],
				m->m[0][(x+1)],
				m->m[1][(x+1)]
			);
			draw_line(f, p,
				m->m[0][(x+1)],
				m->m[1][(x+1)],
				m->m[0][(x+2)],
				m->m[1][(x+2)]
			);
			draw_line(f, p,
				m->m[0][(x+2)],
				m->m[1][(x+2)],
				m->m[0][x],
				m->m[1][x]
			);
			*/
			
			pixel_color(&fill_color, r, g, b);
			r += 50;
			g += 40;
			b += 25;
			r = r % 255;
			g = g % 255;
			b = b % 255;
			render_scanlines(f, m, &fill_color, x, x+1, x+2);
		}
	}
}

//matrix should be a pointer to a struct Matrix
int compare_matrix_indices(const void *p1, const void *p2, void *matrix) {
	struct Matrix *m = (struct Matrix *)matrix;
	//printf("comparing: %f, %f\n", m->m[1][*((int *)(p1))], m->m[1][*((int *)(p2))]);
	//printf("round: %f\n", m->m[1][*((int *)(p1))] - m->m[1][*((int *)p2)]);
	float res = m->m[1][*((int *)p1)] - m->m[1][*((int *)p2)];
	if (res < 0) return -1;
	else if (res > 0) return 1;
	return 0;
}

void render_scanlines(Frame f, struct Matrix *m, struct Pixel *p,
		int p1, int p2, int p3) {
	//determine top and bottom
	int hi, lo, mid;	//index of top, bottom, middle
	int sorted[] = {p1, p2, p3};
	qsort_r(sorted, 3, sizeof(int), *compare_matrix_indices, m);
	lo = sorted[0];
	mid = sorted[1];
	hi = sorted[2];
	
	float x0 = m->m[0][lo], x1 = m->m[0][lo];

	//how much to increment x by, d1 will change
	float 	d0 = (m->m[0][hi] - m->m[0][lo])/(m->m[1][hi] - m->m[1][lo]),
		d1 = (m->m[0][mid] - m->m[0][lo])/(m->m[1][mid] - m->m[1][lo]);
	int y;
	//printf("ylo: %d, ymid: %d, yhi: %d\n", (int)m->m[1][lo], (int)m->m[1][mid], (int)m->m[1][hi]);
	for (y = (int)m->m[1][lo]; y <= (int)m->m[1][hi]; y++) {
		//printf("x0: %f, x1: %f, y: %d, d0: %f, d1: %f\n", x0, x1, y, d0, d1);
		//always go from x0 to x1
		//x1 is always on the side dealing with the middle
		
		if (y == (int)m->m[1][hi] && y == (int)m->m[1][mid]) break;
		
		draw_line(f, p, (int)x0, y, (int)x1, y);
		//swap delta1 at midpoint
		if (y == (int)m->m[1][mid]) {
			x1 =  m->m[0][mid];
			d1 = (m->m[0][hi] - m->m[0][mid])/(m->m[1][hi] - m->m[1][mid]);
			//printf("swapped d1: %f\n", d1);
		}
		
		x0 += d0;
		x1 += d1;
	}
}


