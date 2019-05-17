#include "parameters.h"

void parseCommand(int argc, char* argv[]){
    if (argc == 1 || (argc < 4 && argv[1][0] == 'R') || (argc < 6 && argv[1][0] == 'S')){
        cerr << "$ sim ACCESS_TYPE[S/R] MODE_TYPE[D/F] RANDOM_ACCESS_PROB READ_REQ_PROB PRNG_SEED" << endl;
        exit(-1);
    }

    params.memory_mode = (argv[2][0] == 'D') ? DEFAULT : FAST;

    if (argv[1][0] == 'R'){
        params.cpu_access = REAL;
        params.file = argv[3];
    } else {
        params.cpu_access = STOCHASTIC;
        params.random_access_prob = atoi(argv[3]);
        params.read_req_prob = atoi(argv[4]);
        params.seed = atoi(argv[5]);
    }
}
