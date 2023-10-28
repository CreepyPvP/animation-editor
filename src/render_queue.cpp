#include "render_queue.hpp"

#include <stdlib.h>


void RenderQueue::init(int capacity) {
    messageBuffer = (RenderMessage*) malloc(sizeof(RenderMessage) * capacity);
}

void RenderQueue::execute() {

}

void RenderQueue::push(RenderMessage message) {

}
