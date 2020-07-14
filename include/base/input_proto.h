/* base/input.c */
void input_init(void);
void input(void);
static void keybuffer_init(KeyBuffer *kb);
static void keybuffer_fini(KeyBuffer *kb);
static size_t keybuffer_size(KeyBuffer *kb);
static int kb_mask(int i);
static void keybuffer_push_back(KeyBuffer *kb, int k);
static int keybuffer_pop_front(KeyBuffer *kb);
int popkey(void);
void pushkey(int k);
_Bool haskey(void);
static void process_event(const SDL_Event *e);
static void process_key_event(const SDL_Event *e);
static void keyscan(void);
void get_mouse(int *x, int *y);
int keydecode(int scancode, int *special);
static int find_special(int code);
