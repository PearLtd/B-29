#ifdef _WIN32
#define PATH "c:\\pear.txt"
#else
#define PATH "//home//abo//Work//Projects//LinuxDaemon//Test//log.txt"
#endif


int WriteToLog(char* str)
{
	FILE* log;
    log = fopen(PATH, "a+");
    if (log == NULL) return -1;
    fprintf(log, "%s\n", str);
    fclose(log);
    return 0;
}
