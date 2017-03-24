#ifndef BSAC_H
#define BSAC_H

struct pattern
{
	uint8_t* s; // each int should represent a bit, that is, 0 or 1 
	unsigned long size; // in bit, [!] MUST align to granularity
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
	void (*found_callback)(struct task* t, unsigned long index, unsigned long offset);

	/* Callback function called each time scanner finish
	 * scanning a block. Should provide the pointer to the
	 * next block. [!] it is assumed that the main
	 * binary stream start/continues from the first bit of the first
	 * byte of each block.*/
	(void*) (*next_block_callback)(struct task* t);

	/* size of each binary block, in bit, don't have to align to byte
	 * [!] MUST align to granularity */
	unsigned long block_size;

	/* pointer to the first binary block */
	void* first_block;

	/* total size of the main binary stream, in bit */
	unsigned long total_size;

	/* number of pattern instructions */
	unsigned long pattern_number;

	/* array containing all pattern instructions */
	struct pattern* plist;

	/* literally granularity of scanning, in bit
	 * please don't set this value too high
	 * the memory consumption of AC algorithm is
	 * O(total charachters in patterns*2^granularity) */
	unsigned long granularity;
};

/* no sanity check, so ignore return value for now */
int do_task(struct task t);

#endif
