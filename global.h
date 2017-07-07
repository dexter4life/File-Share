#ifndef GLOBAL_H
#define GLOBAL_H

#define D_PORT 8184

#define MAXBUFFERLENGTH 2097152 //max length for buffers

#define WRITE(name, n) QFile __file(name); \
    __file.open(QIODevice::WriteOnly | QIODevice::Append);\
    __file.write((n));\
    __file.flush();\
    __file.close()

#define PRINTLOG(n) WRITE("C:/report.log", n)
#endif // GLOBAL_H

