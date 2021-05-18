#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

size_t	len(char *str)
{
	size_t i = 0;
	while (str[i])
		i++;
	return (i);	
}

int	errror(char *str)
{
	write(1, str, len(str));
	return (1);
}

int		checkpos(float center_x, float center_y, float posx, float posy, float radius)
{
	float distance = sqrtf(powf(posx - center_x, 2) + powf(posy - center_y, 2));
	if (distance <= radius)
	{
		if ((radius - distance) < 1)
			return (0);
		return (1);
	}
	return (2);
}

int		main (int argc, char **argv)
{
	FILE *file;
	char *paper, bg_color, type, filling;
	int total_width, total_height, read, pos, x, y;
	float center_x, center_y, radius;

	if (argc != 2)
		return (errror("Error: argument\n"));
	if (!(file = fopen(argv[1], "r")) || fscanf(file, "%i %i %c\n", &total_width, &total_height, &bg_color) != 3 || total_height <= 0 || total_width <= 0 || total_height > 300 || total_width > 300 || !(paper = (char *)malloc(sizeof(char) * total_height * total_width)))
		return (errror("Error: Operation file corrupted\n"));
	memset(paper, bg_color, total_width * total_height);
	while ((read = fscanf(file, "%c %f %f %f %c\n", &type, &center_x, &center_y, &radius, &filling)) == 5)
	{
		if (radius <= 0 || (type != 'C' && type != 'c'))
			break;
		x = -1;
		while (++x < total_width)
		{
			y = -1;
			while(++y < total_height)
			{
				pos = checkpos(center_x, center_y, (float)x, (float)y, radius);
				if (pos == 0 || (pos == 1 && type == 'C'))
					paper[y * total_width + x] = filling;
			}
		}
	}
	if (read != -1)
	{
		free(paper);
		return (errror("Error: Operation file corrupted\n"));
	}
	y = -1;
	while (++y < total_height)
	{
		write(1, paper + y * total_width, total_width);
		write(1, "\n", 1);
	}
	free(paper);
	fclose(file);
	return(0);
}