#ifndef BCE_QUEUE_H
#define BCE_QUEUE_H

#include <linux/completion.h>
#include <linux/pci.h>

struct bce_device;

enum bce_queue_type {
    BCE_QUEUE_CQ, BCE_QUEUE_SQ
};
struct bce_queue {
    int type;
};
struct bce_queue_cq {
    int type;
    void *data;

    int index;
};
struct bce_queue_sq {
    int type;
    int el_size;
    void *data;

    int head, tail;

    u32 expected_completion_index;
    void (*completion)(struct bce_queue_sq *q, u32 idx, u32 status, u64 data_size, u64 result);
};


enum bce_qe_completion_status {
    BCE_COMPLETION_SUCCESS = 0,
    BCE_COMPLETION_ERROR = 1,
    BCE_COMPLETION_ABORTED = 2,
    BCE_COMPLETION_NO_SPACE = 3,
    BCE_COMPLETION_OVERRUN = 4
};
enum bce_qe_completion_flags {
    BCE_COMPLETION_FLAG_PENDING = 0x8000
};
struct bce_qe_completion {
    u64 data_size;
    u64 result;
    ushort qid;
    ushort completion_index;
    ushort status; // bce_qe_completion_status
    ushort flags;  // bce_qe_completion_flags
};

static __always_inline void *bce_queue_sq_element(struct bce_queue_sq *q, int i) {
    return (void *) ((u8 *) q->data + q->el_size * i);
}
static __always_inline void *bce_queue_cq_element(struct bce_queue_cq *q, int i) {
    return (void *) ((struct bce_qe_completion *) q->data + i);
}

void bce_queue_handle_completions(struct bce_device *dev, struct bce_queue_cq *cq);

#endif //BCEDRIVER_MAILBOX_H