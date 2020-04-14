int main(){
    int info = 10;
    char* warning = "This is a warning message";
    char* error = "The exit code is not what it's supposed to be.";
    infof("Info: %d \n", info);
    warnf("Warning: %s \n", warning);
    errorf("Error: %s \n", error);
    panicf("Panic! \n");
    return 0;
}