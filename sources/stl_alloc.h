//
//  stl_alloc.h
//
//
//  Created by deng on 14/11/12.
//
//

#ifndef _stl_alloc_h
#define _stl_alloc_h

namespace TinySTL {
    /*
     malloc-based allocator
     */
    class malloc_alloc{
        // oom = out of memory
    private:
        static void* oom_malloc(size_t);
        static void* oom_realloc(void*, size_t);
        typedef void (* _oom_handler) (void);
        static _oom_handler _malloc_alloc_oom_handler;
    public:
        
        static void* allocate(size_t n) {
            void* ret = malloc(n);
            if (ret == 0) {
                ret = oom_malloc(n);
            }
            return ret;
        }
        
        static void* reallocate(void* p, size_t n) {
            void* ret = realloc(p, n);
            if (ret == 0) {
                ret = oom_realloc(p,n);
            }
            return ret;
        }
        
        static void deallocate(void* p, size_t) {
            free(p);
        }
        // do something like std::set_new_handler
        static _oom_handler set_malloc_handler(_oom_handler new_handler) {
            _oom_handler old_handler = _malloc_alloc_oom_handler;
            _malloc_alloc_oom_handler = new_handler;
            return old_handler;
        }
    };
    
    malloc_alloc::_oom_handler malloc_alloc::_malloc_alloc_oom_handler = 0;
    
    void* malloc_alloc::oom_malloc(size_t n) {
        void* ret;
        for (; ; ) {
            if (0 == _malloc_alloc_oom_handler) {
                std::cerr << "out of memory" << std::endl;
                exit(1);
            }
            (*_malloc_alloc_oom_handler)();
            ret = malloc(n);
            if (ret) {
                return ret;
            }
        }
    }
    
    void* malloc_alloc::oom_realloc(void* p, size_t n) {
        void* ret;
        for (; ; ) {
            if (0 == _malloc_alloc_oom_handler) {
                std::cerr << "out of memory" << endl;
            }
            (*_malloc_alloc_oom_handler)();
            ret = realloc(p,n);
            if (ret) {
                return ret;
            }
        }
    }
    
    enum {_ALIGN = 8};
    enum {_MAX_BYTES = 128};
    enum {_NFREELISTS = _MAX_BYTES/_ALIGN};
    
    class default_alloc{
    private:
        union obj{
            union obj* free_list_link;
            char _data[1];
        };
        
        static size_t ROUND_UP(size_t n) {
            return ( (n + _ALIGN -1) & (_ALIGN-1) );
        }
        
        static obj* volatile free_list[_NFREELISTS];
        
        static size_t FREELIST_INDEX(size_t n) {
            return ( (n+_ALIGN-1) / _ALIGN-1);
        }
        
        static void* refill(size_t n);
        static char* chunk_alloc(size_t size, int& nobjs);
        
        // Chunk allocation state
        static char* start_free;
        static char* end_free;
        static size_t heap_size;
        
    public:
        static void* allocate(size_t n) {
            obj* ret;
            obj* volatile * my_free_list;
            
            if (n > _MAX_BYTES) {
                return malloc_alloc::allocate(n);
            }
            my_free_list = free_list + FREELIST_INDEX(n);
            ret = *my_free_list;
            if (ret == 0) {
                void* r = refill(ROUND_UP(n) );
                return r;
            }
            *my_free_list = ret->free_list_link;
            return ret;
        }
        
        static void deallocate(void* p, size_t n) {
            if (n > _MAX_BYTES) {
                malloc_alloc::deallocate(p, n);
            }
            else {
                obj* q = (obj*) p;
                obj* volatile * my_free_list = free_list + FREELIST_INDEX(n);
                q->free_list_link = *my_free_list;
                *my_free_list = q;
            }
        }
        
        static void* reallocate(void* p, size_t old_sz, size_t new_sz) {
            deallocate(p, old_sz);
            p = allocate(new_sz);
            return p;
        }
    };
    
    char* default_alloc::start_free = 0;
    char* default_alloc::end_free = 0;
    size_t default_alloc::heap_size = 0;
    default_alloc::obj* volatile default_alloc::free_list[_NFREELISTS] =
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
    
    void* default_alloc::refill(size_t n) {
        int nobjs = 20;
        char* chunk = chunk_alloc(n, nobjs);
        obj* volatile * my_free_list;
        obj* ret;
        obj* curr_obj; obj* next_obj;
        
        if (1 == nobjs) {
            return chunk;
        }
        
        ret = (obj*) chunk;
        my_free_list = free_list + FREELIST_INDEX(n);
        *my_free_list = next_obj = (obj*)(chunk + n);
        
        for (int i = 1; ; ++i) {
            curr_obj = next_obj;
            next_obj = (obj *)( (char*)curr_obj + n);
            if (nobjs-1 == i) {
                curr_obj->free_list_link = 0;
                break;
            }
            else {
                curr_obj->free_list_link = next_obj;
            }
        }
        return ret;
    }
    
    
    char* default_alloc::chunk_alloc(size_t size, int& nobjs) {
        char* ret;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free - start_free;
        
        if (bytes_left >= total_bytes) {
            // from memory pool
            ret = start_free;
            start_free += total_bytes;
            return ret;
        }
        else if (bytes_left >= size) {
            // from memory pool, but less than nobjs*size
            nobjs = (int)(bytes_left/size);
            total_bytes = size * nobjs;
            ret = start_free;
            start_free += total_bytes;
            return ret;
        }
        else{
            // from system heap
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size >> 4);
            // copy from SGI STL, not clear why doing this ROUND_UP(heap_size >> 4)
            if (bytes_left > 0) {
                // give bytes_left to appropriate free_list
                obj* volatile * my_free_list = free_list + FREELIST_INDEX(bytes_left);
                ( (obj*) start_free)->free_list_link = *my_free_list;
                *my_free_list = (obj*) start_free;
            }
            start_free = (char*)malloc(bytes_to_get);
            if (0 == start_free) {
                obj* volatile * my_free_list = free_list;
                obj* p;
                for (size_t i = size; i < _MAX_BYTES; i += _ALIGN) {
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if (0 != p) {
                        *my_free_list = p->free_list_link;
                        start_free = (char*) p;
                        end_free = start_free +i;
                        return chunk_alloc(size,nobjs);
                    }
                }
                end_free = 0;
                // try _oom_handler by using malloc_alloc
                start_free = (char*)malloc_alloc::allocate(bytes_to_get);
            }
            
            heap_size += bytes_to_get;
            end_free = start_free + bytes_to_get;
            return chunk_alloc(size, nobjs);
        }
    }
}

#endif
