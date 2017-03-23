#ifndef BSAC_H
#define BSAC_H

struct pattern
{
	char* s; // each character should represent a bit, that is, 0 or 1
	unsigned long size; // in bit
};

struct task
{
	/* Function for allocating memory for internal use. 
	 * If you don't have special need, then use `malloc` for user
	 * program and `kmalloc` for kernel program. */
	(void*) (*malloc)(unsigned long size);

	/* Function for releasing memory allocated by malloc.
	 * If you don't have special need, then use `free` for user
	 * program and `kfree` for kernel program. */
	void (*free)(void* ptr);

	/* Callback function called each time a matching is found */
	void (*found_callback)(unsigned long index, unsigned long offset);

	/* Callback function called each time scanner finish
	 * scanning a block. Should provide the pointer to the
	 * next block. [IMPORTATNT] it is assumed that the main
	 * binary stream start/continues from the first bit of the first
	 * byte of each block.*/
	(void*) (*next_block_callback)(void);

	/* size of each binary block, in bit, don't have to align to byte */
	unsigned long block_size;

	/* pointer to the first binary block */
	void* first_block;

	/* total size of the main binary stream, in bit */
	unsigned long total_size;

	/* number of pattern instructions */
	unsigned long pattern_number;

	/* array containing all pattern instructions */
	struct pattern* plist;

	/* literaly granularity of scanning, in bit */
	unsigned long granularity;
};

/* no sanity check, so ignore return value for now */
int do_task(struct task t);

#endif
