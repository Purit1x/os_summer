struct sysinfo {
  uint64 freemem;   // amount of free memory (bytes)
  uint64 nproc;     // number of process
};

uint64 count_free_mem(void);
uint64 count_process(void);
