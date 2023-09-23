//The time for regular commands (bottom left), and help (middle)

typedef struct Message_Queue *Message_Queue_t;

extern Message_Queue_t
Message_Queue_create(const unsigned int capacity);
extern void Message_Queue_enqueue(Message_Queue_t queue, char *message, unsigned int middle);
extern void Message_box_render(Message_Queue_t queue);
extern void Message_Queue_free(Message_Queue_t queue);
