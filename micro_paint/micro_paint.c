#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

size_t		len(char *s)
{
    int    i = 0;
    while (s[i])
        i++;
    return (i);
}

int        errror(char *s)
{
    write(1, s, len(s));
    return (1);
}

int        checkpos(float x, float y, float start_x, float start_y, float rwidth, float rheight)
{
    if (x < start_x || x > start_x + rwidth || y < start_y || y > start_y + rheight)
        return (0);
    else if (x - start_x < 1 || start_x + rwidth - x < 1 || y - start_y < 1 || start_y + rheight - y < 1)
        return (1);
    return (2);
}

int        main(int argc, char *argv[])
{
    FILE    *file;
    char    *paper, bg_color, type, filling;
    int        read, pos, x, y, total_width, total_height;
    float    start_x, start_y, rwidth, rheight;

    if (argc != 2)
        return (errror("Error: argument\n"));
    if (!(file = fopen(argv[1], "r")) ||
        (fscanf(file, "%i %i %c\n", &total_width, &total_height, &bg_color) != 3) ||
        total_width <= 0 || total_width > 300 || total_height <= 0 || total_height > 300 ||
        (!(paper = (char *)malloc(sizeof(char) * (total_width * total_height)))))
        return (errror("Error: Operation file corrupted\n"));
    memset(paper, bg_color, total_width * total_height);
    while ((read = fscanf(file, "%c %f %f %f %f %c\n", &type, &start_x, &start_y, &rwidth, &rheight, &filling)) == 6)
    {
        if (!(rwidth > 0 && rheight > 0) || !(type == 'R' || type == 'r'))
            break ;
        x = -1;
        while (++x < total_width)
        {
            y = -1;
            while (++y < total_height)
            {
                pos = checkpos((float)x, (float)y, start_x, start_y, rwidth, rheight);
                if (pos == 1 || (pos == 2 && type == 'R'))
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
    return (0);
}
