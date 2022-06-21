#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

typedef struct s_back
{
	int w;
	int h;
	char c;

}              t_back;

typedef struct s_draw
{
	char type;
	float x;
	float y;
	float r;
	char c;

}              t_draw;

int print_error(char *str)
{
	int i =-1;
	while(str[++i])
		;
	write(1,str,i);
	write(1,"\n",1);
	return 1;
}

void print_all(t_back back,char *canvas)
{
	int i =-1;
	while(++i < back.h)
    {
        write(1, canvas + i * back.w, back.w);
        write(1, "\n",1);
    }
}

int get_back(FILE *file, t_back *back)
{
	if(fscanf(file,"%d %d %c \n", &back->w, &back->h, &back->c)!= 3)
		return 0;
	if(back->w <= 0 || back->w > 300 ||back->h <= 0 || back->h > 300)
		return 0;
	return 1;
}

char * draw_back( t_back back)
{
	char * canvas = malloc(back.w * back.h);
	int i = -1;
	while(++i < back.w * back.h)
	{
		canvas[i] = back.c;
	}
	return canvas;
}
int is_incircle(float x, float y, t_draw draw )
{
	float dist = sqrtf(powf(x-draw.x , 2) + powf(y-draw.y, 2));
	if(dist<= draw.r)
	{
		if(draw.r - dist <1.00000000)
			return 2;
		return 1;
	}
	return 0;
}

void draw_circle(char * canvas, t_back back,t_draw draw )
{
	int x;
	int y = -1;
	while(++y < back.h)
	{
		x = -1;
		while(++x < back.w)
		{
			int is_in = is_incircle(x, y, draw );

			if((draw.type == 'c' &&is_in == 2) || (draw.type == 'C' &&is_in > 0  ))
				canvas[y * back.w + x] = draw.c;
		}
	}

}
int draw_circles(FILE *file, t_back back, char * canvas)
{
	t_draw draw;
	int ret;
	while((ret = fscanf(file,"%c %f %f %f %c \n", &draw.type, &draw.x, &draw.y, &draw.r, &draw.c))== 5)
	{
		if(draw.r <= 0.00000000 || (draw.type != 'c' && draw.type != 'C' ))
			return 0;
		draw_circle(canvas,  back, draw );
	}
	if(ret != -1)
		return 0;
	return 1;
}

int free_all(FILE *file, char * canvas)
{
    if(file)
        fclose(file);
    if(canvas)
        free(canvas);
    return 1;
}

int main (int argc, char ** argv)
{
	FILE *file;
	t_back back;
	char * canvas = NULL;
	if(argc !=2)
		return print_error("Error: argument");
	if(!(file = fopen(argv[1],"r")))
		return print_error("Error: Operation file corrupted");
	if(!(get_back(file, &back)))
		return print_error("Error: Operation file corrupted")&& free_all(file, NULL);
	if(!(canvas = draw_back(back)))
		return print_error("Error: Operation file corrupted")&& free_all(file, NULL);
	if(!(draw_circles(file,  back, canvas)))
		return print_error("Error: Operation file corrupted")&& free_all(file, canvas);
    print_all(back,canvas);
    free_all(file, canvas);
    return 0;
} 
