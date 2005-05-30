#include <stdio.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {

    char buf1[80];
    char buf2[80];
    FILE *f1 = fopen(argv[1], "r");
    FILE *f2 = fopen(argv[2], "r");
    int vf1, vf2;
    float fldummy;
    char dummy[80];
    int x = 0;
    int y = 0;

    while (fgets(buf1, sizeof(buf1), f1) && fgets(buf2, sizeof(buf2), f2)) {
        sscanf(buf1, "%s\t%d\%f", &dummy, &vf1, &fldummy);
        sscanf(buf2, "%s\t%d", &dummy, &vf2);
        if (vf1 == vf2) y++;
        else x++;
        fprintf(stderr, "%d\t%d\n", x, y);
    }

    return 0;
}
