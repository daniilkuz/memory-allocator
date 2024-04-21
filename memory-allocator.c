

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
	}

}

