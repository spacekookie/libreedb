#include "rcry_helper.h"

rcry_context *rcry_context::self;

rcry_context::~rcry_context() {  }

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
