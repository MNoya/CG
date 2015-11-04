#include "mm.h"
#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#endif //WIN32

static long num_allocs = 0;

void* cg_malloc(size_t size)
{
	void* ptr = malloc(size);

	if (ptr)
	{
		// Prevent overflow
		if (num_allocs + 1 > num_allocs)
		{
			++num_allocs;
		}
	}
	else
	{
		fprintf(stderr, "Warning: Bad Alloc (heap possibly exhausted!)\n");
	}

	return ptr;
}

void* cg_realloc(void* ptr, size_t size)
{
	return realloc(ptr, size);
}

void cg_free(void* ptr)
{
	if (ptr)
	{
		free(ptr);
		--num_allocs;
	}
	else
	{
		fprintf(stderr, "Warning: Attempted to free a NULL pointer\n");
	}
}

void cg_memcheck(void)
{
	if (num_allocs != 0)
	{
		fprintf(stderr, "Warning: Number of allocs does not match"
		" number of deallocs (leaked %ld objects)\n", num_allocs);
		
#ifdef WIN32
		char* message = (char *)malloc(255);
		snprintf(message, 255, "Se detectaron al menos (%ld) fuga(s) de memoria en su programa.", num_allocs);
		MessageBoxA(NULL, message, "Fugas de Memoria (Memory Leaks)", MB_ICONERROR|MB_OK);
		free(message);
#endif //WIN32
	}
}

