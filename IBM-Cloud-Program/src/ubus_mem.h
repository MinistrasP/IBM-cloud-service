struct memoryInfo{
    int memory_total;
    int memory_free;
    int memory_cached;
};

int get_mem(struct memoryInfo *buff);