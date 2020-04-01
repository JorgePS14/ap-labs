#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <regex.h>

#define REPORT_FILE "packages_report.txt"

#define PRIME_SIZE 3089
int PRIME_MUL = 1787, PRIME_SUM = 419;

char* ACTION_REGEX = "([0-9]{4}-[0-9]{2}-[0-9]{2} [0-9]{2}:[0-9]{2})] \[[ALPM]+] (installed|upgraded|removed) ([_a-z0-9-]*[\.[_a-z0-9-]*]*) ";

int noInstalled = 0, noUpgraded = 0, noRemoved = 0;

struct Pac* arr[PRIME_SIZE] = {0};

struct Pac {
    char* name;
    char* installed;
    char* updated;
    char* removed;
    int updateCount;
};

int getHash(char* name) {
    unsigned int hash = 0;
    for(int i = 0; i < strlen(name); i++) {
        hash = hash * PRIME_SUM + name[i];
    }
    hash *= PRIME_MUL; 
    hash %= PRIME_SIZE;
    return hash;
}

int getPacIndex(char* name) {
    unsigned int hash = getHash(name);
    while(arr[hash]) {
        if(!strcmp(arr[hash]->name, name)) {
            return hash;
        }
        if(hash+PRIME_SUM >= PRIME_SIZE) {
            hash %= PRIME_SUM;
        }
        hash += PRIME_SUM;
    }
    return -1;
}

void newPackage(char* name, char* installed, char* updated, char* removed) {
    struct Pac* newPac = (struct Pac*)malloc(sizeof(struct Pac));

    newPac->name = strdup(name);
        
    newPac->installed = strdup(installed);
    
    newPac->updated = strdup(updated);

    newPac->removed = strdup(removed);

    newPac->updateCount = 0;

    unsigned int i = getHash(newPac->name);

    for(int j = 0; i < PRIME_SIZE; i=i+PRIME_SUM) {
        if(!arr[i]) {
            arr[i] = newPac;
            break;
        }
        if(strcmp(arr[i]->removed, "--") != 0) {
            arr[i]->installed = newPac->installed;
            break;
        }
        if(i+PRIME_SUM >= PRIME_SIZE) {
            i %= PRIME_SUM;
        }
    }

    noInstalled++;
}

void removePackage(char* name, char* dateRemoved){
    int index = getPacIndex(name);

    if(index > -1)
        arr[index]->removed = strdup(dateRemoved);

    noRemoved++;
}

void upgradePackage(char* name, char* dateUpgraded) {
    int index = getPacIndex(name);

    if(index > -1) {
        arr[index]->updated = strdup(dateUpgraded);
        arr[index]->updateCount += 1;
    }

    noUpgraded++;
}

int processLine(char *lineContent) {

    regex_t regex;
    regmatch_t matches[4];
    char *name = calloc(1, 48);
    char *action = calloc(1, 48);
    char *datePerformed = calloc(1, 48);

    if (regcomp(&regex, ACTION_REGEX, REG_EXTENDED)) {
        return 1;
    };

    if (regexec(&regex, lineContent, 4, matches, 0)) {
        return 1;
    }

    char content[strlen(lineContent) + 1];
    strcpy(content, lineContent);

    content[matches[1].rm_eo] = 0;
    strcpy(datePerformed, content + matches[1].rm_so);

    content[matches[2].rm_eo] = 0;
    strcpy(action, content + matches[2].rm_so);

    content[matches[3].rm_eo] = 0;
    strcpy(name, content + matches[3].rm_so);


    if(strstr(action, "installed")) {
        newPackage(name, datePerformed, "--", "--");
    } else if(strstr(action, "removed")) {
        removePackage(name, datePerformed);
    } else if(strstr(action, "upgraded")) {
        upgradePackage(name, datePerformed);
    }


    free(name);
    free(action);
    free(datePerformed);
    regfree(&regex);
    return 0;
}

void generateReport() {
    FILE* file = fopen(REPORT_FILE, "w");

	fprintf(file, "Pacman Packages Report\n");
	fprintf(file, "----------------------\n");

	fprintf(file, " - Installed packages: %d\n", noInstalled);
	fprintf(file, " - Removed packages: %d\n", noRemoved);
	fprintf(file, " - Upgraded packages: %d\n", noUpgraded);
	fprintf(file, " - Currently installed: %d\n", noInstalled - noRemoved);

	fprintf(file, "List of package data\n");
    fprintf(file, "----------------------\n");

	for(int i = 0; i < PRIME_SIZE; i++) {

		if(arr[i]) {
			fprintf(file, " - Package Name \t: %s\n", arr[i]->name);
			fprintf(file, "\t - Install date\t\t: %s\n", arr[i]->installed);
			fprintf(file, "\t - Last update date\t: %s\n", arr[i]->updated);
			fprintf(file, "\t - Number of updates: %d\n", arr[i]->updateCount);
			fprintf(file, "\t - Removal date\t\t: %s\n\n", arr[i]->removed);
		}
	}

	fclose(file);
}

int analizeLog(char *logFile) {
    printf("Generating Report from: [%s] log file\n", logFile);

    int f = open(logFile, O_RDONLY);

    if(f == -1) {
        printf("Error while trying to open file: %s\n", logFile);
        return 1;
    }

    int resp;
    char* charIndex = calloc(1, 1);
    char* lineContent = calloc(1, 256);

    while(1) {
        char* charIndex = calloc(1, 1);
        char* lineContent = calloc(1, 512);
        resp = read(f, charIndex, 1);
        
        if(resp == -1) {
            printf("Unexpected error while reading file, exiting now.\n");
            return resp;
        } else if(resp == 0) {
            break;
        }

        while(1) {
            if(charIndex[0] != '\n') {
                resp = read(f, charIndex, 1);

                if(resp == -1) {
                    printf("Unexpected error while reading file, exiting now.\n");
                    return resp;
                } else if(resp == 0) {
                    break;
                }

                strcat(lineContent, charIndex);
            } else {
                break;
            }
        }

        if(strstr(lineContent, "[ALPM] installed") || strstr(lineContent, "[ALPM] upgraded") || strstr(lineContent, "[ALPM] removed"))
            processLine(lineContent);

        free(lineContent);
    }

    close(f);

    generateReport();

    printf("Report is generated at: [%s]\n", REPORT_FILE);
}

int main(int argc, char **argv) {

    if (argc < 2) {
	    printf("Please use the following format: ./pacman-analizer <PacmanLogFile>\n");
	    return 1;
    }

    return analizeLog(argv[1]);
}