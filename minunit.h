/* from http://www.jera.com/techinfo/jtns/jtn002.html */ 
/* slightly modified */
#define MU_ASSERT(message, test) do { if (!(test)) return message; } while (0)
#define MU_RUN_TEST(test) do { char *message = test(); tests_run++; \
  if (message) return message; } while (0)
int tests_run;
