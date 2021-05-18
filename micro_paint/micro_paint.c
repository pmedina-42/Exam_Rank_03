#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

size_t	len(char *s)
{
	int i = 0;
	while (s[i])
		i++;
	return (i);
}

int	errror(char *s)
{
	write(1, s, len(s));
	return (1);
}

int checkpos(float start_x, float start_y, float rwidth, float rheigth, float posx, float posy)
{
	if (posx < start_x || posx > start_x + rwidth || posy < start_y || posy > start_y + rheigth)
		return (0);
	else if ((posx - start_x) < 1 || (start_x + rwidth - posx) < 1 || (posy - start_y) < 1 || (start_y + rheigth - posy) < 1)
		return (1);
	return (2);
}

int	main(int argc, char **argv)
{
	FILE *file;
	char *paper, bg_color, filling, type;
	int x, y, total_width, total_heigth, pos, read;
	float start_x, start_y, rwidth, rheigth;

	if (argc != 2)
		return (errror("Error :argument\n"));
	if (!(file = fopen(argv[1], "r")) || (fscanf(file, "%i %i %c\n", &total_width, &total_heigth, &bg_color) != 3) || total_width <= 0 || total_heigth <= 0 || total_width > 300 || total_heigth > 300 || (!(paper = (char *)malloc(sizeof(char) * total_heigth * total_width))))
		return (errror("Error: Operation file corrupted\n"));
	memset(paper, bg_color, total_width * total_heigth);
	while((read = fscanf(file, "%c %f %f %f %f %c\n", &type, &start_x, &start_y, &rwidth, &rheigth, &filling)) == 6)
	{
		if ((type != 'R' && type != 'r') || rheigth < 1 || rwidth < 1)
			break;
		x = -1;
		while (++x < total_width)
		{
			y = -1;
			while (++y < total_heigth)
			{
				pos = checkpos(start_x, start_y, rwidth, rheigth, x, y);
				if ((pos == 1 && type == 'R') || pos == 0)
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
	while (++y < total_heigth)
	{
		write(1, paper + y * total_width, total_width);
		write(1, "\n", 1);
	}
	free(paper);
	fclose(file);
	return (0);
}