#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 5

//-----------simple channel mechanism------------

// A code where the channel are prototyped. Avoide modifying this code.
// Or modify it with care to what exactly happening.

typedef struct
{
    int buffer[BUFFER_SIZE];

    int count;
    int pull_index;
    int push_index;

    pthread_mutex_t lock;
    pthread_cond_t buffer_not_full;
    pthread_cond_t buffer_not_empty;

} Channel;

Channel *init_channel()
{
    Channel *ch = (Channel *)malloc(sizeof(Channel));

    ch->count = 0;
    ch->pull_index = 0;
    ch->push_index = 0;

    pthread_mutex_init(&ch->lock, NULL);
    pthread_cond_init(&ch->buffer_not_full, NULL);
    pthread_cond_init(&ch->buffer_not_empty, NULL);

    return ch;
}

void send(Channel *ch, int data)
{
    pthread_mutex_lock(&ch->lock);

    while (ch->count == BUFFER_SIZE)
    {
        printf("Sending halted: the buffer is full.\n");
        pthread_cond_wait(&ch->buffer_not_full, &ch->lock);
    }

    ch->buffer[ch->push_index] = data;
    ch->push_index = (ch->push_index + 1) % BUFFER_SIZE;
    ch->count++;

    pthread_cond_signal(&ch->buffer_not_empty);

    pthread_mutex_unlock(&ch->lock);
}

int recieve(Channel *ch)
{
    pthread_mutex_lock(&ch->lock);

    while (ch->count == 0)
    {
        printf("Recieving halted: the buffer is empty.\n");
        pthread_cond_wait(&ch->buffer_not_empty, &ch->lock);
    }
    int data = ch->buffer[ch->pull_index];
    ch->pull_index = (ch->pull_index + 1) % BUFFER_SIZE;
    ch->count--;

    pthread_cond_signal(&ch->buffer_not_full);

    pthread_mutex_unlock(&ch->lock);
    return data;
}

//-----------simple channel mechanism------------

//----------threads (Typical app)-------------

// Here's a simple use of the channels, this code is modifiable.

void *sender_thread(void *arg)
{
    Channel *ch = (Channel *)arg;

    for (int i = 0; i < 10; i++)
    {
        send(ch, i);
        printf("Sent: %d\n", i);
    }

    return NULL;
}

void *reciever_thread(void *arg)
{
    Channel *ch = (Channel *)arg;

    for (int i = 0; i < 10; i++)
    {
        int data = recieve(ch);

        printf("Recieved: %d\n", data);
    }

    return NULL;
}

//----------threads (Typical app)-------------

int main()
{
    pthread_t sender, reciever;
    Channel *ch = init_channel();

    pthread_create(&sender, NULL, sender_thread, ch);
    pthread_create(&reciever, NULL, reciever_thread, ch);

    pthread_join(sender, NULL);
    pthread_join(reciever, NULL);

    free(ch);

    return 0;
}
