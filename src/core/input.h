#ifndef TERM_TRAIN_INPUT_INCLUDE__
#define TERM_TRAIN_INPUT_INCLUDE__

#define INPUT_SIZE (256)

typedef struct input_s{
    bool current[INPUT_SIZE];
    bool previous[INPUT_SIZE];
} Input;

void initTerm();
void restoreTerm();

void createInput(Input* input);
void processInput(Input* input);

bool isKeyPressed(Input* input, unsigned char key);
bool isKeyReleased(Input* input, unsigned char key);
bool ifKeyHolded(Input* input, unsigned char key);

#endif // TERM_TRAIN_INPUT_INCLUDE__
