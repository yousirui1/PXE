#include "base.h"
#include <stdarg.h>

static FILE *fp_err = NULL;
static FILE *fp_log = NULL;

extern char program_name[];
extern time_t current_time;
static char c_dir[MAX_FILENAMELEN];

const char month_tab[48] =
    "Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec ";
const char day_tab[] = "Sun,Mon,Tue,Wed,Thu,Fri,Sat,";

int use_localtime = 0;   //夏时令


#ifdef _WIN32
    #define LOG_DIR  "c://Users//Public//Documents//YZYEduClient//Log"
    #define LOG_ERR_FILE "c://Users//Public//Documents//YZYEduClient//Log//remote_monitor_err.log"
    #define TTF_DIR "c://Windows//Fonts//simsunb.ttf"
#else
    #define LOG_DIR  "./log"
    #define LOG_ERR_FILE "./log/screen_cast_err.log"
    #define TTF_DIR "./font/msyh.ttf"
	#define TIMEZONE_OFFSET(foo) foo->tm_gmtoff
#endif  //WIN32


#define LOG_DIR  "./log"
#define LOG_ERR_FILE "./log/screen_cast_err.log"


/*
 * Name: get_commonlog_time
 *
 * Description: Returns the current time in common log format in a static
 * char buffer.
 *
 * commonlog time is exactly 25 characters long
 * because this is only used in logging, we add " [" before and "] " after
 * making 29 characters
 * "[27/Feb/1998:20:20:04 +0000] "
 *
 * Constrast with rfc822 time:
 * "Sun, 06 Nov 1994 08:49:37 GMT"
 *
 * Altered 10 Jan 2000 by Jon Nelson ala Drew Streib for non UTC logging
 *
 */
char *get_commonlog_time(void)
{
    struct tm *t;
    char *p;
    unsigned int a;
    static char buf[30];
    int time_offset;

    (void) time(&current_time);

    if (use_localtime) {
        t = localtime(&current_time);
#ifndef _WIN32
        time_offset = TIMEZONE_OFFSET(t);
#endif
    } else {
        t = gmtime(&current_time);
        time_offset = 0;
    }
    p = buf + 29;
    *p-- = '\0';
    *p-- = ' ';
    *p-- = ']';
    a = abs(time_offset / 60);
    *p-- = '0' + a % 10;
    a /= 10;
    *p-- = '0' + a % 6;
    a /= 6;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = (time_offset >= 0) ? '+' : '-';
    *p-- = ' ';

    a = t->tm_sec;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_min;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = t->tm_hour;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p-- = ':';
    a = 1900 + t->tm_year;
    while (a) {
        *p-- = '0' + a % 10;
        a /= 10;
    }
    /* p points to an unused spot */
    *p-- = '/';
    p -= 2;
    memcpy(p--, month_tab + 4 * (t->tm_mon), 3);
    *p-- = '/';
    a = t->tm_mday;
    *p-- = '0' + a % 10;
    *p-- = '0' + a / 10;
    *p = '[';
    return p;                   /* should be same as returning buf */
}





void init_logs()
{
    struct tm *t;
    struct stat file_state;
    int ret;

    (void)time(&current_time);
    t = localtime(&current_time);
    /* 检测文件夹是否存在 */
    ret = stat(LOG_DIR, &file_state);
    if (ret < 0)
    {
        if (errno == ENOENT)
        {
#ifdef _WIN32
            ret = mkdir(LOG_DIR);
#else
            ret = mkdir(LOG_DIR, 0755);
#endif
            if (ret < 0)
            {
                return ERROR;
            }
        }
    }
    sprintf(c_dir, "%s/%s-%4.4d%2.2d%2.2d.log", LOG_DIR, program_name, (1900 + t->tm_year), (1 + t->tm_mon), t->tm_mday);

    ret = stat(c_dir, &file_state);
    if (ret < 0)
    {
        if (errno == ENOENT)
        {
            fp_log = fopen(c_dir, "wb+");
        }
    }
    else
    {
        fp_log = fopen(c_dir, "ab+");
    }

    if (!fp_log)
    {
        DEBUG("fopen log error ");
    }

    fp_err = fopen(LOG_ERR_FILE, "ab");
    if (!fp_err)
    {
        DEBUG("fopen err_log error");
    }
}

void close_logs()
{
    if (fp_err)
        fclose(fp_err);

    if (fp_log)
        fclose(fp_log);

    fp_err = NULL;
    fp_log = NULL;
}

void log_msg(const char *fmt, ...)
{
    char buf[MAX_BUFLEN] = {0};
    char *ptr = buf;
    va_list ap;

    if (!fp_log)
        return;

    va_start(ap, fmt);
    vsprintf(ptr, fmt, ap);
    va_end(ap);

    fprintf(fp_log, "%s:%s", get_commonlog_time(), buf);
    fflush(fp_log);
}

void err_msg(const char *fmt, ...)
{
    char buf[MAX_BUFLEN] = {0};
    char *ptr = buf;
    va_list ap;

    if (!fp_err)
        return;

    va_start(ap, fmt);
    vsprintf(ptr, fmt, ap);
    va_end(ap);

    fprintf(fp_err, "%s:%s", get_commonlog_time(), buf);
    fflush(fp_err);
}


int upload_logs(char **buf, int *buf_len)
{
    fseek(fp_log, 0, SEEK_END);
    *buf_len = ftell(fp_log);
    DEBUG("log file len %d", *buf_len);
    fseek(fp_log, 0, SEEK_SET);
    char temp[123] = {0};

    *buf = (char *)malloc(*buf_len + 1);
    if(!(*buf))
    {
        DEBUG("log malloc size: %d error: %s", *buf_len, strerror(errno));
        return ERROR;
    }
    return fread(*buf, 1, *buf_len, fp_log);
}




