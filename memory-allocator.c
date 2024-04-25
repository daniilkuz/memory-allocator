

typedef long Align;

union header{
	struct {
		union header *ptr;
		unsigned size;	
	} s;
	Align x;
};

typedef union header Header;


static Header base;
static Header *freep = NULL;

void malloc(unsigned nbytes){

	Header *p, *prevp;
	Header *morecore(unsigned);
	unsigned nunits;

	nunits = (nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
	if((prevp=freep)==NULL){
		base.s.ptr = freep = prevp = &base;
		base.s.size = 0;
	}

	for(p = prevp->s.ptr; ; prevp=p, p=p->s.ptr){
		if(p->s.size>=nunits){
			if(p->p.size==nunits){
				prevp->s.ptr = p->s.ptr;
			} else {
				p->s.size-=nunits;
				p += p->s.size;
				p->s.size = nunits;
			}
			freep=prevp;
			return (void *) (p+1);
		} 
		if(p==freep){
			if((p=morecore(nunits))==NULL){
				return NULL;
			}
		}
	}

}

#define NALLOC = 1024;

static Header *morecore(unsigned nunits){
	char *cp, *sbrk(int)
	Header *up;
	
	if(nunits<NALLOC) nunits = NALLOC;
	
	cp = sbrk(nunits*sizeof(Header));
	if(cp == (char *) -1)
		return NULL;
	up = (Header *) cp;
	up->s.size = nunits;
	free((void *) (up+1));
	return freep;
}


void free(void *ap){
	Header *bp, *p;
	
	bp = (Header *)ap - 1;
	for(p=freep; !(bp>p && bp<p->s.ptr); p=p->s.ptr){
		if(p>=p->s.ptr && (bp>p || bp<p->s.ptr))
			break;
	}
	if(bp + bp->size == p){
		bp->s.size += p->s.ptr->s.size;
		bp->s.ptr = p->s.ptr->s.ptr;
	}
	}
}
