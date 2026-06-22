#include <stdio.h> // printf
#include <fstream>    // read, write to file
#include <unistd.h>   // fork(), getpid()
#include <sys/wait.h> // wait()
#include <sys/file.h> // flock()
#include <fcntl.h>    // open()

using namespace std;
const char *FILE = "/tmp/F.txt";

void concurProcess() {
    pid_t pid = getpid();

    for (int i = 0; i < 100; i++) {
        int fd = open(FILE, O_RDWR);

    //error check
    if (fd == -1) {
        printf("Error opening file.");
        return;
    }

    // create a critical section, allowing only 1 process to hold the lock
    flock(fd, LOCK_EX);
    int value; 

    ifstream infile(FILE);

    if (!infile) {
        printf("Error reading file.");
        flock(fd, LOCK_UN);
        close(fd);
        return;
    }

    infile >> value; 
    infile.close();

    printf("PID %d read %d", pid, value);

    value++;

    ofstream outfile(FILE);

    if (!outfile) {
        printf("Error writing file.");
        flock(fd, LOCK_UN);
        close(fd);
        return;
    }

    outfile << value;
    outfile.close();

    flock(fd, LOCK_UN);
    close(fd);
    }

int main() {
    ofstream startFile(FILE);

    if (!startFile) {
        printf("Error making file");
        return 1;
    }

    startFile << 0;
    startFile.close();

    for 
}
}