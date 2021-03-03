/*
    * File     : LogFile.h
    * Author   : *
    * Mail     : *
    * Creation : Wed 27 Jan 2021 09:32:15 PM CST
*/

#ifndef _LOGFILE_H
#define _LOGFILE_H

#include <memory>
#include <string>

//#define ddd(arg) printf("%s \tline: %d, \tfunction: %s\n", arg, __LINE__, __func__)
#define ddd(arg) std::cout << arg << "\tline: " << __LINE__ << "\tfunction: " << __func__<< std::endl;

class Mutex;


class AppendFile {
public:
    explicit AppendFile(std::string filename);
    ~AppendFile();

    void append(const char *logline, std::size_t len);
    void flush();
    off_t written_bytes() const {
        return written_bytes_;
    }

private:
    size_t write(const char *logline, size_t len);
    FILE *fp_;
    char buffer_[64 * 1024];
    off_t written_bytes_;
};






class LogFile {
public:
    LogFile(const std::string &basename, off_t roll_size, bool thread_safe = true, int flush_interval = 3, int check_everyn = 1024);
    ~LogFile() = default;

    void append(const char *logline, int len);
    void flush();
    bool roll_file();
private:
    void append_unlock(const char *logline, int len);
    static std::string get_filename(const std::string &basename, time_t *now);

private:
    const std::string basename_;
    const off_t roll_size_;
    const int flush_interval_;
    const int check_everyn_;

    int count_;

    std::unique_ptr<Mutex> mutex_;
    time_t start_of_peroid_;
    time_t last_roll_;
    time_t last_flush_;

    std::unique_ptr<AppendFile> file_;
    const static int kRollPerSeconds = 60 * 60 * 24;
};

#endif
