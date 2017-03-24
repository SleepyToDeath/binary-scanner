#include "ac.h"

struct node
{
	uint8_t is_tail;
	unsigned long index;
	struct node** sub;
	struct node* fail;
	struct node* next;
//	unsigned long c;
};

struct task_data
{
	struct node* root;
	unsigned long alpha; // size of alphabet
	struct task* t;
	char* current_block;
	unsigned long block_offset;
	unsigned long total_offset;
};

static struct node* alloc_node(struct task_data* t)
{
	struct node* n = (struct node*)t->t->malloc( sizeof(struct node) );
	n->sub = (struct node**)t->t->malloc( sizeof(void*) * t->alpha );

	unsigned long i;
	struct node* r;
	if (t->root)
		r = t->root;
	else
		r = n;
	for (i=0; i<t->alpha; i++)
		n->sub[i] = r;
	n->is_tail = 0;
	n->index = 0;
	n->fail = 0;
	n->next = 0;

	return n;
}

static void free_node(struct task_data* t, struct node* n)
{
	t->t->free( (void*)(n->sub) );
	t->t->free( (void*)n );
}

static void init_trie(struct task_data* t)
{
	t->root = alloc_node(t);

	struct node* n;
	struct node* r = t->root;
	struct pattern* p;
	unsigned long i,j,k;
	unsigned long g = t->t->granularity;
	for(i=0; i<t->t->pattern_number; i++)
	{
		n = r;
		p = t->t->plist + i;
		for (j=0; j<p->size/g; j++)
		{
			unsigned long c = 0;
			for (k=0; k<g; k++)
				c += (p->s[j*g+k]) << k;

			if (n->sub[c] == r)
				n->sub[c] = alloc_node(t);

			n = n->sub[c];
		}
		n->is_tail = 1;
	}
}

static void destroy_trie(struct task_data* t)
{
	struct node* cur = NULL;
	struct node* next = t->root;
	while(next)
	{
		cur = next;
		next = cur->next;
		free_node(t, cur);
	}
}

static void init_fail_node(struct node* p, struct task_data* t)
{
	int i;
	struct node* f;
	struct node* r = t->root;
	for (i=0;i<=t->alpha;i++)
	{
		if (p->sub[i]!=r)
		{
			f = p->fail;
			while ((f!=r) && (f->sub[i]==r))
				f = f->fail;
			p->sub[i]->fail = f->sub[i];
		}
	}
}

static void init_fail(struct task_data* t)
{
	struct node* r = t->root;
	struct node* cur;
	struct node* tail;
	unsigned long i;

	r->fail = r;
	cur = r;
	tail = r;
	while( cur )	// bfs
	{
		init_fail_node( cur, t );

		for (i=0; i<t->alpha; i++)
			if (cur->sub[i]!=r)
			{
				tail->next = cur->sub[i];
				tail = cur->sub[i];
			}
		cur = cur->next;
	}
}

static struct task_data* init_task_data(struct task* t)
{
	struct task_data* td = (struct task_data*) t->malloc(sizeof(struct task_data));
	td->t = t;
	td->root = 0;
	td->alpha = 1 << (t->granularity)
	td->current_block = t->first_block;
	td->block_offset = 0;
	td->total_offset = 0;
	return td;
}

static void destroy_task_data(struct task_data* t)
{
	t->t->free(t);
}

static unsigned long next_char(struct task_data* t)
{
	unsigned long i;
	unsigned long c;

	if ( t->block_offset >= t->t->block_size )
	{
		current_block = next_block_callback(t->t);
		t->block_offset = 0;
	}

	c = 0;
	for (i=0; i<t->t->granularity; i++)
	{
		unsigned long byte_offset = (t->block_offset+i) / 8;
		unsigned long bit_offset = (t->block_offset+i) % 8;
		char tmp = ((char*)t->current_block)[byte_offset];
		tmp = tmp & (1<<bit_offset);
		tmp = !(!tmp);
		c += tmp << i;
	}
	t->block_offset += t->t->granularity;
	t->total_offset += t->t->granularity;

	return c;
}

static void run(struct task_data* t, struct node** np)
{
	unsigned long c = next_char(t);
	struct node* n = *np;
	struct node* next = NULL;
	struct node* r = t->root;
	do
	{
		if (n->tail)
			t->t->found_callback(t->t, n->index, t->total_offset-1);
		if ((!next) && (n->sub[c]!=r))
			next = n->sub[c];
		n = n->fail;
	}
	while (n!=r)
	if (!next)
		next = r;
	*np = next;
}

int do_task(struct task* t)
{
	unsigned long i;
	struct node** cur;
	struct task_data* td;

	td = init_task_data(t);
	init_trie(td);
	init_fail(td);

	*cur = td->root;
	for(i=0; i<t->total_size; i++)
		run(td, cur);
	
	destroy_trie(td);
	destroy_task_data(td);
}


