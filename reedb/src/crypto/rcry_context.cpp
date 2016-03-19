#include "rcry_context.h"
#include "rcry_engine.h"
#include <iostream>

rcry_context *rcry_context::self;

rcry_context::~rcry_context() { }

rcry_context::rcry_context() {
    this->count = 0;
    this->ready = false;
}

rcry_context *rcry_context::instance() {
    if (!self) self = new rcry_context();
    return self;
}

int rcry_context::get_count_incr() { return this->count++; }

int rcry_context::get_count() { return this->count; }

bool rcry_context::is_ready() { return this->ready; }

void rcry_context::rcry_context_update(rcry_engine **engines) { this->engines = engines; }

rcry_engine *rcry_context::get_engine_with_id(int id) {
    if (id < this->count) {
        return this->engines[id];
    }
}

void rcry_context::add_engine(rcry_engine *engine) {
    engines[count] = engine;
    std::cout << "Added engine id: " << engines[count]->query_id() << std::endl;
    count++;
}
