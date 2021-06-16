#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int copy_read_write(int fromfile, int tofile);
int copy_mmap(int fromfile, int tofile);
void showhelp();

int main (int argc, char **argv){
    char c;
    int fromfile = 0;
    int tofile = 0;
    int returnval;
    int (*function_to_use)(int, int) = copy_read_write; // Default mode

    if (argc == 1)
    {   //no arguments
        showhelp();
        return 0;
    }
	while (1)
    {
        c = getopt (argc, argv, "hm");

        if (c == -1) {
            break;
        }

        // Otherwise process the option
        if (c == 'm')
        {
            function_to_use = copy_mmap;
        }

        else if (c == 'h')
        {
            showhelp();
            return 0;
        }

        else
        {
            showhelp();
            return 0;
        }

	}

    fromfile = open(argv[optind], O_RDONLY);
    if (fromfile < 0)
    {   //fail of opening file
        fprintf(stderr,"\nError while opening input file!\n");
        return 1;
    }
    tofile = open(argv[optind+1], O_TRUNC | O_CREAT | O_RDWR, 0666); // for linux 0666 is flag
    if (tofile < 0)
    {   //fail of opening file
        fprintf(stderr,"\nError while creating output file!\n");
        return 1;
    }

    returnval = function_to_use(fromfile, tofile);
    if (close(fromfile))
    {
        fprintf(stderr,"\nError while closing input file!\n");
        return 1;
    }

    if (close(tofile))
    {
        fprintf(stderr,"\nERROR: Couldn't close the input file!\n");
        return 1;
    }

    return returnval;
}

int copy_read_write (int fromfile, int tofile)
{
	size_t copiedbuf;
	char buffer[1024]; //1mb buffer
    do
    {
        copiedbuf = read(fromfile, &buffer, sizeof(buffer));
        if (copiedbuf < 0)
        {
		    fprintf(stderr,"\nError while reading input file!\n");
            return 1;
        }
        if (write(tofile, &buffer, copiedbuf) < 0)
        {
            fprintf(stderr,"\nError while writing output file!\n");
            return 1;
        }
    } while (copiedbuf > 0);
    return 0;
}

int copy_mmap (int fromfile, int tofile)
{
	struct stat st;
	void *map_in, *map_out;

	if (fstat(fromfile, &st) < 0)
    {
		fprintf(stderr,"\nError when fstat input file!\n");
		return 1;
	}

	ftruncate(tofile, st.st_size);

    map_in = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fromfile, 0);

	if (map_in == MAP_FAILED)
    {
		fprintf(stderr,"\nError when mmap the input file!\n");
		return 1;
    }

    map_out = mmap(0, st.st_size, PROT_WRITE, MAP_SHARED, tofile, 0);

	if (map_out == MAP_FAILED)
    {
		fprintf(stderr,"\nError when mmap the output file!\n");
		return 1;
    }

	if (memcpy(map_out, map_in, st.st_size) == NULL)
    {
		fprintf(stderr,"\nError - Couldn't memcpy()!\n");
		return 1;
    }


	if (munmap(map_in, st.st_size))
    {
		fprintf(stderr,"\nERROR: munmap for the input file failed!\n");
		return 1;
    }

	if (munmap(map_out, st.st_size))
    {
		fprintf(stderr,"\nERROR: munmap for the output file failed!\n");
		return 1;
    }
}

void showhelp ()
{
    printf("Help:\n\n"
            "1. copy <file_name> <new_file_name>:"
            "\tCopy file with read() and write ()\n"
            "2. copy -m <file_name> <new_file_name>:"
            "\tCopy file with mmap() and memcpy ()\n"
            "3. copy -h:"
            "\t print help text\n"
            "\n");
}
