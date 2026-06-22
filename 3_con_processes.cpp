#include <fcntl.h>    // open file
#include <stdio.h> // printf
#include <sys/wait.h> // wait()
#include <sys/file.h> // flock()
#include <fstream>    // read and write to file
#include <unistd.h>   // fork(), getpid()


using namespace std;
const char *FILE_F = "/tmp/F.txt";

void concurProcess() {
    pid_t pid = getpid();

    for (int i = 0; i < 100; i++) { // each process will obtain 100 integers
        int fd = open(FILE_F, O_RDWR); // open the file for reading and writing

    //error check
    if (fd == -1) {
        printf("Error opening file.");
        return;
    }

    // allow only 1 process to hold the lock at a time
    flock(fd, LOCK_EX);
    int value; 

    ifstream infile(FILE_F); // open the file for reading

    if (!infile) { // error check
        printf("Error reading file.");
        flock(fd, LOCK_UN); // release lock
        close(fd); // close file descriptor
        return;
    }

    infile >> value; // read value from file
    infile.close(); // close the input file

    printf("PID %d read %d\n", pid, value);

    value++;

    ofstream outfile(FILE_F); // open the file for writing

    if (!outfile) { // error check
        printf("Error writing file.");
        flock(fd, LOCK_UN); // release lock
        close(fd); // close file descriptor
        return;
    }

    outfile << value;
    outfile.close();

    flock(fd, LOCK_UN); // release lock
    close(fd); // close file descriptor
    }
}

int main() {
    ofstream startFile(FILE_F); // open the file for writing

    if (!startFile) { // error check
        printf("Error making file");
        return 1;
    }

    startFile << 0; // initialize the file with 0
    startFile.close(); // close the start file

    for (int i = 0; i < 3; i++) { // create 3 concurrent processes
        pid_t pid = fork();

        if (pid == 0) { // child process
            concurProcess();
            return 0;
        }

        else if (pid < 0) { // error check
            perror("Error forking process.");
            exit(1);
        }
    }

    for (int i = 0; i < 3; i++) { // wait forchild processes to finish
        wait(NULL);
    }
}